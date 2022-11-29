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
    std::shared_ptr<ThreadPool> thread_pool = ThreadPool::ptr();
    std::cout << "--continue-- " << std::endl;
    thread_pool->commit(TaskLevel::DO_KEEP, &Epoll_Reactor::start_listen, Epoll_Reactor::ptr());
    thread_pool->startThreadPool();
    std::cout << "服务器运行ip: " << (*Setting::ptr())["target_server_ip"] << " port: "
    << (*Setting::ptr())["target_server_port"] << "\n";

    while (1) {
        std::string command;
        std::cin >> command;
        if (command == "quit") {
            std::cerr << "退出中" << std::endl;
            Setting::ptr()->save_setting();
            User_Manager::ptr()->save();
            break;
        } else {
            std::cout << "当前服务器正在运行， 输入quit即可退出程序" << std::endl;
        }
    }

    return 0;
}