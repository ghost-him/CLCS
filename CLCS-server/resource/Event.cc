#include "Event.h"

Event::Event() {
    _buf = std::make_shared<Event_Buf>();
    _epoll_event.events = 0;
    _epoll_event.data.ptr = this;
    _sender = std::make_shared<Server_Message_Sender>();
    // 第一个Event类：deamon 变量，这个会在 Epoll_reactor.startInit()中生成，所以其他的类已经
    // 创建完成，所以可以直接调用startInit方法，无需担心空指针的问题
    _sender->startInit();
    _temp_message = std::make_shared<Message_Stream>();
}

void Event::set_socket_fd(int fd) {
    _sender->set_fd(fd);
}

int Event::get_socket_fd() {
    return _sender->get_fd();
}

void Event::set_event_status(unsigned int status) {
    _epoll_event.events = status;
}

unsigned int Event::get_event_status() {
    return _epoll_event.events;
}

void Event::set_is_content(bool status) {
    _is_content = status;
}

bool Event::get_is_content() {
    return _is_content;
}

void Event::set_uuid(const std::string & uuid) {
    _uuid = uuid;
}
// 获取自己的uuid
std::string Event::get_uuid() {
    return _uuid;
}

std::shared_ptr<Message_Stream> Event::get_temp_message() {
    return _temp_message;
}

void Event::execute() {
    lock.lock();
    _function();
    lock.unlock();
}

std::shared_ptr<Event> Event::get_self() {
    return shared_from_this();
}

std::shared_ptr<Server_Message_Sender> Event::get_sender() {
    return _sender;
}

epoll_event* Event::get_epoll_event() {
    return &_epoll_event;
}

std::shared_ptr<Event_Buf> Event::get_message_buf() {
    return _buf;
}