#pragma once
#include "stdinc.h"
#include "Epoll_Reactor_Module/Epoll_Reactor.h"

class Group_Control {
public:
    Group_Control();
    /*
     * 设置自己服务的那个epoll
     */
    bool set_epoll_reactor(std::shared_ptr<Epoll_Reactor>);
    /*
     * 添加一个用户
     * 用户加入了群组
     */
    bool add(std::shared_ptr<Event>);
    /*
     * 删除一个用户
     * 用户离开了群组，或用户与服务器断开了连接
     */
    bool erase(std::shared_ptr<Event>);
    /*
     * 一个用户说了句话,其他人收到了话
     */
    void speak(std::shared_ptr<Event>, std::shared_ptr<Message_Stream>);

    /*
     * 设置最大的人数
     */
    void set_max_user(unsigned int);
    /*
    * 获取最大的人数
    */
    unsigned int get_max_user();

private:
    // 当前在群组中的人，通过uuid来查找
    std::map<std::string, std::shared_ptr<Event>> _user_store;
    // 最大允许的用户的数量
    std::atomic<unsigned int> _max_user;
    // 群组锁，当调用上面的函数的时候就锁往
    std::mutex _group_lock;
    std::shared_ptr<Epoll_Reactor> _epoll_reactor;
};
