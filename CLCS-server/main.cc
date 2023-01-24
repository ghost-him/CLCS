#include "Initializer_Module/Init.h"
#include "Epoll_Reactor_Module/Epoll_Reactor.h"
#include "Functional_Auxiliary_Module/Auxiliary.h"


// 半秒内输入的次数
int count = 0;

void count_mis() {
    // 半秒以后减少
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    count --;
}

int main(int argc, char** argv) {
    Auxiliary::startInit();
    // 解析参数
    if (!Command_Parameter_Analysis::parse(argc, argv))
        return 0;

    // 初始化各个模块
    Init::startInit();

    std::shared_ptr<Thread_Pool> thread_pool = Thread_Pool::ptr();
    OUT << (std::string)"--continue-- " + "\n";
    thread_pool->commit(TaskLevel::DO_KEEP, &Epoll_Reactor::start_listen, Epoll_Reactor::ptr());
    thread_pool->startThreadPool();
    OUT << "服务器运行ip: " + (*Setting::ptr())["target_server_ip"] + " port: "
    + (*Setting::ptr())["target_server_port"] + "\n";

    while (1) {
        // 正常阻塞，多线程安全
        Global_Option::input_lock.lock();
        std::string command;
        IN->get_line(command);
        if (command == "quit") {
            OUT << "退出中";
            Setting::ptr()->save_setting();
            User_Manager::ptr()->save();
            break;
        } else if (command == "block") {
            OUT << "已阻塞，输入 CTRL + C 恢复";
            More_Ctrl_C::set_state(Block_Input);
            Global_Option::input_lock.lock();
        } else {
            OUT << (std::string) "当前服务器正在运行， 输入quit即可退出程序, 输入 block 即可阻塞输入" + "\n";
        }

        // 输入的次数加1
        count ++;
        // 半秒以后减1
        thread_pool->commit(TaskLevel::DO_ONCE, count_mis);
        // 如果半秒以内输入的次数超过了10次，则阻塞
        if (count > 10) {
            Log::ptr()->log("[warn] main: Exceptions to the number of entries, block");
            OUT << "检测到异常的输入次数（单位时间内，输入次数大于规定的值），已阻塞，输入 CTRL + C 恢复";
            More_Ctrl_C::set_state(Block_Input);
        }
        Global_Option::input_lock.unlock();
    }

    return 0;
}