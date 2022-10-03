#include "Business.h"
#include "Init.h"
#include "Message.h"
#include "Setting.h"
#include "ThreadPool.h"
#include "User_Manager.h"
#include "stdinc.h"

int main() {

    // 初始化各个模块
    Init::startInit();
    // 初始化业务系统
    Business::init_server();

    auto thread_pool = ThreadPool::getInstance();
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
        if (command == "quit")
            break;
        Command_Analysis::getInstance()->check(std::move(command));
    }

    // 保存设置
    Business::save();
    return 0;
}