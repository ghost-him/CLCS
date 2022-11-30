#pragma once
#include "stdinc.h"
#include "Message.h"

class Message_Header_Analysis {
public :
    void set_header(const std::shared_ptr<char[]>&);

    /*
     * 开始解析内容
     */
    bool convert();

    /*
     * 获取类型
     */
    Message_Header::Level get_level();
    /*
     * 获取参数1和参数2
     */
    Message_Header::Option get_para1();
    Message_Header::Option get_para2();
    /*
     * 获取uuid
     */
    const std::string& get_target_uuid() const ;
    const std::string& get_source_uuid() const ;
    /*
     * 获取消息的长度
     */
    int get_content_len();

private:
    std::shared_ptr<char[]> _header = nullptr;
    // 当前的消息类型
    Message_Header::Level _level = 0;
    // 参数1
    Message_Header::Option _para1 = 0;
    // 参数2
    Message_Header::Option _para2 = 0;
    // 目标uuid
    std::string _uuid1;
    // 自己的uuid
    std::string _uuid2;
    // 消息长度
    int _message_size = 0;
};