#pragma once
#include "stdinc.h"
#include "Message.h"
#include "User_Manager.h"

class MessageReceiver;

class Business {
public:
    /*
     * 初始化业务系统
     */
    static void startInit();

    /*=====网络相关====*/
    /*
     * 重设目标ip地址和端口, 若输入的ip和地址无效， 则重新输入
     */
    static void reset_target_ip_port();
    static void reset_target_ip_port(std::string &, int);
    /*
     * 重新连接目标服务器
     */
    static void re_connect_server();
    /*
     * 查看当前的连接状态
     */
    static void cat_connect_status();
    /*
     * 测试当前连接到服务器的延迟
     */
    static void Testing_network_latency();

    /*====软件维护相关======*/
    /*
     * 从服务器监听
     */
    static void start_accept_from_server();
    /*
     * 保存设置
     */
    static void save();
    /*
     * cat 命令主框架
     */
    static void cat_func(std::string &);
    /*
     * 查看自己的uuid
     */
    static void cat_uuid();
    /*
     * 查看当前记录的用户
     */
    static void cat_user();

            /*====通讯相关========*/
    /*
     * 向服务器发送recall消息
     */
    static void recall_request(std::string&);
    /*
     * 向指定的人(昵称, uuid)发送消息
     */
    static void send_message(std::string&, std::string&);

    /*=======用户相关===========*/
    /*
     * 重命名
     */
    static void rename(std::string&, std::string&);
    /*
     * 添加用户
     */
    static void add_user(const std::string&);

    /* ==================
     * 以下是用户不可操控的， 由软件自动运行
     *
     * =================
     */
    /*
     * 初始化服务器， 在每次连接成功的时候运行
     */
    static void init_server();
    /*
     * 回应recall消息
     */
    static void return_recall(const std::string&, std::shared_ptr<unsigned char>, int);
    /*
     * 显示recall消息
     */
    static void display_recall(const std::string&);
    /*
     * 显示用户发送的消息
     */
    static void display_user_message(const std::string&, const std::string&) ;
    /*
     * 自动添加用户
     */
    static void add_user();
    /*
     * 显示延迟
     */
    static void display_latency();

    // 是否是第一次运行
    static bool is_start;
    // 用于测试延时
    static time_t last_time;

    static Log* log;
    static Language* lang;
    static Setting* setting;
    static User_Manager* u_m;
    // 读取数据包
    static MessageReceiver mr;
    // 处理数据包
    static MessageAnalysis ma;
    static Server_Connector s_c;
    static MessageSender m_s;
};
