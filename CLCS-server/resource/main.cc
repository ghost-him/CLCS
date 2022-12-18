#include "Init.h"
#include "Message.h"
#include "Setting.h"
#include "Thread_Pool.h"
#include "User_Manager.h"
#include "stdinc.h"
#include "Epoll_Reactor.h"
#include "Log.h"

// 全局锁
std::mutex program_lock;
// 半秒内输入的次数
int count = 0;

// 恢复被阻塞的输入，并保存
void weakup(int) {
    Setting::ptr()->save_setting();
    User_Manager::ptr()->save();
    program_lock.unlock();
}

void count_mis() {
    // 半秒以后减少
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    count --;
}

int main(int argv, char** args) {
    // 判断程序是否设置了 block 参数
    if (argv > 1) {
        std::string temp = args[1];
        if (temp == "block")
            program_lock.try_lock();
    }

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
        program_lock.lock();
        std::string command;
        IN->get_line(command);
        if (command == "quit") {
            OUT << "退出中";
            Setting::ptr()->save_setting();
            User_Manager::ptr()->save();
            break;
        } else if (command == "block") {
            OUT << "已阻塞，输入 CTRL + C 恢复";
            program_lock.lock();
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
            program_lock.lock();
        }
        program_lock.unlock();
    }

    return 0;
}