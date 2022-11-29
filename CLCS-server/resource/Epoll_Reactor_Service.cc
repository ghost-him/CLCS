#include "Epoll_Reactor_Service.h"

void Epoll_Reactor_Service::set_epoll_reactor(const std::shared_ptr<Epoll_Reactor> &ptr) {
    _epoll_reactor = ptr;
    mg.startInit();
}

void Epoll_Reactor_Service::accept_connection() {
#ifdef DEBUG_EPOLL
    std::cerr << "Epoll_Reactor_Service accept_connection" << std::endl;
#endif
    sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_fd = 0;

    if ((client_fd = accept(_epoll_reactor->_server_socket_fd, (struct sockaddr *) &client_addr, &addr_len)) == -1) {
        if (errno != EAGAIN && errno != EINTR)
            _epoll_reactor->log->log((*_epoll_reactor->lang)["epoll_reactor_accept_error"]);
    }

    if (!set_nonblock(client_fd)) {
#ifdef DEBUG_EPOLL
        std::cerr << "set_nonblock error, closing the fd" << strerror(errno) << std::endl;
#endif
        close(client_fd);
        _epoll_reactor->log->log("[error] set socket nonblock error");
        return;
    }

    std::shared_ptr<Event> event = std::make_shared<Event>();
    _epoll_reactor->store_add_event(event);
    event->set_socket_fd(client_fd);
    event->set_event_status(EPOLLIN | EPOLLET);
    event->set_function(&Epoll_Reactor_Service::receive_message, _epoll_reactor->_service, event);
    _epoll_reactor->epoll_add_event(event);
#ifdef DEBUG_EPOLL
    std::cerr << "new fd: " << client_fd << std::endl;
#endif
}

void Epoll_Reactor_Service::receive_message(const std::shared_ptr<Event> &ptr) {
#ifdef DEBUG_EPOLL
    std::cerr << "Epoll_Reactor_Service receive message" << std::endl;
#endif
    std::cerr << "start to  receive message" << std::endl;

    std::shared_ptr<Message_Stream> message = ptr->get_temp_message();
    // 当有内容可以读的时候，就一直读
    if (!ptr->get_is_content()) {
        int head_len = -1;
        // 读取头文件
        do {
            head_len++;
            auto ret = read(ptr->get_socket_fd(), message->get_header().get() + head_len, 1);
            switch (ret) {
                case 0: {
#ifdef DEBUG_EPOLL
                    std::cerr << "Epoll_Reactor_Service client has closed the connection" << strerror(errno)
                              << std::endl;
#endif

                    // 客户端断开了连接
                    // 移除该事件
                    _epoll_reactor->epoll_erase_event(ptr);
                    // 断开了连接
                    _epoll_reactor->disconnect(ptr);
                    // 移除存储
                    _epoll_reactor->store_remove_event(ptr);
                    return;
                }
                case -1: {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        break;
                    } else {
                        // 读取异常
#ifdef DEBUG_EPOLL
                        std::cerr << "Epoll_Reactor_Service receive_message error, closing the fd" << strerror(errno)
                                  << std::endl;
#endif
                        _epoll_reactor->epoll_erase_event(ptr);
                        _epoll_reactor->disconnect(ptr);
                        _epoll_reactor->store_remove_event(ptr);
                        return;
                    }
                }
                default:
                    break;
            }
        } while (  head_len < 100 && *(message->get_header().get() + head_len) != '\n');

        if (!check_header(message->get_header())) {
            // 头消息
            // 无效的头标记
            _epoll_reactor->log->log(
                    "[error] epoll_reactor_service: invalid header: " + (std::string) message->get_header().get());
            return;
        } else {

        }

        // head_len的长度是消息的长度， 不包含最后的\n
        message->set_header_len(head_len + 1);
        // 解析头文件
        if (!message->analysis.convert()) {
            _epoll_reactor->log->log("[error] epoll reactor service: read invalid header");
        }
        // 设置数据的大小
        message->set_content_len(message->analysis.get_content_len());
        // 下一次读取数据
        ptr->set_is_content(true);
    } else {
        // 读取数据文件
        int remain_to_read = message->get_content_len();
        do {
            int ret = read(ptr->get_socket_fd(),
                           message->get_content().get() + (message->get_content_len() - remain_to_read),
                           remain_to_read);
            switch (ret) {
                case 0: {
                    // 客户端断开了连接
#ifdef DEBUG_EPOLL
                    std::cerr << "Epoll_Reactor_Service receive_message error, closing the fd " << strerror(errno)
                              << std::endl;
#endif
                    _epoll_reactor->epoll_erase_event(ptr);
                    _epoll_reactor->disconnect(ptr);
                    _epoll_reactor->store_remove_event(ptr);
                    return;
                }
                case -1: {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        break;
                    } else {
                        // 读取异常
#ifdef DEBUG_EPOLL
                        std::cerr << "Epoll_Reactor_Service receive_message error, closing the fd" << strerror(errno)
                                  << std::endl;
#endif
                        _epoll_reactor->epoll_erase_event(ptr);
                        _epoll_reactor->disconnect(ptr);
                        _epoll_reactor->store_remove_event(ptr);
                        return;
                    }
                }
                default:
                    break;
            }
            // 剩余要读的数据为减去当前已经读到的数据
            if (ret != -1)
                remain_to_read -= ret;
        } while (remain_to_read > 0);
        // 下一次读的时候开始读头标记
        ptr->set_is_content(false);

        // 获取当前事件的数据缓冲区
        auto buf = ptr->get_message_buf();
        // 将这次事件添加到数据缓冲区
        auto receive_message = message->copy();
        buf->push_receive_message(receive_message);
