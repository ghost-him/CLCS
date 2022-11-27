#pragma once
#include "Log.h"
#include "User.h"
#include "stdinc.h"
#include "Language.h"
#include "User_Manager.h"
#include "Security.h"
#include "Setting.h"

class MessageHeader;
class MessageSender;
class MessageReceiver;
class MessageGenerator;
class MessageAnalysis;

// TODO 重构此页的代码， 使用 标准库 来重构


/*
 * 用于标识信息的头标记
 */

struct MessageHeader {
public:
    typedef char Level;
    static const char RECALL = '1';
    static const char TEXT_CHAT = '2';
    static const char TEXT_SYS = '3';
    static const char KEY_TEST = '4';

    typedef char Option;
    static const char NOMEAN = '1';
    static const char SEND = '2';
    static const char RECV = '3';
    static const char USER_LIST = '4';
    static const char ADD_USER = '5';
    static const char INIT_SYS = '6';
    static const char TIME = '7';
};

/*
 * 消息生成器
 * 用于生成特定格式的消息
 * 可以生成加密信息
 *
 */

class MessageGenerator {
public:
    MessageGenerator();

    void startInit();
    /*
     * 设置信息类型和参数
     */
    void set_level_options(MessageHeader::Level, MessageHeader::Option, MessageHeader::Option);
    /*
     * 设置参数
     */
    void set_level(MessageHeader::Level);
    /*
     * 获取当前的参数
     */
    MessageHeader::Level get_level();
    /*
     * 设置参数
     */
    void set_option(MessageHeader::Option, MessageHeader::Option);
    /*
     * 获取参数1，2
     */
    MessageHeader::Option get_para1() const;
    MessageHeader::Option get_para2() const;
    /*
     * 设置目标用户
     * 设置目标用户的公钥路径
     */
    void set_target_user_uuid(const std::string&);
    /*
     * 获取目标用户
     */
    std::string get_target_user_uuid();
    /*
     * 拷贝赋值函数
     */
    const MessageGenerator& operator=(const MessageGenerator&);
    /*
     * 快速添加指定的内容
     */
    bool set_content(const std::string&, bool = true);
    // MessageGenerator& operator=(const char *);
    /*
     * 获取头标记
     */
    std::string get_header();
    /*
     * 获取文件的密文
     */
    std::shared_ptr<unsigned char[]> get_content();
    /*
     * 获取密文的长度
     */
    int get_content_size();
    /*
     * 设置消息的长度
     */
    void set_content_len(int);

private:
    // 转好以后的长度
    std::shared_ptr<unsigned char[]> _buf;
    // 生成数据的长度
    int _len;

    // 类型
    MessageHeader::Level _level;
    // 参数1, 2
    MessageHeader::Option _para1;
    MessageHeader::Option _para2;
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

/*
 * 负责处理接受回来的数据，并执行相应的函数
 */
// TODO 测试recall接受函数
class MessageAnalysis {
public:
    /*
     * 构造初始化
     */
    MessageAnalysis();
    void startInit();
    explicit operator const char *();
    // MessageAnalysis& operator=(const std::string&);

    /*
     * 获取当前的类型
     */
    MessageHeader::Level get_level();
    /*
     * 获取参数1和参数2
     */
    MessageHeader::Option get_para1();
    MessageHeader::Option get_para2();
    /*
     * 获取uuid
     */
    const std::string& get_uuid1() const ;
    const std::string& get_uuid2() const ;
    /*
     * 获取当前的文件内容
     */
    std::shared_ptr<unsigned char[]> get_raw();
    const std::string& get_content();
    /*
     * 获取消息的长度
     */
    int get_message_len();
    /*
     * 读取命令,
     * 当可以触发一个函数的时候，返回false, 否则返回true以为while做循环
     */
    int get_command(MessageReceiver *mr);
    /*
     * 检查头消息是否正确
     */
    bool check_header(const char *);

private:
    RSA_decrypt dec;
    // 当前的消息类型
    MessageHeader::Level _level;
    // 参数1
    MessageHeader::Option _para1;
    // 参数2
    MessageHeader::Option _para2;
    // 目标uuid
    std::string _uuid1;
    // 自己的uuid
    std::string _uuid2;
    // 消息长度
    int _message_size;
    // 消息内容
    std::shared_ptr<unsigned char[]> _raw;
    // 消息解密以后的内容
    std::string _content;
    // 用户管理器
    std::shared_ptr<User_Manager> u_m;
};


/*
 * 负责将数据发送给服务器
 * 设计模式：单例设计模式
 */

class MessageSender {
public:
    /*
     * 返回唯一的实例
     */
    static std::shared_ptr<MessageSender> ptr();

    /*
     * 初始化
     */
    void startInit();
    /*
     * 设置要发送的文件描述符
     */
    void set_fd(int);
    // 发送一个字符串
    void send_message(const std::string&);
    void send_message(std::shared_ptr<unsigned char[]>, int);

private:
    static std::shared_ptr<MessageSender> _ptr;
    MessageSender();
    // 目标服务器的文件描述符
    int _socket_fd;
    std::shared_ptr<Log> log ;
    std::shared_ptr<Language> lang;
    // 加解锁
    std::mutex _sender;
};

/*
 * 负责从服务器或客户端接受数据
 * 设计模式：单例设计模式
 * TODO 设计多线程安全
 */

class MessageReceiver {
public:
    MessageReceiver();
    void startInit();
    /*
     * 获取头文件
     */
    const std::shared_ptr<char[]>& get_header();
    /*
     * 获取当前的内容
     */
    std::shared_ptr<unsigned char[]> get_content();
    /*
     * 读取一行
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
    void set_read_content(int len);
    /*
     * 设置当前的fd
     */
    void set_fd(int);
    /*
     * 获取当前的fd
     */
    int get_fd() const;

private:
    // 读到的头文件
    std::shared_ptr<char[]> _header;
    // 头文件的长度
    int _header_len;

    // 受到的消息
    std::shared_ptr<unsigned char[]> _buf;
    // 处理受到的消息
    MessageAnalysis _analysis;
    // 目标服务器
    int _socket_fd;
    // 内容的长度
    int _content_len;
    // 是否读内容
    bool _is_content;
    // 是否重新开辟空间
    bool _is_reset;
    // 日志系统
    std::shared_ptr<Log> log;
    // 语言系统
    std::shared_ptr<Language> lang;

};

/*
 * 负责处理即将发送的数据
 */

