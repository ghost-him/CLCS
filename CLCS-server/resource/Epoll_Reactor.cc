#include "Epoll_Reactor.h"
/*
 * 初始化static变量
 */

std::shared_ptr<Epoll_Reactor> Epoll_Reactor::_ptr(new Epoll_Reactor());
int Epoll_Reactor::MAX_EVENTS = 0;
int Epoll_Reactor::_epoll_root = 0;
std::list<Event> Epoll_Reactor::_events;
epoll_event* Epoll_Reactor::_run_events;
int Epoll_Reactor::_serv_fd = 0;
std::shared_ptr<Log> Epoll_Reactor::log = nullptr;
std::shared_ptr<Language> Epoll_Reactor::lang;
MessageGenerator Epoll_Reactor::mg;
std::shared_ptr<User_Manager> Epoll_Reactor::u_m = nullptr;
std::map<std::string, std::list<Event>::iterator*> Epoll_Reactor::_user_store;


std::shared_ptr<Epoll_Reactor> Epoll_Reactor::ptr() {
    return _ptr;
}


void Epoll_Reactor::Init_Epoll() {
    log = Log::ptr();
    lang = Language::ptr();
    setting = Setting::ptr();
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

    // 设置端口复用
    int opt = 1;
    setsockopt(_serv_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(opt));

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
    mg.startInit();
    u_m = User_Manager::ptr();
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

bool Epoll_Reactor::check(int ret) {
    if (ret > 0)
        return true;
    if (ret == 0) {
        // 连接关闭
        log->log((*lang)["epoll_reactor_client_close"]);
        return false;
    }
    log->log((*lang)["epoll_reactor_read_error"]);
    return false;
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
        set_event(now_event, client_fd, recvdata, &(now_event->_index));
        add_event(EPOLLIN, now_event);
        log->log((*lang)["epoll_reactor_new_connection"] + inet_ntoa(client_addr.sin_addr) + " port: " + std::to_string(ntohs(client_addr.sin_port)));
        return;
    }while(0);
}

void Epoll_Reactor::recvdata(std::list<Event>::iterator* event) {
    Event &now_event = **event;
    if (!now_event.is_content) {

        // 读取头文件====》一直读，直到读到 \n 字符则停止
        int total_len = -1;
        do {
            total_len ++;
            auto ret = read(now_event._socket_fd, now_event.header + total_len, 1);
            if (!check(ret)) {
                // 结束了， 但是非法
                if (errno == EAGAIN) {
                    return ;
                }
                // 如果是非法的
                shutdown((*event)->_socket_fd, 2);
                del_event(now_event);

                _events.erase(*event);
                return ;
            }

        }while (total_len < 100 && *(now_event.header + total_len) != '\n');
        // 如果当前的内容不是头消息
        if (!check_header(now_event.header)) {
#ifdef IS_DEBUG
            std::cerr << "invalid _header " <<now_event.header << std::endl;
#endif
            return ;
        }

        now_event.header_len = total_len + 1; // total_len的长度是消息的长度， 不包含最后的\n

        // 如果正常读取
        now_event.is_content = true;
        std::stringstream ss(now_event.header);
        // 读取头文件
        if (ss.eof()) {
            now_event.is_content = false;
            return ;
        }
        ss >> now_event.level;
        if (ss.eof()) {
            now_event.is_content = false;
            return ;
        }
        ss >> now_event.option1;
        if (ss.eof()) {
            now_event.is_content = false;
            return ;
        }
        ss >> now_event.option2;
        if (ss.eof()) {
            now_event.is_content = false;
            return ;
        }
        ss >> now_event.target_uuid;
        if (ss.eof()) {
            now_event.is_content = false;
            return ;
        }
        ss >> now_event.source_uuid;
        if (ss.eof()) {
            now_event.is_content = false;
            return ;
        }
        ss >> now_event.content_len;
    } else {
        // 如果是文件内容
        now_event._buf.reset(new unsigned char [now_event.content_len]);
        // 读取消息内容
        int remain_to_read = now_event.content_len;
        do {
            int ret = read(now_event._socket_fd, now_event._buf.get() + (now_event.content_len - remain_to_read), remain_to_read);

            if (!check(ret)) {
                // 如果是非法的
                if (errno == EAGAIN) {
                    log->log("[error] epoll_reactor: read error can not read more");
                    break;
                }
                shutdown((*event)->_socket_fd, 2);
                del_event(now_event);
                _events.erase(*event);
                return;
            }
            // 剩余要读的数据为减去当前已经读到的数据
            remain_to_read -= ret;
        } while (remain_to_read > 0);
        // 分析当前的数据
        analysis(event);
        now_event.is_content = false;
    }
}
/*
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
 */

