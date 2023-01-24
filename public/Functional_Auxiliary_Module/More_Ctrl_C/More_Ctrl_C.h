#pragma once
#include "stdinc.h"
#include "Global_Variables/Default.h"
#include "Initializer_Module/Setting.h"
#include "User_Manager_Module/User_Manager.h"
#include "Message_IO_Module/Message_IO.h"

/*
 * 拓展ctrl-c的作用
 */

enum ctrl_state {
    Block_Input = 1,
    Block_Output
};

class More_Ctrl_C {
public :
    static void startInit();

    static std::shared_ptr<More_Ctrl_C> ptr();
    /*
     * 处理状态改变
     */
    static void request(int);
    /*
     * 设置状态
     * 参数：状态的参数
     */
    static void set_state(int);


private:
    More_Ctrl_C() = default;
    static std::shared_ptr<More_Ctrl_C> _ptr;

    static std::deque<int> _state;

    /*
     * 恢复输入状态
     */
    static void restore_input();
    /*
     * 恢复输出状态
     */
    static void restore_output();
};

