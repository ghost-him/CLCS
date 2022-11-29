#pragma once
#include "stdinc.h"
#include "Event_Buf.h"
#include "MessageSender.h"
#include "Epoll_Reactor_Service.h"


/*
 * event 类
 * 用于描述一个事件，一个连接控制类
 */

class Event : public std::enable_shared_from_this<Event> {
public :
    Event();

    // 设置文件描述符
    void set_socket_fd(int);
    // 获取文件描述符
    int get_socket_fd();

    // 设置当前的事件的状态
    void set_event_status(unsigned int);
    // 获取当前事件的状态
    unsigned int get_event_status();

    // 设置当前读取的状态
    void set_is_content(bool);
    bool get_is_content();

    std::shared_ptr<Message_Stream> get_temp_message();

    /*
     * 作用：设置一个事件的回调函数
     * func: 函数的名字， args: 参数
     */
    template<class Function, class ... Args>
    void set_function(Function &&func, Args &&... args) {
        _function = std::move(std::bind(std::forward<Function>(func), std::forward<Args>(args)...));
    }
    // 执行回调函数
    void execute();

    // 设置当前元素在存储器中的位置
    void set_location(std::list<std::shared_ptr<Event>>::iterator&&);
    // 获取当前元素在存储器中的位置
    std::list<std::shared_ptr<Event>>::iterator get_location();

    // 获取自己的指针
    std::shared_ptr<Event> get_self();
    // 获取自己的发送器
    std::shared_ptr<Server_Message_Sender> get_sender();
    // 获取epoll_event
    epoll_event* get_epoll_event();
    // 获取数据缓冲区
    std::shared_ptr<Event_Buf> get_message_buf();
    // 锁, 防止在读的时候导致在监听树中的状态被更改
    std::mutex lock;
private :
    // 数据缓冲区
    std::shared_ptr<Event_Buf> _buf;
    // 属性
    epoll_event _epoll_event;
    // 自己的文件发送器
    std::shared_ptr<Server_Message_Sender> _sender;
    // 自己的回调函数
    std::function<void()> _function;
    // 自己在储存器中的位置
    std::list<std::shared_ptr<Event>>::iterator _location;
    // 是否是数据
    bool _is_content;

    // 临时读取区
    std::shared_ptr<Message_Stream> _temp_message;

};
