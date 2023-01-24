/*
 * 用于处理客户端自己的信息
 */

#pragma once
#include "stdinc.h"
#include "Message_Module/Message.h"
#include "User_Manager_Module/User_Manager.h"
#include "Message_Process_Module/Message_Process.h"
#include "Request_Management_Module/Request_Management.h"

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
    static void reset_target_ip_port(const std::string &, int);
    /*
     * 重新连接目标服务器
     */
    static void re_connect_server();
    /*
     * 查看当前的连接状态
     */
    static void cat_connect_status();
    /*
     * 查看自己是否在群组中
     */
    static void cat_group();
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
    static void recall_request(const std::string&);
    /*
     * 向指定的人(昵称, uuid)发送消息
     */
    static void send_user_message(const std::string&, const std::string&);

    /*
     * 向群组中说话
     */
    static void send_group_message(const std::string&);

    /*=======用户相关===========*/
    /*
     * 重命名
     */
    static void rename(const std::string&, const std::string&);
    /*
     * 添加用户
     */
    static void add_user(const std::string&);

    /*=========群组相关=============*/

    /*
     * 客户端发送
     * 请求加入群组
     */
    static void join_group();
    /*
     * 客户端发送
     * 请求离开群组
     */
    static void leave_group();


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
     * 显示recall消息
     */
    static void display_recall(const std::string&);
    /*
     * 显示用户发送的消息
     */
    static void display_user_message(const std::string&, const std::string&) ;
    /*
     * 显示群组中的消息
     */
    static void display_group_message(const std::string &);

    /*
     * 自动添加用户
     */
    static void add_user();
    /*
     * 显示延迟
     */
    static void display_latency();

    /*
     * 连接服务器
     */
    static bool connect_server(const std::string& ip, int port);

    /*
     * 收到了一个请求
     * 参数：收到的请求的uuid, 答复情况
     */
    static void accept_response(const std::string &,  Message_Header::Option);

    /*
     * 对应join_group函数
     * 服务器返回的请求同意
     */
    static void response_accept_group();
    /*
     * 对应join_group函数
     * 服务器返回的请求失败
     */
    static void response_refuse_group();


    // 是否是第一次运行
    static bool is_start;

    static std::shared_ptr<Log> log;
    static std::shared_ptr<Language> lang;
    static std::shared_ptr<Setting> setting;
    static std::shared_ptr<User_Manager> u_m;

    static Message_Receiver mr;
    static Message_Analysis ma;

    static std::shared_ptr<Message_Process> m_process;

    static std::shared_ptr<Server_Connector> s_c;

    static bool _is_closed;
};
