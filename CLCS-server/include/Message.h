#pragma once
#include "Log.h"
#include "Server_Connector.h"
#include "User.h"
#include "stdinc.h"
#include "Language.h"

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
    static const char RECALL = 1;
    static const char TEXT_CHAT = 2;
    static const char TEXT_SYS = 3;
    static const char KEY_TEST = 4;

    typedef char Option;
    static const char NOMEAN = 0;
    static const char SEND = 1;
    static const char RECV = 2;
    static const char USER_LIST = 3;
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
    MessageGenerator(MessageHeader::Level, MessageHeader::Option);

    void set_level(MessageHeader::Level);
    MessageHeader::Level get_level();

    void set_user(const User&);
    const User& get_user();

    explicit operator const char *();
    MessageGenerator& operator=(MessageGenerator&);
    MessageGenerator& operator=(const char *);

    char conv_buf[3];
    const char * convert_to_str(char t);

private:
    void addSpace();

    // 单词输出的最大长度
    char _buf[BUFSIZ];
    MessageHeader::Level _level;
    MessageHeader::Option _option;

    User _user;

    Language* lang = Language::getInstance();

};

/*
 * 负责处理接受回来的数据，并执行相应的函数
 */

class MessageAnalysis {
public:
    explicit operator const char *();
    MessageAnalysis& operator=(const std::string&);

    MessageHeader::Level get_level();
    std::string get_option();
    std::string get_content();

private:
    std::string _level;
    std::string _option;
    std::string _content;
};


/*
 * 负责将数据发送给服务器
 * 设计模式：单例设计模式
 * TODO 设计多线程安全
 */

class MessageSender {
public:
    void InitMessageSender();

    static MessageSender* getInstance();

    // 发送一个字符串
    void send_message(const std::string&);



private:
    MessageSender();

    static MessageSender* _pMessageSender;
    // 连接器对象， 用于更新socketfd
    Server_Connector * _pConnector;
    // 目标服务器的文件描述符
    int _socketfd;
    MessageGenerator _generator;
    Log* log;
    Language* lang;
};

/*
 * 负责从服务器或客户端接受数据
 * 设计模式：单例设计模式
 * TODO 设计多线程安全
 */

class MessageReceiver {
public:
    void InitMessageReceiver();

    static MessageReceiver* getInstance();
    /*
     * 从服务器接受一个服务器
     */
    void receive_message();

    std::string get_content();

private:
    MessageReceiver();

    static MessageReceiver* _pMessageReceiver;
    // 受到的消息
    std::string _message;
    // 处理受到的消息
    MessageAnalysis _analysis;
    // 连接器对象， 用于更新socketfd
    Server_Connector * _pConnector;
    // 目标服务器
    int _socketfd;
    // 日志系统
    Log* log;

    Language* lang = Language::getInstance();
};

/*
 * 负责处理即将发送的数据
 */

