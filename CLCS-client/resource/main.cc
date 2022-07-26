#include "stdinc.h"
#include "Init.h"
#include "Server_Connector.h"
#include "ThreadPool.h"
#include "Message.h"
#include "Business.h"

// TODO 检测连接是否成功， 如果未连接成功， 则退出程序

int main() {
    Init::startInit();
    Server_Connector::getInstance()->start_connect();
    ThreadPool::getInstance()->startThreadPool();

    auto threadpool = ThreadPool::getInstance();
    MessageSender::getInstance()->InitMessageSender();
    MessageReceiver::getInstance()->InitMessageReceiver();
    threadpool->commit(TaskLevel::DO_KEEP, Business::start_accept_from_server);

    std::cout << "输入quit即可退出程序" << std::endl;
    while (1) {
        std::string line;
        std::cin >> line;
        if (line.size() > 8100) {
            continue;
        }

        if (line == "quit")
            break;
        auto ms = MessageSender::getInstance();
        ms->send_message(line);
    }

    Server_Connector::getInstance()->close_connect();

    return 0;
}