#ifdef DEBUG_EPOLL
        std::cerr << "receive header len: " << message->get_header_len() << "receive header: "
                  <<message->get_header()
                  << std::endl;
        std::cerr << "receive content len: " << message->get_content_len() << "receive content: "
                  << message->get_content() << std::endl;
#endif
        // 从监听树中删除节点
        _epoll_reactor->epoll_erase_event(ptr);
        // 分析数据，执行相应的函数
        analysis(ptr);
    }

    std::cerr << "end to receive" << std::endl;
}

void Epoll_Reactor_Service::analysis(const std::shared_ptr<Event> &ptr) {
#ifdef DEBUG_EPOLL
    std::cerr << "Epoll_Reactor_Service start to analysis message" << std::endl;
#endif
    // 获取第一个数据
    std::shared_ptr<Message_Stream> message = ptr->get_message_buf()->get_next_receive_message();

    if (message == nullptr) {
#ifdef DEBUG_EPOLL
        std::cerr << "Epoll_Reactor_Service analysis nullptr" << std::endl;
#endif
        ptr->set_event_status(EPOLLIN | EPOLLET);
        ptr->set_function(&Epoll_Reactor_Service::receive_message, _epoll_reactor->_service, ptr);
        _epoll_reactor->epoll_add_event(ptr);
        return;
    }

#ifdef DEBUG_EPOLL
    std::cerr << "Epoll_Reactor_Service get the message" << std::endl;
#endif

    // 获取第一个数据的头分析器
    Message_Header_Analysis &header = message->analysis;

#ifdef DEBUG_EPOLL
    std::cerr << "Epoll_Reactor_Service start to choice the message" << std::endl;
#endif
    switch (header.get_level()) {
        case MessageHeader::RECALL: {
            send_recall_message(ptr, message);
            break;
        }
        case MessageHeader::TEXT_SYS: {
            if (header.get_para1() == MessageHeader::INIT_SYS) {
#ifdef DEBUG_EPOLL
                std::cerr << "Epoll_Reactor_Service INIT_SYS" << std::endl;
#endif
                // 储存当前的用户
                if (!_epoll_reactor->add_user(ptr, message)) {
                    // 添加用户失败， 断开连接
                    _epoll_reactor->epoll_erase_event(ptr);
                    _epoll_reactor->disconnect(ptr);
                    _epoll_reactor->store_remove_event(ptr);
                }
#ifdef DEBUG_EPOLL
                std::cerr << "Epoll_Reactor_Service init the client" << std::endl;
#endif
                init_the_client(ptr, message);
            } else if (header.get_para1() == MessageHeader::ADD_USER) {
                // 客户端请求当前的uuid的信息
                send_user_key(ptr, message);
            } else if (header.get_para1() == MessageHeader::TIME) {
                // 直接返回该消息即可
                send_time_message(ptr, message);
            }
            break;
        }
        case MessageHeader::TEXT_CHAT: {
            redirect_the_message(ptr, message);
            break;
        }
        default:
            break;
    }

}


bool Epoll_Reactor_Service::set_nonblock(int fd) {
    int oldflag = fcntl(fd, F_GETFL, 0);
    // 如果不是-1.则返回true
    return fcntl(fd, F_SETFL, oldflag | O_NONBLOCK) != -1;
}


bool Epoll_Reactor_Service::check_header(const std::shared_ptr<char[]> &ptr) {
#ifdef DEBUG_EPOLL
    std::cerr << "Epoll_Reactor_Service start check_header message" << std::endl;
#endif
    switch (*ptr.get()) {
        case MessageHeader::RECALL: {
            if (*(ptr.get() + 2) != MessageHeader::NOMEAN || (*(ptr.get() + 4) != MessageHeader::NOMEAN))
                return false;
            break;
        }
        case MessageHeader::TEXT_CHAT: {
            if (*(ptr.get() + 2) != MessageHeader::NOMEAN || *(ptr.get() + 4) != MessageHeader::NOMEAN)
                return false;
            break;
        }
        case MessageHeader::TEXT_SYS: {
            if ((*(ptr.get() + 2) != MessageHeader::INIT_SYS && *(ptr.get() + 2) != MessageHeader::ADD_USER &&
                 *(ptr.get() + 2) != MessageHeader::TIME) || *(ptr.get() + 4) != MessageHeader::NOMEAN)
                return false;
            break;
        }
    }
    return true;
}

