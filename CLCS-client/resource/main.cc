#include "Business.h"
#include "Init.h"
#include "Message.h"
#include "Setting.h"
#include "ThreadPool.h"
#include "User_Manager.h"
#include "stdinc.h"

int main() {
#ifdef IS_DEBUG
    std::cerr << "enable the debug mode" << std::endl;
#endif

    // 初始化各个模块
    Init::startInit();

    // 连接服务器
    Business::init_server();

    auto thread_pool = ThreadPool::ptr();
    thread_pool->startThreadPool();
    thread_pool->commit(TaskLevel::DO_KEEP, Business::start_accept_from_server);

    // 命令分析系统开始运行
    bool is_empty = false;
    while (1) {
        // 第一次读的时候会读取到空行
        if (!is_empty)
            std::cout << "CLCS > " << std::flush;
        std::string command;
        getline(std::cin, command);
        if (command.empty()) {
            is_empty = true;
            continue;
        }
        is_empty = false;

        // 如果command为quit, 则退出程序
        if (command == "quit") {
            Server_Connector::ptr()->close_connect();
            break;
        }

        Command_Analysis::ptr()->check(std::move(command));
    }

    // 保存设置
    Business::save();
#ifdef IS_DEBUG
    std::cerr << "end the program" << std::endl;
#endif
    return 0;
}