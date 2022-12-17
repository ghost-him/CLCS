#include "Epoll_Reactor.h"

std::shared_ptr<Epoll_Reactor> Epoll_Reactor::_ptr(new Epoll_Reactor());

std::shared_ptr<Epoll_Reactor> Epoll_Reactor::ptr() {
    return _ptr;
}

void Epoll_Reactor::set_ip_port(const std::string& ip, int port) {
    _ip = ip;
    _port = port;
}

void Epoll_Reactor::startInit() {
    log = Log::ptr();
    lang = Language::ptr();
    setting = Setting::ptr();
    // 设置服务
    _service = std::make_shared<Epoll_Reactor_Service>();
    _service->set_epoll_reactor(ptr());
    // 设置用户管理器
    u_m = User_Manager::ptr();

    // 初始化变量
    MAX_EVENT = std::stoi((*setting)["epoll_reactor_max_events"]);
    _max_connect = std::stoi((*setting)["epoll_reactor_max_connect"]);

    _ip = (*setting)["target_server_ip"];
    _port = std::stoi((*setting)["target_server_port"]);

    // 初始化运行队列
    _run_events = std::make_shared<epoll_event[]>(MAX_EVENT + 1);
    // 初始化监听树
    _root = epoll_create(MAX_EVENT + 1);
    // 判断是否创建成功
    if (_root <= 0) {
        log->log((*lang)["epoll_reactor_create_tree_failed"]);
        log->exit_process();
    }

    // 开启tcp监听

    // 获取文件描述符
    _server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_server_socket_fd == -1) {
        log->log((*lang)["epoll_reactor_create_socket_failed"]);
        log->exit_process();
    }

    // 设置端口复用
    int opt = 1;
    setsockopt(_server_socket_fd, SOL_SOCKET, SO_REUSEPORT, (void *)&opt, sizeof(opt));

    // 设置为非阻塞的
    if (!_service->set_nonblock(_server_socket_fd)) {
#ifdef DEBUG_EPOLL
        std::cerr << "set_nonblock error, closing the fd" << std::endl;
#endif
        shutdown(_server_socket_fd, SHUT_RDWR);
        close(_server_socket_fd);
        log->log("[error] set socket nonblock error");
        log->exit_process();
    }

    // 配置连接的属性
    _server_addr.sin_family = AF_INET;
    _server_addr.sin_port = htons(_port);

    if (_ip != "default") {    // TODO 修改默认属性， 用于储存方便
        int ret = inet_pton(AF_INET, _ip.c_str(), &_server_addr.sin_addr.s_addr);
        if (ret == 0) {
            // 有效的ip
            log->log((*lang)["epoll_reactor_invalid_ip"] + _ip);
            _server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        } else if (ret == -1) {
            // 无效的ip
            log->log((*lang)["epoll_reactor_convert_ip_error"]);
            log->exit_process();
        }
    } else {    // 若没有手动输入
        _server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }

    if (bind(_server_socket_fd, (sockaddr*) &_server_addr, sizeof(_server_addr)) == -1) {
        log->log((*lang)["epoll_reactor_bind_addr_failed"]);
        log->exit_process();
    }

    if (listen(_server_socket_fd, _max_connect) == -1) {
        log->log((*lang)["epoll_reactor_listen_error"]);
        log->exit_process();
    }

    // 守护进程
    std::shared_ptr<Event> deamon = std::make_shared<Event>();
    // 添加到存储器中
    store_add_event(deamon);
    // 设置属性
    deamon->set_socket_fd(_server_socket_fd);
#ifdef ET
    deamon->set_event_status(EPOLLIN | EPOLLET);
#else
    deamon->set_event_status(EPOLLIN);
#endif
    deamon->set_function(Epoll_Reactor_Service::accept_connection);
    // 添加到监听树中
    epoll_add_event(deamon);
}

void Epoll_Reactor::store_add_event(std::shared_ptr<Event> ptr) {
    _data_structure_lock.lock();
    _event_store.insert(ptr);
    _data_structure_lock.unlock();
}

void Epoll_Reactor::store_remove_event(std::shared_ptr<Event> ptr) {
    _data_structure_lock.lock();
    _event_store.erase(ptr);
    _user_store.erase(ptr->get_uuid());
    _data_structure_lock.unlock();
}

void Epoll_Reactor::epoll_add_event(std::shared_ptr<Event> ptr) {

    epoll_event events = {0, {nullptr}};

    events.data.ptr = ptr->get_epoll_event()->data.ptr;
    events.events = ptr->get_epoll_event()->events;
#ifdef DEBUG_EPOLL
    std::cerr << "Epoll_Reactor epoll_add_event " << ptr->get_socket_fd() << std::endl;
#endif
    _data_structure_lock.lock();
    if (epoll_ctl(_root,
                  EPOLL_CTL_ADD,
                  ptr->get_socket_fd(),
                  &events)) {
        log->log("[error] epoll reactor add event error %e , fd: " + std::to_string(ptr->get_socket_fd()));
    }
    _data_structure_lock.unlock();
}