void Epoll_Reactor_Service::send_recall_message(const std::shared_ptr<Event> &ptr,
                                                const std::shared_ptr<Message_Stream> &message) {
#ifdef DEBUG_EPOLL
    std::cerr << "Epoll_Reactor_Service send_recall_message" << std::endl;
#endif
    auto buf = ptr->get_message_buf();
    // 回调消息，可以不进行处理，直接进行返回
    buf->push_send_message(message);

    // 改为发送状态
    ptr->set_event_status(EPOLLOUT | EPOLLET);
    ptr->set_function(&Epoll_Reactor_Service::send_message, _epoll_reactor->_service, ptr);
    _epoll_reactor->epoll_add_event(ptr);
}

void Epoll_Reactor_Service::init_the_client(const std::shared_ptr<Event> &ptr,
                                            const std::shared_ptr<Message_Stream> &message) {
#ifdef DEBUG_EPOLL
    std::cerr << "Epoll_Reactor_Service init_the_client" << std::endl;
#endif
    Message_Header_Analysis &analysis = message->analysis;
    mg.set_level_options(MessageHeader::TEXT_SYS,
                         MessageHeader::INIT_SYS,
                         MessageHeader::NOMEAN);
    // 发送的消息的目的地就是来源地
    mg.set_target_user_uuid(analysis.get_source_uuid());
    mg.set_content("ok", false);

#ifdef DEBUG_EPOLL
    std::cerr << "Epoll_Reactor_Service set the message" << std::endl;
#endif

    std::shared_ptr<Message_Stream> send_message = std::make_shared<Message_Stream>();

    // 设置头标记
    std::string header = std::move(mg.get_header());
    memcpy(send_message->get_header().get(), header.c_str(), header.size());

    // 设置头标记的长度
    send_message->set_header_len(header.size());

    // 设置内容的长度
    send_message->set_content_len(mg.get_content_size());
    // 设置内容
    memcpy(send_message->get_content().get(), mg.get_content().get(), mg.get_content_size());

    auto buf = ptr->get_message_buf();
    buf->push_send_message(send_message);

#ifdef DEBUG_EPOLL
    std::cerr << "Epoll_Reactor_Service set epollout" << std::endl;
#endif

    // 改为发送状态
    ptr->set_event_status(EPOLLOUT | EPOLLET);
    ptr->set_function(&Epoll_Reactor_Service::send_message, _epoll_reactor->_service, ptr);
    _epoll_reactor->epoll_add_event(ptr);
}

void Epoll_Reactor_Service::send_user_key(const std::shared_ptr<Event> &ptr,
                                          const std::shared_ptr<Message_Stream> &message) {
#ifdef DEBUG_EPOLL
    std::cerr << "Epoll_Reactor_Service send_user_key" << std::endl;
#endif
    Message_Header_Analysis &analysis = message->analysis;
    // 请求的uuid
    std::string uuid(37, '\0');
    for (int i = 0; i < 37; i++) {
        uuid[i] = static_cast<char>(message->get_content()[i]);
    }
    // 查找用户
    User *target_user = _epoll_reactor->u_m->find_user(uuid);

    std::string content = uuid;
    if (target_user == nullptr) {
        // 未找到
        _epoll_reactor->log->log("[warn] epoll_reactor: can not find request user");
        content += " null";
    } else {
        // 找到了
        content += " " + _epoll_reactor->u_m->get_key_content(target_user->get_pub_path());
    }
    mg.set_level_options(MessageHeader::TEXT_SYS,
                         MessageHeader::ADD_USER,
                         MessageHeader::NOMEAN);
    // 发送的目的地就是来源地
    mg.set_target_user_uuid(analysis.get_source_uuid());
    mg.set_content(content, false);

    // 要发送的数据
    std::shared_ptr<Message_Stream> send_message = std::make_shared<Message_Stream>();
    // 获取头标记
    std::string header = std::move(mg.get_header());
    memcpy(send_message->get_header().get(), header.c_str(), header.size());

    // 设置头标记的长度
    send_message->set_header_len(header.size());

    // 设置内容的长度
    send_message->set_content_len(mg.get_content_size());
    // 设置内容
    memcpy(send_message->get_content().get(), mg.get_content().get(), mg.get_content_size());

    auto buf = ptr->get_message_buf();
    buf->push_send_message(send_message);

    // 改为发送状态
    ptr->set_event_status(EPOLLOUT | EPOLLET);
    ptr->set_function(&Epoll_Reactor_Service::send_message, _epoll_reactor->_service, ptr);
    _epoll_reactor->epoll_add_event(ptr);
}

