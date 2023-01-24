#pragma once
#include "Logger_Module/Log.h"
#include "User_Manager_Module/User.h"
#include "stdinc.h"
#include "Language_Module/Language.h"
#include "User_Manager_Module/User_Manager.h"
#include "Security_Module/Security.h"
#include "Initializer_Module/Setting.h"
#include "Message_Stream.h"
/*
 * 用于标识信息的头标记
 */

class Message_Stream;

struct Message_Header {
public:
    typedef char Level;
    static const char RECALL = '1';
    static const char TEXT_CHAT = '2';
    static const char TEXT_SYS = '3';
    static const char REQUEST = '4';

    typedef char Option;
    static const char NOMEAN = '0';
    static const char USER = '5';
    static const char GROUP = '6';
    static const char ADD_USER = '7';
    static const char INIT_SYS = '8';
    static const char TIME = '9';
    static const char ACCESS = 'a';
    static const char REJECT = 'b';
    static const char JOIN = 'c';
    static const char LEAVE = 'd';
};




/*
 * 负责处理消息的解密的任务
 */
class Message_Analysis {
public:
    /*
     * 构造初始化
     */
    Message_Analysis();
    void startInit();

    /*
     * 设置要解密的消息
     */

    void set_message(std::shared_ptr<Message_Stream>);
    /*
     * 获取解密以后的消息
     */
    const std::string& get_content();
    /*
     * 获取原始消息
     */
    std::shared_ptr<unsigned char[]> get_raw();
    /*
     * 获取原始消息的长度
     */
    size_t get_raw_len();


private:
    RSA_decrypt dec;
    // 当前要解析的消息
    std::shared_ptr<Message_Stream> _stream;
    // 消息解密以后的内容
    std::string _content;
    // 用户管理器
    std::shared_ptr<User_Manager> u_m;
};



/*
 * 负责读取数据，解析并判断数据的正解性
 */

class Message_Receiver {
public:
    Message_Receiver();
    void startInit();
    /*
     * 读取一个命令
     */
    int read_command();

    /*
     * 设置当前的fd
     */
    void set_fd(int);
    /*
     * 获取当前的fd
     */
    int get_fd() const;
    /*
     * 获取当前读到的消息
     */
    std::shared_ptr<Message_Stream> get_message();



private:
    /*
     * 读取头标记
     */
    int read_header();
    /*
     * 读取文件的内容
     * 0: 关闭了连接
     * >0: 正常的连接
     * <0: 异常
     */
    int read_content();
    /*
     * 设置读文件, 并且设置读的长度
     */
    void set_read_content_len(int len);

    // 读到的消息
    std::shared_ptr<Message_Stream> _stream;

    // 处理受到的消息
    Message_Analysis _analysis;
    // 目标服务器
    int _socket_fd;

    // 是否读内容
    bool _is_content;

    // 日志系统
    std::shared_ptr<Log> log;
    // 语言系统
    std::shared_ptr<Language> lang;

};