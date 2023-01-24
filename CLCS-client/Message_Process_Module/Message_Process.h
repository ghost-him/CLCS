/*
 * 用于处理客户端和服务器之间的通讯的过程
 * 用于处理生成信息， 处理信息，然后发送信息
 *
 * 封装了MessageGenerator, Message_Sender, Server_connector三个系统
 */

#pragma once
#include "../../public/Message_Module/Message.h"
#include "Server_Connector_Module/Server_Connector.h"
#include "Message_Module/Message_Sender.h"
#include "Message_Module/Message_Generator.h"

class Message_Process {
public:
    static std::shared_ptr<Message_Process> ptr();
    void startInit();
    /*
     * 测试延迟
     */
    void Test_Latency();
    /*
     * 向服务器发送回调信息
     */
    void recall_mes(const std::string &, const std::string &);
    /*
     * 向指定的用户发送信息
     * 指定的用户，要发送的消息
     */
    void send_user_message(const std::string &, const std::string &);
    /*
     * 向群组中发送消息
     * 要发送的消息
     */
    void send_group_message(const std::string &);
    /*
     * 在服务器中添加自己的信息
     */
    void init_the_server();
    /*
     * 请求指定uuid的公钥
     */
    void add_user(const std::string&);
    /*
     * 更新连接
     */
    void update_connection();
    /*
     * 加入群组
     */
    void join_group(const std::string&);
    /*
     * 离开群组
     */
    void leave_group();

private:
    Message_Process(){}
    static std::shared_ptr<Message_Process> _ptr;

    void set_and_send(Message_Header::Level level, Message_Header::Option para1, Message_Header::Option para2,
                      const std::string& uuid,
                      const std::string& message,
                      bool is_encrypt);

    Message_Generator _mg;
    std::shared_ptr<Client_Message_Sender> _ms;
    std::shared_ptr<Server_Connector> _sc;
    std::shared_ptr<Log> _logger;
    std::shared_ptr<User_Manager> _um;

    pthread_mutex_t _is_send_message = PTHREAD_MUTEX_INITIALIZER;
};