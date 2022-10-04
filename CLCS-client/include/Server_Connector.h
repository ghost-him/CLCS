#pragma once
#include "stdinc.h"
#include "Log.h"
#include "Setting.h"
#include "Language.h"

/*
 * 用于与服务器做连接
 * 设计模式：单例模式
 * 一个程序只需要有一个连接器来与服务器做连接即可
 *
 * 作用：返回连接好以后的文件描述符
 */

// TODO 完善当前的类
// 1. 连接功能
// 2. 查看ip地址和端口
// 3. 文件描述符设置为非阻塞的模式
// 4. 将目标服务器的地址存到储存文件中

class Server_Connector {
public:
    static Server_Connector* ptr;
    /*
     * 初始化连接器
     */
    void startInit();
    /*
     * 设置目标服务器的ip地址和端口
     * 注意检查返回值！
     */

    bool set_target(const char*, int);

    /*
     * 连接目标服务器
     */
    bool start_connect();
    void close_connect();


    // 获取当前服务器的文件描述符
    int get_socketfd();
    /*
     * 获取当前的连接状态
     */
    bool get_connect_status();

    void wait_for_connection();
private:
    Server_Connector();
    // 目标连接的ip和端口
    std::string _ip;
    int _port;

    // 当前的连接的状态
    bool _is_connected;

    // 当前的文件描述符
    int _socket_fd = 0;

    // 储存文件的结构体
    sockaddr_in _serv_addr;

    // 日志系统
    Log* log;
    // 设置系统
    Setting* setting;
    // 语言系统
    Language* lang;

};