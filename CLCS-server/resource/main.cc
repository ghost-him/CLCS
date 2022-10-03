#include "Init.h"
#include "Message.h"
#include "Setting.h"
#include "ThreadPool.h"
#include "User_Manager.h"
#include "stdinc.h"
#include "Epoll_Reactor.h"




int main() {
    // 初始化各个模块
    Init::startInit();
    auto thread_pool = ThreadPool::getInstance();
    std::cout << "--continue-- " << std::endl;
    thread_pool->commit(TaskLevel::DO_ONCE, Epoll_Reactor::start_listen);
    thread_pool->startThreadPool();

    std::cout << "服务器运行ip: " << (*Setting::getInstance())["target_server_ip"] << " port: "
    << (*Setting::getInstance())["target_server_port"] << "\n";

    while (1) {
        std::string command;
        std::cin >> command;
        if (command == "quit") {
            Setting::getInstance()->save_setting();
            User_Manager::getInstance()->save();
            break;
        } else {
            std::cout << "当前服务器正在运行， 输入quit即可退出程序" << std::endl;
        }
    }

    return 0;
}