void Epoll_Reactor::epoll_erase_event(std::shared_ptr<Event> ptr) {
    epoll_event temp = {0, {nullptr}};
    temp.data.ptr = nullptr;
    _data_structure_lock.lock();
    if (epoll_ctl(_root,
                  EPOLL_CTL_DEL,
                  ptr->get_socket_fd(),
                  &temp)) {
        log->log("[error] epoll reactor erase event error %e , fd: " + std::to_string(ptr->get_socket_fd()));
    }
    _data_structure_lock.unlock();
}

void Epoll_Reactor::epoll_flush_event(std::shared_ptr<Event> ptr) {
    epoll_event events = {0, {nullptr}};

    events.data.ptr = ptr->get_epoll_event()->data.ptr;
    events.events = ptr->get_epoll_event()->events;

    _data_structure_lock.lock();
    if (epoll_ctl(_root,
                  EPOLL_CTL_MOD,
                  ptr->get_socket_fd(),
                  &events)) {
    }
    _data_structure_lock.unlock();
}

void Epoll_Reactor::disconnect(std::shared_ptr<Event> ptr) {
#ifdef DEBUG_EPOLL
    std::cerr << "closing the connect, fd: " << ptr->get_socket_fd() << std::endl;
#endif
    shutdown(ptr->get_socket_fd(), SHUT_RDWR);
    close(ptr->get_socket_fd());
}

void Epoll_Reactor::start_listen() {
    int number_fd = epoll_wait(_root, _run_events.get(), MAX_EVENT + 1, 0);
    if (number_fd < 0) {
        log->log((*lang)["epoll_reactor_wait_error"]);
        log->exit_process();
    }

    for (int i = 0; i < number_fd; i++) {
        std::shared_ptr<Event> ptr = ((Event *)_run_events[i].data.ptr)->get_self();

#ifdef ET
        if ((_run_events[i].events & (EPOLLIN | EPOLLET)) && (ptr->get_epoll_event()->events & (EPOLLIN | EPOLLET))) {
            //Thread_Pool::ptr()->commit(TaskLevel::DO_ONCE, &Event::execute, ptr.get());
            ptr->execute();
        } else {
            epoll_flush_event(ptr);
        }

        if ((_run_events[i].events & (EPOLLOUT | EPOLLET)) && (ptr->get_epoll_event()->events & (EPOLLOUT | EPOLLET))) {
            //Thread_Pool::ptr()->commit(TaskLevel::DO_ONCE, &Event::execute, ptr.get());
            ptr->execute();
        } else {
            epoll_flush_event(ptr);
        }
#else
        if ((_run_events[i].events & (EPOLLIN)) && (ptr->get_epoll_event()->events & (EPOLLIN))) {
            //Thread_Pool::ptr()->commit(TaskLevel::DO_ONCE, &Event::execute, ptr.get());
            ptr->execute();
        }

        if ((_run_events[i].events & (EPOLLOUT)) && (ptr->get_epoll_event()->events & (EPOLLOUT))) {
            //Thread_Pool::ptr()->commit(TaskLevel::DO_ONCE, &Event::execute, ptr.get());
            ptr->execute();
        }
#endif

    }
}

Epoll_Reactor::Epoll_Reactor() {
    /*
     * 设置初始值
     */
    _root = 0;
    MAX_EVENT = 4096;
    _run_events = nullptr;

    bzero(&_server_addr, sizeof(_server_addr));
    _server_socket_fd = 0;
    _port = 0;

    _max_connect = 4096;
}

bool Epoll_Reactor::add_user(std::shared_ptr<Event> ptr,std::shared_ptr<Message_Stream> message) {
    // 添加在线用户
    ptr->set_uuid(message->analysis.get_source_uuid());
    _user_store.emplace(message->analysis.get_source_uuid(), ptr);
    // 检查当前的用户是否已经存在在数据库中
    auto answer = u_m->find_user(message->analysis.get_source_uuid());
    if (answer != nullptr)
        return true;

#ifdef DEBUG_EPOLL
    std::cerr << "add user: " << message->analysis.get_source_uuid() << std::endl;
#endif

    // 如果不存在，则创建一个
    User new_user;
    // 设置uuid
    new_user.set_uuid(message->analysis.get_source_uuid());
    // 设置公钥路径
    // 创建公钥文件
    std::string pub_path = File_Manager::ptr()->get("keys") + message->analysis.get_source_uuid() + ".pub";
    int file_fd = open(pub_path.c_str(), O_CREAT|O_RDWR, 0700);
    if (file_fd < 0) {
        log->log("[error] epoll_reactor: open file error:, %e");
        return false;
    }
    int ret = write(file_fd, message->get_content().get(), message->get_content_len());
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
    log->log("[info] epoll_reactor: add user finish");
    return true;
}