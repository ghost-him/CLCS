#include "Message.h"

MessageSender* MessageSender::_pMessageSender = new MessageSender;

MessageGenerator::MessageGenerator()
    :_level(MessageHeader::RECALL), _para1(MessageHeader::NOMEAN), _para2(MessageHeader::NOMEAN){
}

void MessageGenerator::startInit() {
    log = Log::ptr;
    lang = Language::ptr;
    setting = Setting::ptr;
    u_m = User_Manager::ptr;
    enc.startInit();
}

void MessageGenerator::set_level_options(MessageHeader::Level level, MessageHeader::Option para1, MessageHeader::Option para2) {
    _level = level;
    _para1 = para1;
    _para2 = para2;
}


void MessageGenerator::set_level(MessageHeader::Level level) {
    _level = level;
}

MessageHeader::Level MessageGenerator::get_level() {
    return _level;
}

void MessageGenerator::set_option(MessageHeader::Option para1, MessageHeader::Option para2) {
    _para1 = para1;
    _para2 = para2;
}

MessageHeader::Option MessageGenerator::get_para1() const {
    return _para1;
}

MessageHeader::Option MessageGenerator::get_para2() const {
    return _para2;
}

void MessageGenerator::set_target_user_uuid(const std::string& uuid) {
    _target_user_uuid = uuid;
    enc.init();
    if (!enc.set_and_read_key(u_m->get_pub_key_path(uuid))) {
#ifdef IS_DEBUG
        std::cerr << "set target user uuid: " << uuid << std::endl;
#endif
        log->log("[error] MessageGenerator: set and read key error: " + u_m->get_pub_key_path(uuid));
    }
}

std::string MessageGenerator::get_target_user_uuid() {
    return _target_user_uuid;
}


const MessageGenerator & MessageGenerator::operator=(const MessageGenerator& t) {
    _buf = t._buf;
    _level = t._level;
    _para1 = t._para1;
    _para2 = t._para2;
    _target_user_uuid = t._target_user_uuid;
    _len = t._len;
    _content = t._content;
    lang = t.lang;
    return *this;
}


bool MessageGenerator::set_content(const std::string& str, bool is_encrypt) {
    // 如果加密当前的内容
    if (is_encrypt) {
        int unit = enc.size();
        int str_len = unit - 11;
        int num = str.size() / str_len + 1;
        _len = num * unit;
        _content = str;
        // 开辟一个指定的空间的大小
        _buf.reset(new unsigned char[_len + 1]);
        for (int index = 0; index < num; index ++) {
            // 转换一部分字符串的片段
            enc.set_input(str.substr(str_len * index, str_len));
            enc.convert();
            // 将当前的片段输出到指定的位置
            memcpy(_buf.get() + index * unit, enc.get_output(), unit);
            _buf.get()[index * unit + unit] = '\0';
        }
    } else {
        // 如果不加密
        _len = str.size();
        _buf.reset(new unsigned char[_len]);
        memcpy(_buf.get(), str.c_str(), _len);
    }
    return true;
}

// TODO 重写该函数
// 读入消息， 然后计算加密以后的消息的长度
MessageGenerator& MessageGenerator::operator = (const char * chars) {
    std::string str = chars;
    set_content(str);
    return *this;
}

MessageAnalysis::MessageAnalysis() {
    _message_size = 0;
}

void MessageAnalysis::startInit() {
    u_m = User_Manager::ptr;
    // 设置解密的钥匙
    dec.startInit();
    dec.set_and_read_key(u_m->get_self_pri_path());
}

MessageAnalysis::operator const char *() {
    return _content.c_str();
}
/*
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
 */

std::string MessageGenerator::get_header() {
    /*
     *  1 1 1\n
     *  [uuid1] [uuid2]\n
     *  [len]\n
     */

    std::string res(5, '\0');
    res[0] = _level;
    res[1] = ' ';
    res[2] = _para1;
    res[3] = ' ';
    res[4] = _para2;
    res.push_back(' ');
    res += _target_user_uuid;
    res += " ";
    res += User_Manager::get_self_uuid();
    res.push_back(' ');
    res += std::to_string(_len);
    res.push_back('\n'); // 以\n结尾
    return res;
}

