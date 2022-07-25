#include "stdinc.h"
#include "Init.h"
#include "Server_Connector.h"
#include "ThreadPool.h"
#include "Message.h"
#include "Epoll_Reactor.h"
#include "Business.h"


int main() {
    Init::startInit();
    Epoll_Reactor* er = Epoll_Reactor::getInstance();
    er->start_linsten();

    return 0;
}