#pragma once
#include "stdinc.h"
#include "Epoll_Reactor.h"
#include "Message_Module/Message_Header_Analysis.h"
#include "Message_Module/Message_Generator.h"

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
    static void set_epoll_reactor(std::shared_ptr<Epoll_Reactor>);

    /*
     * 接受连接
     */
    static void accept_connection();

    /*
     * 读取从客户端传来的数据
     */
    static void receive_message(std::shared_ptr<Event>);

    /*
     * 根据不同的参数来执行不同的函数
     */
    static void analysis(std::shared_ptr<Event>);

    /*
     * 向客户端发送消息
     */
    static void send_message(std::shared_ptr<Event>);

    /*
     * 加入群组
     */
    static void add_group(std::shared_ptr<Event>, std::shared_ptr<Message_Stream>);
    /*
     * 离开群组
     */
    static void leave_group(std::shared_ptr<Event>, std::shared_ptr<Message_Stream>);

private:
    /*
     * 将文件描述符设置为非阻塞
     */
    static bool set_nonblock(int fd);

    /*
     * 当前的消息是回调消息
     * 客户端发送了回调消息
     */
    static void send_recall_message(std::shared_ptr<Event>, std::shared_ptr<Message_Stream>);

    /*
     * 当前的消息是初始化消息
     * 客户端刚刚连接上了服务器
     */
    static void init_the_client(std::shared_ptr<Event>, std::shared_ptr<Message_Stream>);

    /*
     * 当前的消息是用户请求用户的公钥
     * 客户端要添加好友
     */
    static void send_user_key( std::shared_ptr<Event> , std::shared_ptr<Message_Stream>);

    /*
     * 直接返回消息
     * 当前的用户在测试延时
     */
    static void send_time_message(std::shared_ptr<Event>, std::shared_ptr<Message_Stream>);

    /*
     * 转发消息
     * 客户向其他的客户发送了一个消息
     */
    static bool redirect_the_message(std::shared_ptr<Event>, std::shared_ptr<Message_Stream> );

    // 自己服务的epoll reactor
    static std::shared_ptr<Epoll_Reactor> _epoll_reactor;
    // 消息生成器
    static Message_Generator mg;
};