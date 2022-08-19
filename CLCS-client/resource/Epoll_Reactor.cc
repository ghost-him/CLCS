#include "Epoll_Reactor.h"
/*
 * 初始化static变量
 */

Epoll_Reactor* Epoll_Reactor::_pEpoll_Reactor = new Epoll_Reactor;
int Epoll_Reactor::MAX_EVENTS = 0;
int Epoll_Reactor::_epoll_root = 0;
std::list<Event> Epoll_Reactor::_events;
epoll_event* Epoll_Reactor::_run_events;
int Epoll_Reactor::_serv_fd = 0;
Log* Epoll_Reactor::log;
Language* Epoll_Reactor::lang;


Epoll_Reactor* Epoll_Reactor::getInstance() {
    return _pEpoll_Reactor;
}


void Epoll_Reactor::Init_Epoll() {
    log = Log::getInstance();
    lang = Language::getInstance();
    setting = Setting::getInstance();
    // 初始化变量
    MAX_EVENTS = std::stoi((*setting)["epoll_reactor_max_events"]);
    _max_connect = std::stoi((*setting)["epoll_reactor_max_connect"]);

    _ip = (*setting)["target_server_ip"];
    _port = std::stoi((*setting)["target_server_port"]);

    _run_events = new epoll_event [MAX_EVENTS + 1];
    // 初始化监听树
    _epoll_root = epoll_create(MAX_EVENTS + 1);
    if (_epoll_root <= 0) {
        log->log((*lang)["epoll_reactor_create_tree_failed"]);
        log->exit_process();
    }
    // 设置为非阻塞模式
	_serv_fd = socket(AF_INET, SOCK_STREAM | O_NONBLOCK, 0);
    if (_serv_fd == -1) {
        log->log((*lang)["epoll_reactor_create_socket_failed"]);
        log->exit_process();
    }
    // 初始化本地的地址
    bzero(&_server_addr, sizeof(_server_addr));
    _server_addr.sin_family = AF_INET;
    _server_addr.sin_port = htons(_port);
    // 判断是否自己手动输入
    if (_ip != "default") {    // TODO 修改默认属性， 用于储存方便
        int ret = inet_pton(AF_INET, _ip.c_str(), &_server_addr.sin_addr.s_addr);
        if (ret == 0) { //
            log->log((*lang)["epoll_reactor_invalid_ip"] + _ip);
            _server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        } else if (ret == -1) {
            log->log((*lang)["epoll_reactor_convert_ip_error"]);
            log->exit_process();
        }
    } else {    // 若没有手动输入
        _server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }

    if (bind(_serv_fd, (sockaddr*) &_server_addr, sizeof(_server_addr)) == -1) {
        log->log((*lang)["epoll_reactor_bind_addr_failed"]);
        log->exit_process();
    }

    if (listen(_serv_fd, _max_connect) == -1) {
        log->log((*lang)["epoll_reactor_listen_error"]);
        log->exit_process();
    }

    Event deamon;
    _events.push_front(deamon);
    auto now_event= _events.begin();
    set_event(now_event, _serv_fd, acception_connection);
    add_event(EPOLLIN, now_event);
}

void Epoll_Reactor::set_ip(std::string& ip) {
    _ip = ip;
    (*setting)["target_server_ip"] = ip;
}
std::string Epoll_Reactor::get_ip() {
    if (!_ip.empty())
        return _ip;
    return "default ip";
}

void Epoll_Reactor::set_port(int port) {
    _port = port;
    (*setting)["target_server_port"] = std::to_string(port);
}

int Epoll_Reactor::get_port() const {
    return _port;
}

void Epoll_Reactor::add_event(int events, std::list<Event>::iterator& event) {
    epoll_event epv = {0, {nullptr}};
    int op = EPOLL_CTL_ADD;
    epv.data.ptr = &event;
    epv.events = (*event)._events = events;

    if (epoll_ctl(_epoll_root, op, (*event)._socket_fd, &epv) < 0) {
        log->log((*lang)["epoll_reactor_add_event_error"]);
    } else {

    }
}

void Epoll_Reactor::del_event(Event & event) {
    struct epoll_event epv = {0, {0}};

    epv.data.ptr = nullptr;

    // 从监听树中删除
    epoll_ctl(_epoll_root, EPOLL_CTL_DEL, event._socket_fd, &epv);
}


Epoll_Reactor::Epoll_Reactor() {
}

Epoll_Reactor::~Epoll_Reactor() {
}

void Epoll_Reactor::acception_connection() {
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_fd;

    if ((client_fd = accept(_serv_fd, (struct sockaddr*)&client_addr, &addr_len)) == -1) {
        if (errno != EAGAIN && errno != EINTR)
            log->log((*lang)["epoll_reactor_accept_error"]);
    }

    // 枚举可以插入的位置
    int index = 0;
    do {
        // 设置为非阻塞
        int flag = 0;
        if ((flag = fcntl(client_fd, F_SETFL, O_NONBLOCK)) < 0) {
            log->log((*lang)["epoll_reactor_set_no_block_error"]);
            break;
        }

        Event temp;
        _events.push_front(temp);
        auto now_event = _events.begin();
        now_event->_index = _events.begin();
        set_event(now_event, client_fd, recvdata, client_fd, &(now_event->_index));
        add_event(EPOLLIN, now_event);
        log->log((*lang)["epoll_reactor_new_connection"] + inet_ntoa(client_addr.sin_addr) + " port: " + std::to_string(ntohs(client_addr.sin_port)));
        return;
    }while(0);
}

void Epoll_Reactor::recvdata(int socketfd, std::list<Event>::iterator* event) {
    Event &now_event = **event;
    int len = read(socketfd, now_event._buf, sizeof(now_event._buf));
    del_event(now_event);
    if (len > 0) {
        now_event._buf[len] = '\0';
        strcat(now_event._buf, " server received\n");
        now_event._len = len + strlen(" server received\n");
        set_event(now_event._index, socketfd, senddata, socketfd, event);
        add_event(EPOLLOUT, *event);
    } else if (len == 0) {
        log->log((*lang)["epoll_reactor_client_close"]);
        _events.erase(*event);
        close(socketfd);
    } else {
        log->log((*lang)["epoll_reactor_read_error"]);
        _events.erase(*event);
        close(socketfd);
    }
}

void Epoll_Reactor::senddata(int socketfd, std::list<Event>::iterator* event) {
    Event &now_event = **event;
    int len = write(socketfd, now_event._buf, now_event._len);
    del_event(now_event);

    if (len > 0) {
        // 发送成功
        set_event(*event, socketfd, recvdata, socketfd, event);
        add_event(EPOLLIN, *event);
    } else {
        log->log((*lang)["epoll_reactor_write_error"]);
        _events.erase(*event);
        close(socketfd);
    }
}

void Epoll_Reactor::start_linsten() {
    while (1) {
        int nfd = epoll_wait(_epoll_root, _run_events, MAX_EVENTS + 1, -1);
        if (nfd < 0) {
            log->log((*lang)["epoll_reactor_wait_error"]);
            log->exit_process();
        }

        for (int i = 0; i < nfd; i++) {
            Event &temp = **(std::list<Event>::iterator *)_run_events[i].data.ptr;
            if ((_run_events[i].events & EPOLLIN) && (temp._events & EPOLLIN)) {
                temp._func();
            }

            if ((_run_events[i].events & EPOLLOUT) && (temp._events & EPOLLOUT)) {
                temp._func();
            }
        }
    }
}