void Epoll_Reactor::start_listen() {

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

void Epoll_Reactor::analysis(std::list<Event>::iterator * event) {
    Event& now_event = **event;
#ifdef IS_DEBUG
    std::cerr << "receive _header" << now_event.header << " " << "content" << now_event._buf << std::endl;
#endif
    switch (now_event.level) {
        case MessageHeader::RECALL: {
            del_event(now_event);
            set_event(*event, now_event._socket_fd, send_self_recall,event);
            add_event(EPOLLOUT, *event);
            break;
        }
        case MessageHeader::TEXT_SYS: {
            if (now_event.option1 == MessageHeader::INIT_SYS) {
                // 储存当前的用户
                if (!add_user(event)) {
                    // 添加用户失败， 断开连接
                    shutdown((*event)->_socket_fd, 2);
                    del_event(now_event);
                }
                init_client(event);
            } else if (now_event.option1 == MessageHeader::ADD_USER) {
                // 客户端请求当前的uuid的信息
                return_user_message(event);
            } else if (now_event.option1 == MessageHeader::TIME) {
                // 直接返回该消息即可
                del_event(now_event);
                set_event(*event, now_event._socket_fd, send_self_recall,event);
                add_event(EPOLLOUT, *event);
            }

            break;
        }
        case MessageHeader::TEXT_CHAT: {
            forwarding_message(event);
            break;
        }
    }
}

void Epoll_Reactor::send_self_recall(std::list<Event>::iterator *event) {
    Event& now_event = **event;
    // 发送头文件
#ifdef IS_DEBUG
    std::cerr << "start send_self_recall" << std::endl;
#endif

    if (!send_message(now_event._socket_fd, reinterpret_cast<unsigned char*>(now_event.header), now_event.header_len)) {
        log->log("[error] send _header error: %e");
        shutdown((*event)->_socket_fd, 2);
        del_event(now_event);
        _events.erase(*event);
    }
    // 发送消息内容
    if (!send_message(now_event._socket_fd, now_event._buf.get(), now_event.content_len)) {
        log->log("[error] send message error: %e");
        shutdown((*event)->_socket_fd, 2);
        del_event(now_event);
        _events.erase(*event);
    }
    del_event(now_event);
    set_event(*event, now_event._socket_fd, recvdata, event);
    add_event(EPOLLIN, *event);
}

bool Epoll_Reactor::send_message(int fd, unsigned char * buf, int len) {
#ifdef IS_DEBUG
    std::cerr << "send message: " << buf << std::endl;
#endif

    int remain_to_send = len;
    do{
        int ret = write(fd, buf + (len - remain_to_send), remain_to_send);
        if (!check(ret)) {
            if (errno == EAGAIN) {
                log->log("[error] epoll_reactor: can not write message, %e");
                break;
            }
            return false;
        }
        remain_to_send -= ret;
    }while (remain_to_send > 0);
    return true;
}

bool Epoll_Reactor::check_header(const char * header) {
    switch (*header) {
        case MessageHeader::RECALL: {
            if (*(header + 2) != MessageHeader::NOMEAN || (*(header +4) != MessageHeader::NOMEAN))
                return false;
            break;
        }
        case MessageHeader::TEXT_CHAT: {
            if (*(header + 2) != MessageHeader::NOMEAN || *(header + 4) != MessageHeader::NOMEAN)
                return false;
            break;
        }
        case MessageHeader::TEXT_SYS: {
            if ((*(header + 2) != MessageHeader::INIT_SYS && *(header + 2) != MessageHeader::ADD_USER && *(header + 2) != MessageHeader::TIME) || *(header + 4) != MessageHeader::NOMEAN)
                return false;
            break;
        }
    }
    return true;
}

bool Epoll_Reactor::add_user(std::list<Event>::iterator * event) {
    Event& now_event = **event;
    // 检查当前的用户是否已经存在
    auto answer = u_m->find_user(now_event.source_uuid);
    if (answer != nullptr)
        return true;
    // 如果不存在， 则创建一个
    User new_user;
    // 设置uuid
    new_user.set_uuid(now_event.source_uuid);
    // 设置公钥路径
    // 创建公钥文件
    std::string pub_path = FileManager::ptr()->get("keys") + now_event.source_uuid + ".pub";
    int file_fd = open(pub_path.c_str(), O_CREAT|O_RDWR, 0700);
    if (file_fd < 0) {
        log->log("[error] epoll_reactor: open file error:, %e");
        return false;
    }
    int ret = write(file_fd, now_event._buf.get(), now_event.content_len);
    if (ret < 0) {
        log->log("[error] epoll_reactor: write user pub key error: %e");
        // 删除已经创建的文件
        remove(pub_path.c_str());
        return false;
    }
    close(file_fd);
    // 添加到管理器中
    new_user.set_pub_path(pub_path);
    u_m->add_user(new_user);
    _user_store.emplace(now_event.source_uuid, event);
    log->log("[info] epoll_reactor: add user finish");
    return true;
}

bool Epoll_Reactor::init_client(std::list<Event>::iterator* event) {
    Event& now_event = **event;
    mg.set_level_options(MessageHeader::TEXT_SYS, MessageHeader::INIT_SYS, MessageHeader::NOMEAN);
    mg.set_target_user_uuid(now_event.source_uuid);
    mg.set_content("ok", false);
    // 设置头消息
    strcpy(now_event.header, mg.get_header().c_str());
    now_event.header_len = mg.get_header().size();
    // 设置发送的内容
    now_event._buf = mg.get_content();
    now_event.content_len = mg.get_content_size();
    // 设置为输出模式
    del_event(now_event);
    set_event(*event, now_event._socket_fd, send_self_recall,event);
    add_event(EPOLLOUT, *event);
    return true;
}

bool Epoll_Reactor::return_user_message(std::list<Event>::iterator* event) {
    Event& now_event = **event;
    // 请求的uuid
    std::string uuid(36, '\0');
    for (int i = 0; i < 37; i ++) {
        uuid[i] = static_cast<char>(now_event._buf.get()[i]);
    }
    // 查找该用户
    User* target_user = u_m->find_user(uuid);

    std::string content = uuid;
    if (target_user == nullptr) {
        // 未找到
        log->log("[warn] epoll_reactor: can not find request user");
        content += " null";
    } else {
        // 找到了
        content += " " + u_m->get_key_content(target_user->get_pub_path());
    }
    mg.set_level_options(MessageHeader::TEXT_SYS, MessageHeader::ADD_USER, MessageHeader::NOMEAN);
    mg.set_content(content, false);
    mg.set_target_user_uuid(now_event.source_uuid);

    // 设置头消息
    strcpy(now_event.header, mg.get_header().c_str());
    now_event.header_len = mg.get_header().size();
    // 设置发送的内容
    now_event._buf = mg.get_content();
    now_event.content_len = mg.get_content_size();
    // 设置为输出模式
    del_event(now_event);
    set_event(*event, now_event._socket_fd, send_self_recall,event);
    add_event(EPOLLOUT, *event);
    return true;
}

bool Epoll_Reactor::forwarding_message(std::list<Event>::iterator * event) {
#ifdef IS_DEBUG
    std::cerr << "start forwarding_message" << std::endl;
#endif
    // 将消息原封不动地转发到target_uuid上
    Event& now_event = **event;
    if (_user_store.count(now_event.target_uuid) == 0) {
        return false;
    }
    std::list<Event>::iterator* target_user = _user_store[now_event.target_uuid];
    // 复制头文件的消息
    Event &target = **target_user;
    memcpy(target.header, now_event.header, now_event.header_len);
    target.header_len = now_event.header_len;
    // 复制消息的内容
    target._buf = now_event._buf;
    target.content_len = now_event.content_len;

    del_event(target);
    set_event(*target_user, target._socket_fd, send_self_recall, target_user);
    add_event(EPOLLOUT, *target_user);

    return true;
}

