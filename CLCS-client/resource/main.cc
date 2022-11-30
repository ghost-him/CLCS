#include "Business.h"
#include "Init.h"
#include "Message.h"
#include "Setting.h"
#include "Thread_Pool.h"
#include "User_Manager.h"
#include "stdinc.h"

int main() {
#ifdef DEBUG_MAIN
    std::cerr << "enable the debug mode" << std::endl;
#endif

    // 初始化各个模块
    Init::startInit();

    // 连接服务器
    Business::init_server();

    auto thread_pool = Thread_Pool::ptr();
    thread_pool->startThreadPool();
    thread_pool->commit(TaskLevel::DO_KEEP, Business::start_accept_from_server);

    // 命令分析系统开始运行
    bool is_empty = false;
    while (1) {
        std::string command;
        IN->get_line(command);
        if (command.empty()) {
            is_empty = true;
            continue;
        }
        is_empty = false;

        // 如果command为quit, 则退出程序
        if (command == "quit") {
            OUT << "退出中";
            Server_Connector::ptr()->close_connect();
            Business::_is_closed = true;
            // 保存设置
            Business::save();
            break;
        }

        Command_Analysis::ptr()->check(std::move(command));
    }
#ifdef DEBUG_MAIN
    std::cerr << "end the program" << std::endl;
#endif
    return 0;
}