/*
 * 用于处理客户端和服务器之间的通讯的过程
 * 用于处理生成信息， 处理信息，然后发送信息
 *
 * 封装了MessageGenerator, MessageSender, Server_connector三个系统
 */

#pragma once
#include "Message.h"
#include "Server_Connector.h"

class Message_Process {
public:
    static Message_Process* ptr;
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
     */
    void send_user_message(const std::string &, const std::string &);
    /*
     * 在服务器中添加自己的信息
     */
    void init_the_server();
    /*
     * 请求指定uuid的公钥
     */
    void add_user(const std::string&);

    void update_connection();
private:
    void set_and_send(MessageHeader::Level level, MessageHeader::Option para1, MessageHeader::Option para2,
                      const std::string& uuid,
                      const std::string& message,
                      bool is_encrypt);

    MessageGenerator _mg;
    MessageSender _ms;
    Server_Connector* _sc;
    Log* _logger;
    User_Manager* _um;

    pthread_mutex_t _is_send_message = PTHREAD_MUTEX_INITIALIZER;
};