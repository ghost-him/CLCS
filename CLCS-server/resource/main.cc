#include "Init.h"
#include "Message.h"
#include "Setting.h"
#include "Thread_Pool.h"
#include "User_Manager.h"
#include "stdinc.h"
#include "Epoll_Reactor.h"

std::mutex program_lock;

void weakup(int) {
    program_lock.unlock();
}

int main() {
    signal(SIGINT, weakup);
    // 初始化各个模块
    Init::startInit();
    std::shared_ptr<Thread_Pool> thread_pool = Thread_Pool::ptr();
    OUT << (std::string)"--continue-- " + "\n";
    thread_pool->commit(TaskLevel::DO_KEEP, &Epoll_Reactor::start_listen, Epoll_Reactor::ptr());
    thread_pool->startThreadPool();
    OUT << "服务器运行ip: " + (*Setting::ptr())["target_server_ip"] + " port: "
    + (*Setting::ptr())["target_server_port"] + "\n";

    while (1) {
        program_lock.try_lock();
        std::string command;
        IN->get_line(command);
        if (command == "quit") {
            OUT << "退出中";
            Setting::ptr()->save_setting();
            User_Manager::ptr()->save();
            break;
        } else if (command == "back") {
            OUT << "已阻塞，输入 CTRL + C 恢复";
            program_lock.lock();
        } else {
            OUT << (std::string) "当前服务器正在运行， 输入quit即可退出程序, 输入back即可阻塞输入" + "\n";
        }
    }

    return 0;
}