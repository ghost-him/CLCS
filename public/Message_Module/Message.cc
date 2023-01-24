#include "Message.h"


// TODO 重写该函数
// 读入消息， 然后计算加密以后的消息的长度
/*
Message_Generator& Message_Generator::operator = (const char * chars) {
    std::string str = chars;
    set_content(str);
    return *this;
}
 */

Message_Analysis::Message_Analysis() {

}

void Message_Analysis::startInit() {
    u_m = User_Manager::ptr();
    // 设置解密的钥匙
    dec.startInit();
    dec.set_and_read_key(u_m->get_self_pri_path());
}


std::shared_ptr<unsigned char[]> Message_Analysis::get_raw() {
    return _stream->get_content();
}

size_t Message_Analysis::get_raw_len() {
    return _stream->get_content_len();
}

void Message_Analysis::set_message(std::shared_ptr<Message_Stream> stream) {
    // 设置要解密的消息
    _stream = stream;
}

const std::string& Message_Analysis::get_content() {
#ifdef DEBUG_MAIN
    std::cerr << "message analysis start decrypt the content" << std::endl;
#endif

    /*
     * 分段读取并解码一个大的文字块
     */

    // 清空代码
    _content.clear();
    auto analysis = _stream->analysis;

    auto message_size = _stream->get_content_len();
    if ((analysis->get_level() != Message_Header::TEXT_CHAT) ||
       ((analysis->get_level() == Message_Header::TEXT_CHAT) && (analysis->get_para1() != Message_Header::USER))) {
        // 如果不是要加密的消息
        _content.resize(message_size, '\0');
        for (int i = 0; i < message_size; i++) {
            _content[i] = static_cast<char>(_stream->get_content()[i]);
        }
        return _content;
    }
#ifdef DEBUG_MAIN
    std::cerr << "message analysis user message" << " message length: " << _message_size << std::endl;
    std::cerr << "message analysis dec size:" << dec.size() << std::endl;
#endif

    size_t len = message_size;
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
        // 解码当前的字节块
        dec.set_input(_stream->get_content().get() + block * read_len, read_len);
        // 转换失败
        if (!dec.convert()) {
            _content.clear();
#ifdef DEBUG_MAIN
            std::cerr << "rsa_decrypt end convert failed" << std::endl;
#endif
            return _content;
        }
#ifdef DEBUG_MAIN
        std::cerr << "rsa_decrypt end convert" << std::endl;
#endif
        // 将转换好的字节块添加到文字后面
        _content += dec.get_output().get();
        // 读取下一个字节块
        block ++;
    }
#ifdef DEBUG_MAIN
    std::cerr << "message analysis: content " <<  _content << std::endl;
#endif

    return _content;
}


void Message_Receiver::startInit() {
    log = Log::ptr();
    lang = Language::ptr();
    _stream = std::make_shared<Message_Stream>();
}

int Message_Receiver::read_command() {
    int ret = read_header();
    //  如果不是正常的读取
    if (ret <= 0) {
        return ret;
    }

    // 解析并判断头标记是否合法
    if (!_stream->analysis->convert()) {
        // 不合法
        return 0;
    }
    // 设置要读的长度
    set_read_content_len(_stream->analysis->get_content_len());
    ret = read_content();
    // 不论是不是正常的读取，都返回
    return ret;
}


int Message_Receiver::read_header() {
#ifdef DEBUG_INPUT
    std::cerr << "try to read _header, fd: " << _socket_fd << std::endl;
#endif

    int total_len = -1;
    // 读取头标记，头标记以\n结尾
    do {
        total_len ++;
        auto ret = read(_socket_fd, _stream->get_header().get() + total_len, 1);
        if (ret < 0) {
            log->log((std::string)"[error] Message_Receiver: read _header error: %e, last time read:" + _stream->get_header().get());
            return -1;
        } else if (ret == 0) {
            log->log((*lang)["MessageReceiver_close_connect"]);
            return 0;
        }

    }while (total_len < 100 && *(_stream->get_header().get() + total_len) != '\n');
    // 设置读到的长度
    _stream->set_header_len(total_len + 1); // total_len的长度是消息的长度， 不包含最后的\n
#ifdef DEBUG_INPUT
    std::cerr << "message receiver receive _header: ";
    write(STDERR_FILENO, _stream->get_header().get(), _stream->get_header_len());
    std::cerr << std::endl;
#endif
    return _stream->get_header_len();
}

int Message_Receiver::read_content() {
    // 读取内容
    if (!_is_content) {
        log->log((*lang)["MessageReceiver_read_content_error"]);
        return false;
    }
    // 读取完内容以后就该读取头标记了
    _is_content = false;

    // 函数的返回值
    int ret;
    // 读取指定长度的内容
    ret = read(_socket_fd, _stream->get_content().get(), _stream->get_content_len());
    if (ret < 0) {
        // 读取失败
        log->log((*lang)["MessageReceiver_read_error"]);
        return -1;
    } else if (ret == 0) {
        // 连接关闭
        log->log("target user close the connect");
        return 0;
    }
    // 正常读完所有的消息
#ifdef DEBUG_INPUT
    std::cerr << "message receiver receive content: ";
    write(STDERR_FILENO, _stream->get_content().get(), _stream->get_content_len());
    std::cerr << std::endl;
#endif
    // 返回返回值
    return ret;
}

void Message_Receiver::set_read_content_len(int len) {
    // 设置要读的内容
    _is_content = true;
    _stream->set_content_len(len);
}

void Message_Receiver::set_fd(int fd) {
    _socket_fd = fd;
}

int Message_Receiver::get_fd() const {
    return _socket_fd;
}

std::shared_ptr<Message_Stream> Message_Receiver::get_message() {
    return _stream;
}

Message_Receiver::Message_Receiver() {
    _socket_fd = 0;
    _is_content = false;
}