std::shared_ptr<unsigned char> MessageGenerator::get_content() {
    return _buf;
}

int MessageGenerator::get_content_size() {
    return _len;
}

void MessageGenerator::set_content_len(int len) {
    _len = len;
}

MessageHeader::Level MessageAnalysis::get_level() {
    return _level;
}

MessageHeader::Option MessageAnalysis::get_para1() {
    return _para1;
}
MessageHeader::Option MessageAnalysis::get_para2() {
    return _para2;
}

const std::string& MessageAnalysis::get_uuid1() const {
    return _uuid1;
}

const std::string& MessageAnalysis::get_uuid2() const {
    return _uuid2;
}

std::shared_ptr<unsigned char> MessageAnalysis::get_raw() {
    return _raw;
}

const std::string& MessageAnalysis::get_content() {
#ifdef IS_DEBUG
    std::cerr << "message analysis start decrypt the content" << std::endl;
#endif

    /*
     * 分段读取并解码一个大的文字块
     */

    // 清空代码
    _content.clear();
    // 如果是回调信息
    if (_level != MessageHeader::TEXT_CHAT) {
        _content.resize(_message_size + 1, '\0');
        for (int i = 0; i < _message_size; i++) {
            _content[i] = static_cast<char>(_raw.get()[i]);
        }
        return _content;
    }
#ifdef IS_DEBUG
    std::cerr << "message analysis user message" << " message length: " << _message_size << std::endl;
    std::cerr << "message analysis dec size:" << dec.size() << std::endl;
#endif


    // 定义一个缓存区
    unsigned char buf[BUFSIZ];
    size_t len = _message_size;
    size_t read_len;
    // 当前的字节块
    int block = 0;
    while (1) {
        // 每次读取dec长度的信息
        read_len = std::min(dec.size(), len);
        // 如果为0， 则说明读到了末尾
        if (read_len == 0)
            break;
        // 每次减读到的长度
        len = std::max(len - dec.size(), (size_t)0);
        // 读取中间一段的字节块
        memcpy(buf, _raw.get() + block * read_len, read_len);
        // 解码当前的字节块
        dec.set_input(buf, read_len);
        // 转换失败
        if (!dec.convert()) {
            _content.clear();
#ifdef IS_DEBUG
            std::cerr << "rsa_decrypt end convert failed" << std::endl;
#endif
            return _content;
        }
#ifdef IS_DEBUG
        std::cerr << "rsa_decrypt end convert" << std::endl;
#endif
        // 将转换好的字节块添加到文字后面
        _content += dec.get_output();
        // 读取下一个字节块
        block ++;
    }
#ifdef IS_DEBUG
    std::cerr << "message analysis: content " <<  _content << std::endl;
#endif

    return _content;
}

int MessageAnalysis::get_message_len() {
    return _message_size;
}

int MessageAnalysis::get_command(MessageReceiver* mr) {
    // 读取头信息
    int ret;
    if ((ret = mr->read_header(), ret) <= 0)
        return ret;
    const char * head = mr->get_header();

    // 检查头信息
    if (!check_header(head)) {
        return false;
    }

    std::stringstream ss(head);
    // 读取头信息的各个信息
    if (ss.eof())
        return false;
    ss >> _level;

    if (ss.eof())
        return false;
    ss >> _para1;

    if (ss.eof())
        return false;
    ss >> _para2;

    if (ss.eof())
        return false;
    ss >> _uuid1;

    if (ss.eof())
        return false;
    ss >> _uuid2;

    if (ss.eof())
        return false;
    ss >> _message_size;
    // 设置读取的长度
    mr->set_read_content(_message_size);

    // 如果读成功了
    if ((ret = mr->read_content()) > 0) {
        _raw = mr->get_content();
    } else {
        return ret;
    }
    return true;
}

