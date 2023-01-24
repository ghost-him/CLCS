#pragma once
#include "Message.h"
#include "stdinc.h"
#include "Message_Stream.h"

/*
 * 消息生成器
 * 用于生成特定格式的消息
 * 可以生成加密信息
 */

class Message_Generator {
public:
    Message_Generator();

    void startInit();
    /*
     * 设置信息类型和参数
     */
    void set_level_options(Message_Header::Level, Message_Header::Option, Message_Header::Option);
    /*
     * 设置目标用户
     * 设置目标用户的公钥路径
     */
    void set_target_user_uuid(const std::string&);
    /*
     * 拷贝赋值函数
     */
    const Message_Generator& operator=(const Message_Generator&);
    /*
     * 快速添加指定的内容
     */
    bool set_content(const std::string&, bool = true);

    /*
     * 获取信息流
     */
    std::shared_ptr<Message_Stream> get_message();

private:
    /*
     * 设置消息的长度
     */
    void set_content_len(int);
    /*
     * 获取头标记
     */
    std::string get_header();
    /*
     * 生成的消息流
     */
    std::shared_ptr<Message_Stream> _stream;

    // 生成数据的长度
    int _len;
    // 类型
    Message_Header::Level _level;
    // 参数1, 2
    Message_Header::Option _para1;
    Message_Header::Option _para2;
    // 目标的用户
    std::string _target_user_uuid;
    // 发送的数据的内容
    std::string _content;

    std::shared_ptr<Log> log = nullptr;

    std::shared_ptr<Language> lang = nullptr;

    std::shared_ptr<Setting> setting = nullptr;

    std::shared_ptr<User_Manager> u_m = nullptr;

    RSA_encrypt enc;

};