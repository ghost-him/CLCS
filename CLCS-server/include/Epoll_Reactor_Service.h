#pragma once
#include "stdinc.h"
#include "Epoll_Reactor.h"
#include "Message_Header_Analysis.h"

/*
 * business类
 * 用于处理相应的事务
 */

class Epoll_Reactor;

class Epoll_Reactor_Service {
    friend class Epoll_Reactor;
public:
    /*
     * 设置自己的epoll reactor
     */
    void set_epoll_reactor(const std::shared_ptr<Epoll_Reactor>&);

    /*
     * 接受连接
     */
    void accept_connection();

    /*
     * 读取从客户端传来的数据
     */
    void receive_message(const std::shared_ptr<Event> &);

    /*
     * 根据不同的参数来执行不同的函数
     */
    void analysis(const std::shared_ptr<Event> &);

private:
    /*
     * 将文件描述符设置为非阻塞
     */
    bool set_nonblock(int fd);

    /*
     * 判断串是不是头标记
     */
    bool check_header(const std::shared_ptr<char[]> &);

    /*
     * 当前的消息是回调消息
     * 客户端发送了回调消息
     */
    void send_recall_message(const std::shared_ptr<Event> &, const std::shared_ptr<Message_Stream>&);

    /*
     * 当前的消息是初始化消息
     * 客户端刚刚连接上了服务器
     */
    void init_the_client(const std::shared_ptr<Event>&, const std::shared_ptr<Message_Stream> &);

    /*
     * 当前的消息是用户请求用户的公钥
     * 客户端要添加好友
     */
    void send_user_key(const std::shared_ptr<Event> &, const std::shared_ptr<Message_Stream>&);

    /*
     * 直接返回消息
     * 当前的用户在测试延时
     */
    void send_time_message(const std::shared_ptr<Event>&, const std::shared_ptr<Message_Stream> &);

    /*
     * 转发消息
     * 客户向其他的客户发送了一个消息
     */
    bool redirect_the_message(const std::shared_ptr<Event>&, const std::shared_ptr<Message_Stream> &);

    /*
     * 向客户端发送消息
     */
    void send_message(const std::shared_ptr<Event> &);

    // 自己服务的epoll reactor
    std::shared_ptr<Epoll_Reactor> _epoll_reactor;
    // 消息生成器
    MessageGenerator mg;
};