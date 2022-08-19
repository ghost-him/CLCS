#pragma once
#include "stdinc.h"
#include "Log.h"
#include "Setting.h"
#include "Language.h"
#include "Message.h"

// TODO 重构此部分的代码
// TODO 重写recvdata和senddata函数，格式类似于threadPool的
class Event {
public:
    std::list<Event>::iterator _index;          // 当前的点在维护数组中的位置
    int _socket_fd = 0;       // 当前的操作符
    int _events = 0;         // 当前事件的状态
    std::function<void()> _func;
    time_t _last_active = 0;    // 上一次运行的事件

    char header[BUFSIZ];
    int header_len = 0;

    char level;
    char option1;
    char option2;
    std::string source_uuid;
    std::string target_uuid;
    int content_len = 0;            // 缓冲区的大小

    bool is_content = false;
    std::shared_ptr<unsigned char> _buf; // 消息内容缓冲区
};
/*
 * epoll 反应堆
 * 设计模式：单例模式
 */

class Epoll_Reactor {
public:
    static Epoll_Reactor* getInstance();

    /*
     * 初始化反应堆
     */
    void Init_Epoll();

    /*
     * 设置当前的ip
     * 若不设置，则使用默认ip
     */
    void set_ip(std::string&);

    /*
     * 获取当前的ip
     */

    std::string get_ip();

    /*
     * 设置端口
     */
    void set_port(int);

    /*
     * 获取当前的端口
     */
    int get_port() const;
    /*
     * 检查文件的读取情况
     */
    static bool check(int);
    /*
     * 作用：设置一个事件的回调函数
     * sockfd 当前事件的描述符， func: 函数的名字， args: 参数
     */
    template<class Function, class ... Args>
    static void set_event(std::list<Event>::iterator& iter, int sockfd, Function &&func, Args &&... args) {
        (*iter)._socket_fd = sockfd;
        (*iter)._events = 0;
        (*iter)._func = std::bind(std::forward<Function>(func), std::forward<Args>(args)...);
        if ((*iter).content_len <= 0) {
            (*iter).content_len = 0;
        }

        (*iter)._last_active = time(nullptr);
    }

    /*
     * 将一个节点添加到监听树中
     * 参数:监听的参数， 被监听的事件
     */

    static void add_event(int , std::list<Event>::iterator&);

    /*
     * 将一个节点从监听树中删除
     */

    static void del_event(Event&);

    /*
     * 从文件描述符中读取内容
     */
    static void recvdata(std::list<Event>::iterator*);

    /*
     * 向文件描述符发送文件
     */
    // static void senddata(int, std::list<Event>::iterator*);
    /*
     * 开始监听
     */
    static void start_listen();
    /*
     * 处理读到的命令
     * 待处理的event
     */
    static void analysis(std::list<Event>::iterator*);
    /*
     * 返回recall函数
     */
    static void send_self_recall(std::list<Event>::iterator*);

    /*
     * 发送消息
     * fd, buf, 长度
     * 成功：true
     * 失败：false
     */
    static bool send_message(int, unsigned char*, int);
    /*
     * 检查当前的头消息是否符合规范
     */
    static bool check_header(const char *);
    /*
     * 添加当前event用户
     */
    static bool add_user(std::list<Event>::iterator*);
    /*
     * 用于初始化当前的客户端
     */
    static bool init_client(std::list<Event>::iterator*);
    /*
     * 返回用户的公钥
     */
    static bool return_user_message(std::list<Event>::iterator*);
    /*
     * 转发消息
     */
    static bool forwarding_message(std::list<Event>::iterator*);

    static MessageGenerator mg;
private:
    static Epoll_Reactor* _pEpoll_Reactor;
    Epoll_Reactor();
    ~Epoll_Reactor();

    /*
     * 守护函数， 用于连接
     */
    static void acception_connection();

    static int MAX_EVENTS;          // 最大的事件数

    static int _epoll_root;        // 反应堆的根节点
    static std::list<Event> _events;         // 数组
    static epoll_event* _run_events;         // 运行时产生的数组
    sockaddr_in _server_addr;
    static int _serv_fd;    // 当前服务器的文件描述符
    int _port;              // 端口
    std::string _ip;        // ip地址
    int _max_connect;       // 最大的连接数
    // 获取其他模块的接口
    static Log* log;
    Setting* setting;
    static Language* lang;

    // 通过uuid来查找目标在当前监听树中的位置
    static std::map<std::string, std::list<Event>::iterator*> _user_store;

    // 用户管理器
    static User_Manager* u_m;
};

