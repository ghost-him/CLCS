#pragma once
#include "stdinc.h"
#include "Message_Header_Checker.h"

class Message_Header_Analysis {
public :
    void set_header(std::shared_ptr<char[]>);

    /*
     * 开始解析内容
     */
    bool convert();

    /*
     * 获取类型
     */
    char get_level() const;
    /*
     * 获取参数1和参数2
     */
    char get_para1() const;
    char get_para2() const;
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
    char _level = 0;
    // 参数1
    char _para1 = 0;
    // 参数2
    char _para2 = 0;
    // 目标uuid
    std::string _uuid1;
    // 自己的uuid
    std::string _uuid2;
    // 消息长度
    int _message_size = 0;
};