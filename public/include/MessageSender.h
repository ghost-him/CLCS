#pragma once
#include "stdinc.h"
#include "Message.h"
/*
 * 负责将数据发送给服务器
 */

class MessageSender {
public:
    /*
     * 初始化
     */
    virtual void startInit();
    /*
     * 设置要发送的文件描述符
     */
    void set_fd(int);
    /*
     * 获取当前的文件描述符
     */
    int get_fd();
    // 发送一个字符串
    virtual int send_message(const std::string&);
    virtual int send_message(std::shared_ptr<unsigned char[]>, int);

protected:
    MessageSender();
    // 目标服务器的文件描述符
    int _socket_fd;
    std::shared_ptr<Log> log ;
    std::shared_ptr<Language> lang;
    // 加解锁
    std::mutex _sender;
};

class Client_Message_Sender : public MessageSender{
public:
    /*
    * 返回唯一的实例
    */
    static std::shared_ptr<Client_Message_Sender> ptr();

private:
    static std::shared_ptr<Client_Message_Sender> _ptr;
    Client_Message_Sender(){}
};

class Server_Message_Sender : public MessageSender {
public:
    Server_Message_Sender(){}
    int send_message(const std::string&) override;
    int send_message(std::shared_ptr<unsigned char[]>, int) override;

};