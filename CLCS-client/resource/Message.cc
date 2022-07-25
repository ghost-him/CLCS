#include "Message.h"

MessageSender* MessageSender::_pMessageSender = new MessageSender;
MessageReceiver* MessageReceiver::_pMessageReceiver = new MessageReceiver;


MessageGenerator::MessageGenerator()
    :_level(MessageHeader::RECALL), _option(MessageHeader::NOMEAN){
}

MessageGenerator::MessageGenerator(MessageHeader::Level level, MessageHeader::Option opt)
    :_level(level), _option(opt) {
}

void MessageGenerator::set_level(MessageHeader::Level level) {
    _level = level;
}

MessageHeader::Level MessageGenerator::get_level() {
    return _level;
}

void MessageGenerator::set_user(const User& user) {
    _user = user;
}
const User& MessageGenerator::get_user() {
    return _user;
}

MessageGenerator::operator const char *() {
    return _buf;
}

MessageGenerator & MessageGenerator::operator=(MessageGenerator& str) {
    strcpy(_buf, str._buf);
    return *this;
}

MessageGenerator& MessageGenerator::operator = (const char * str) {
	// TODO 完成格式生成器
    // 当前消息的文件等级
    const char * text_level = convert_to_str(_level);
    const char * text_option;
    switch (_level) {
        case MessageHeader::RECALL: {
            text_option = convert_to_str(_option);
            break;
        }
        case MessageHeader::TEXT_CHAT: {
            text_option = _user.get_uuid();
            break;
        }
            // TODO 根据实际的业务需求完成当前的逻辑
        case MessageHeader::TEXT_SYS: {
            break;
        }
        case MessageHeader::KEY_TEST: {
            break;
        }
        default:
            Log::getInstance()->log((*lang)["MessageGenerator_invalid_level"] + str);
            break;
    }
    strcpy(_buf, text_level);
    strcat(_buf, text_option);
    strcat(_buf, str);
    return *this;

}

const char * MessageGenerator::convert_to_str(char t) {
    conv_buf[0] = t;
    conv_buf[1] = ' ';
    conv_buf[2] = '\0';
    return conv_buf;
}



void MessageGenerator::addSpace() {
    int len = strlen(_buf);
    if (len >= BUFSIZ)
        return;
    _buf[len] = ' ';
    _buf[len + 1] = '\0';
}

MessageAnalysis::operator const char *() {
    return _content.c_str();
}

MessageAnalysis &MessageAnalysis::operator=(const std::string &str) {
    MessageHeader::Level level;     // 获取选项
    std::string option;
    std::string content;

    std::stringstream ss(str);

    ss >> level;
    ss >> option;
    ss >> content;
    return *this;
}

MessageHeader::Level MessageAnalysis::get_level() {
    return _level[0];
}
std::string MessageAnalysis::get_option() {
    return _option;
}
std::string MessageAnalysis::get_content() {
    return _content;
}

void MessageSender::InitMessageSender() {
    _pConnector = Server_Connector::getInstance();
    _socketfd = 0;
    log = Log::getInstance();
    lang = Language::getInstance();
    _socketfd = _pConnector->get_socketfd();
}


MessageSender *MessageSender::getInstance() {
    return _pMessageSender;
}

void MessageSender::send_message(const std::string & str) {
    // TODO 测试该函数
    // 只负责规范化以及发送消息
    // 不负责处理每次发送多少消息， 最多接受 8196 / 100 * 100 = 8100 个字符的消息
    if (str.size() > BUFSIZ / 100 * 100) {
        log->log((*lang)["MessageSender_message_too_long"]);
    }

    // 规范化字符串，然后输出该字符串
    char message_to_send[BUFSIZ];
    strcpy(message_to_send, (const char *)(_generator = str.c_str()));
    int ret = write(_socketfd, message_to_send, strlen(message_to_send));
    if (ret == -1) {
        log->log((*lang)["MessageSender_send_error"]);
    }
}

// TODO 初始化成员变量
MessageSender::MessageSender() {

}

void MessageReceiver::InitMessageReceiver() {
    _pConnector = Server_Connector::getInstance();
    _socketfd = 0;
    log = Log::getInstance();
    _socketfd = _pConnector->get_socketfd();
}


MessageReceiver *MessageReceiver::getInstance() {
    return _pMessageReceiver;
}

// TODO 优化这个类
void MessageReceiver::receive_message() {
    char buf[BUFSIZ];
    int ret;
    while (1) {
        ret = read(_socketfd, buf, BUFSIZ);
        if (ret == -1)
            break;
        else if(ret == 0) {
            _pConnector->close_connect();
            log->log((*lang)["MessageReceiver_close_connect"]);
            break;
        }
        // TODO 使用analysis规范语句
        // _analysis = buf;
        // 获取到当前的内容
        // _message += _analysis.get_content();
        _message = buf;
    }
    // 若文件不是读到结尾
    if (errno != EAGAIN) {
        log->log((*lang)["MessageReceiver_read_error"]);
    }
}

std::string MessageReceiver::get_content() {
    return _message;
}


MessageReceiver::MessageReceiver() {

}