void Epoll_Reactor_Service::send_time_message(const std::shared_ptr<Event> &ptr,
                                              const std::shared_ptr<Message_Stream> &message) {
#ifdef DEBUG_EPOLL
    std::cerr << "Epoll_Reactor_Service send_time_message" << std::endl;
#endif
    // 使用和回调函数一样
    send_recall_message(ptr, message);
}

bool Epoll_Reactor_Service::redirect_the_message(const std::shared_ptr<Event> &ptr,
                                                 const std::shared_ptr<Message_Stream> &message) {
#ifdef DEBUG_EPOLL
    std::cerr << "Epoll_Reactor_Service redirect_the_message" << std::endl;
#endif
    Message_Header_Analysis &analysis = message->analysis;
    if (_epoll_reactor->_user_store.count(analysis.get_target_uuid()) == 0) {
        // 不存在这个人
        return false;
    }

    std::shared_ptr<Event> target_user = *_epoll_reactor->_user_store[analysis.get_target_uuid()];

    auto buf = target_user->get_message_buf();
    buf->push_send_message(message);

    // 自己继续监听，目标用户更改状态
    ptr->set_event_status(EPOLLIN | EPOLLET);
    ptr->set_function(&Epoll_Reactor_Service::receive_message, _epoll_reactor->_service, ptr);
    _epoll_reactor->epoll_add_event(ptr);

    // 删除在监听树中的目标
    _epoll_reactor->epoll_erase_event(target_user);
    // 更改目标的监听状态
    target_user->set_event_status(EPOLLOUT | EPOLLET);
    ptr->set_function(&Epoll_Reactor_Service::send_message, _epoll_reactor->_service, target_user);
    // 添加回去
    _epoll_reactor->epoll_add_event(target_user);
    return true;
}

void Epoll_Reactor_Service::send_message(const std::shared_ptr<Event> &ptr) {
#ifdef DEBUG_EPOLL
    std::cerr << "Epoll_Reactor_Service send message" << std::endl;
#endif

    std::cerr << "start to send" << std::endl;

    std::shared_ptr<Event_Buf> buf = ptr->get_message_buf();

    std::shared_ptr<Message_Stream> message = buf->get_next_send_message();

#ifdef DEBUG_EPOLL
    std::cerr << "-----------------------------------------------------------" << std::endl;
    std::cerr << "Epoll_Reactor_Service send message header len:" << message->get_header_len()
              << " header: ";
    write(STDERR_FILENO, std::reinterpret_pointer_cast<unsigned char[]>(message->get_header()).get(),
          message->get_header_len());
    std::cout << std::endl;
#endif

#ifdef DEBUG_EPOLL
    std::cerr << "Epoll_Reactor_Service send message content len:" << message->get_content_len()
              << " content: ";
    write(STDERR_FILENO, message->get_content().get(), message->get_content_len());
    std::cout << std::endl;
    std::cerr << "-----------------------------------------------------------" << std::endl;
#endif
    auto sender = ptr->get_sender();
    // 发送头文件的内容
    if (sender->send_message(std::reinterpret_pointer_cast<unsigned char[]>(message->get_header()),
                             message->get_header_len()) <= 0) {
#ifdef DEBUG_EPOLL
        std::cerr << "Epoll_Reactor_Service send message header error " << std::endl;
#endif
        _epoll_reactor->epoll_erase_event(ptr);
        _epoll_reactor->disconnect(ptr);
        _epoll_reactor->store_remove_event(ptr);
    }

    // 发送数据的内容
    if (sender->send_message(message->get_content(),
                             message->get_content_len()) <= 0) {
#ifdef DEBUG_EPOLL
        std::cerr << "Epoll_Reactor_Service send message content error " << std::endl;
#endif
        _epoll_reactor->epoll_erase_event(ptr);
        _epoll_reactor->disconnect(ptr);
        _epoll_reactor->store_remove_event(ptr);
    }

    _epoll_reactor->epoll_erase_event(ptr);
    ptr->set_event_status(EPOLLIN | EPOLLET);
    ptr->set_function(&Epoll_Reactor_Service::receive_message, _epoll_reactor->_service, ptr);
    _epoll_reactor->epoll_add_event(ptr);

#ifdef DEBUG_EPOLL
    std::cerr << "Epoll_Reactor_Service end send message" << std::endl;
#endif
    std::cerr << "end to send" << std::endl;
}