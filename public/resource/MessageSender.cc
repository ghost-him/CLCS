#include "Message_Sender.h"

std::shared_ptr<Client_Message_Sender> Client_Message_Sender::_ptr(new Client_Message_Sender());


std::shared_ptr<Client_Message_Sender> Client_Message_Sender::ptr() {
    return _ptr;
}

void Message_Sender::startInit() {
    log = Log::ptr();
    lang = Language::ptr();
}

void Message_Sender::set_fd(int target) {
    _socket_fd = target;
}

int Message_Sender::get_fd() {
    return _socket_fd;
}

int Message_Sender::send_message(const std::string & str) {
    _sender.lock();
    log->log("[info] message sender start send message");

#ifdef DEBUG_OUTPUT
    std::cerr << "send message len : " << str.size() << std::endl;
    std::cerr << "send message :";
    write(STDERR_FILENO, str.c_str(), str.size());
    std::cerr << std::endl;
#endif

    // 只负责发送消息
    ssize_t ret = write(_socket_fd, str.c_str(), str.size());

    _sender.unlock();
    log->log("[info] message sender end send message");
    // 判断返回值
    if (ret == -1) {
        log->log((*lang)["MessageSender_send_str_error"]);
    }
    return ret;

}

int Message_Sender::send_message(std::shared_ptr<unsigned char[]> message, int len) {
    _sender.lock();
    log->log("[info] message sender start send message");

#ifdef DEBUG_OUTPUT
    std::cerr << "send message len : " << len << std::endl;
    std::cerr << "send message :";
    write(STDERR_FILENO, message.get(), len);
    std::cerr << std::endl;
#endif

    // 发送加密的消息
    ssize_t ret = write(_socket_fd, message.get(), len);

    _sender.unlock();
    log->log("[info] message sender end send message");
    // 判断返回值
    if (ret == -1) {
        log->log((*lang)["MessageSender_send_ptr_error"]);
    }
    return ret;
}

Message_Sender::Message_Sender() {
    _socket_fd = 0;
}

int Server_Message_Sender::send_message(const std::string& str) {
#ifdef DEBUG_OUTPUT
    std::cerr << "send message :";
    write(STDERR_FILENO, str.c_str(), str.size());
    std::cerr << std::endl;
#endif


#ifdef ET
    int remain_to_send = str.size();
    do{
        int ret = write(_socket_fd, str.c_str() + (str.size() - remain_to_send), remain_to_send);
        switch (ret) {
            case 0: {
                return ret;
            }
            case -1: {
                if (errno == EAGAIN) {
                    break;
                } else {
                    // 读取异常
                    return ret;
                }
            }
            default:
                break;
        }
        remain_to_send -= ret;
    }while (remain_to_send > 0);
#else
    int ret = write(_socket_fd, str.c_str(), str.size());
        switch (ret) {
            case 0: {
                return ret;
            }
            case -1: {
                return ret;
            }
            default:
                break;
        }
#endif
    return str.size();
}

int Server_Message_Sender::send_message(std::shared_ptr<unsigned char[]> message, int len) {
#ifdef DEBUG_OUTPUT
    std::cerr << "send message :";
    write(STDERR_FILENO, message.get(), len);
    std::cerr << std::endl;
#endif

#ifdef ET
    int remain_to_send = len;
    do{
        int ret = write(_socket_fd, message.get() + (len - remain_to_send), remain_to_send);
        switch (ret) {
            case 0: {
                return ret;
            }
            case -1: {
                if (errno == EAGAIN) {
                    break;
                } else {
                    // 读取异常
                    return ret;
                }
                break;
            }
            default:
                break;
        }
        remain_to_send -= ret;
    }while (remain_to_send > 0);
#else
    int ret = write(_socket_fd, message.get(), len);
        switch (ret) {
            case 0: {
                return ret;
            }
            case -1:{
                // 读取异常
                return ret;
            }
            default:
                break;
        }
#endif

    return len;
}