bool MessageAnalysis::check_header(const char * header) {
    // 检查头标记的正确性
    switch (*header) {
        case MessageHeader::RECALL: {
            if (*(header + 2) != MessageHeader::NOMEAN || (*(header +4) != MessageHeader::NOMEAN))
                return false;
            break;
        }
        case MessageHeader::TEXT_CHAT: {
            if (*(header + 2) != MessageHeader::NOMEAN || *(header + 4) != MessageHeader::NOMEAN)
                return false;
            break;
        }
        case MessageHeader::TEXT_SYS: {
            if ((*(header + 2) != MessageHeader::INIT_SYS && *(header + 2) != MessageHeader::ADD_USER && *(header + 2) != MessageHeader::TIME) || *(header + 4) != MessageHeader::NOMEAN)
                return false;
            break;
        }
    }
    return true;
}



void MessageSender::startInit() {
    _socket_fd = 0;
    log = Log::ptr;
    lang = Language::ptr;
    _generator.startInit();
}

void MessageSender::set_fd(int target) {
    _socket_fd = target;
}

void MessageSender::send_message(const std::string & str) {
    // 只负责发送消息
    ssize_t ret = write(_socket_fd, str.c_str(), str.size());
    if (ret == -1) {
        log->log((*lang)["MessageSender_send_str_error"]);
    }
}

void MessageSender::send_message(std::shared_ptr<unsigned char> str, int len) {
    // 发送加密的消息
    ssize_t ret = write(_socket_fd, str.get(), len);
    if (ret == -1) {
        log->log((*lang)["MessageSender_send_ptr_error"]);
    }
}

MessageSender::MessageSender() {
    log = Log::ptr;
    lang = Language::ptr;
}

void MessageReceiver::startInit() {
    _socket_fd = 0;
    log = Log::ptr;
    lang = Language::ptr;
    _socket_fd = 0;
}

const char* MessageReceiver::get_header() {
    return header;
}

std::shared_ptr<unsigned char> MessageReceiver::get_content() {
    return _buf;
}

int MessageReceiver::read_header() {
#ifdef IS_DEBUG
    std::cerr << "try to read header" << std::endl;
#endif

    int total_len = -1;
    // 读取头标记，头标记以\n结尾
    do {
        total_len ++;
        auto ret = read(_socket_fd, header + total_len, 1);
        if (ret < 0) {
            log->log((std::string)"[error] MessageReceiver: read header error: %e, last time read:" + header);
            return -1;
        } else if (ret == 0) {
            log->log((*lang)["MessageReceiver_close_connect"]);

            return 0;
        }

    }while (total_len < 100 && *(header + total_len) != '\n');
    // 设置读到的长度
    header_len = total_len + 1; // total_len的长度是消息的长度， 不包含最后的\n
#ifdef IS_DEBUG
    std::cerr << "message receiver receive header: " << header << std::endl;
#endif

    return header_len;
}

int MessageReceiver::read_content() {
    // 读取内容
    if (!_is_content) {
        log->log((*lang)["MessageReceiver_read_content_error"]);
        return false;
    }
    // 读取完内容以后就该读取头标记了
    _is_content = false;

    _buf.reset(new unsigned char [_content_len]);

    int ret;
    // 读取指定长度的内容
    ret = read(_socket_fd, _buf.get(), _content_len);
    if (ret < 0) {
        log->log((*lang)["MessageReceiver_read_error"]);
        return -1;
    } else if (ret == 0) {
        log->log("target user close the connect");
        return 0;
    }
    // 正常读完所有的消息
#ifdef IS_DEBUG
    std::cerr << "message receiver receive content: " << _buf << std::endl;
#endif
    return ret;
}

void MessageReceiver::set_fd(int fd) {
    _socket_fd = fd;
}

int MessageReceiver::get_fd() const {
    return _socket_fd;
}

MessageReceiver::MessageReceiver() {

}

