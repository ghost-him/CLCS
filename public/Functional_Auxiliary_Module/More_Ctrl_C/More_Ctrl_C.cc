#include "More_Ctrl_C.h"

std::shared_ptr<More_Ctrl_C> More_Ctrl_C::_ptr(new More_Ctrl_C);
std::deque<int> More_Ctrl_C::_state;

std::shared_ptr<More_Ctrl_C> More_Ctrl_C::ptr() {
    return _ptr;
}

void More_Ctrl_C::startInit() {
    signal(SIGINT, request);
}

void More_Ctrl_C::request(int sig) {
    // 如果有状态
    if (!_state.empty()) {
        auto i = _state.front();
        _state.pop_front();

        switch (i) {
            case Block_Input: {
                restore_input();
                break;
            }
            case Block_Output: {
                restore_output();
                OUT << "";
                break;
            }
        }
    } else {
        OUT << "已阻塞输出，再次输入ctrl-c即可恢复";
        set_state(Block_Output);
    }

}

void More_Ctrl_C::set_state(int state) {
    switch (state) {
        case Block_Input: {
            Global_Option::input_lock.try_lock();
            break;
        }
        case Block_Output: {
            Global_Option::is_block_output = true;
            break;
        }
    }
    _state.emplace_back(state);
}

void More_Ctrl_C::restore_input() {
    OUT << "已恢复输入";
    Setting::ptr()->save_setting();
    User_Manager::ptr()->save();
    Global_Option::input_lock.unlock();
}

void More_Ctrl_C::restore_output() {
    OUT << "已恢复输出";
    Global_Option::is_block_output = false;
}

