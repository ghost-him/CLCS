#pragma once
#include "stdinc.h"
#include "Log.h"
#include "Setting.h"
#include "Language.h"

#include "Message.h"
#include "Event.h"
#include "Epoll_Reactor_Service.h"
#include "Thread_Pool.h"




/*
 * epoll_reactor 类
 * 是核心部件， 用于控制epoll_reactor
 * 非阻塞的
 */

class Epoll_Reactor_Service;

class Epoll_Reactor {
    friend class Epoll_Reactor_Service;
public :
    /*
     * 获取指向自己的指针
     */
    static std::shared_ptr<Epoll_Reactor> ptr();

    /*
     * 设置ip和端口
     */
    void set_ip_port(const std::string&, int);

    /*
     * 为epoll reactor初始化
     */
    void startInit();
    /*
     * 向存储器中添加事件
     */
    void store_add_event(std::shared_ptr<Event>);
    /*
     * 从存储器中删除事件
     */
    void store_remove_event(std::shared_ptr<Event>);

    /*
     * 向监听树中添加事件
     */
    void epoll_add_event(std::shared_ptr<Event>);
    /*
     * 从监听树中删除事件
     */
    void epoll_erase_event(std::shared_ptr<Event>);
    /*
     * 刷新事件
     */
    void epoll_flush_event(std::shared_ptr<Event>);

    /*
     * 与指定的断开连接
     */
    void disconnect(std::shared_ptr<Event>);

    /*
     * 开始监听事件
     */
    void start_listen();


private:

    Epoll_Reactor();
    // 单例模式
    static std::shared_ptr<Epoll_Reactor> _ptr;
    // 事件集合
    std::set<std::shared_ptr<Event>> _event_store;
    // 自己的服务类
    std::shared_ptr<Epoll_Reactor_Service> _service;

    // 存储用户的信息
    std::shared_ptr<User_Manager> u_m;
    // 通过uuid 来查找指定的事件在监听树中的位置
    std::map<std::string, std::shared_ptr<Event>> _user_store;

    /*
     * epoll reactor自己的属性
     */
    // 根
    int _root;
    // 最大的事件的连接数
    int MAX_EVENT;
    // 运行时产生的数组
    std::shared_ptr<epoll_event[]> _run_events;
    // epoll reactor 的锁， 在操作添加，减少等操作时，会将数据结构锁住
    std::mutex _data_structure_lock;

    /*
     * tcp连接属性
     */

    // 服务器的连接属性
    sockaddr_in _server_addr;
    // 服务器的文件描述符
    int _server_socket_fd;
    // 服务器的端口
    int _port;
    // 服务器的ip地址
    std::string _ip;
    // 服务器的可最大的连接数
    int _max_connect;

    /*
     * 其他辅助类
     */

    std::shared_ptr<Language> lang;
    std::shared_ptr<Log> log;
    std::shared_ptr<Setting> setting;

    /*
     * 辅助函数
     */

    // 当有新地连接进来的时候，将这个连接的用户添加到记录中
    bool  add_user(std::shared_ptr<Event>, std::shared_ptr<Message_Stream>);

};

