#include "Event_Buf.h"

Event_Buf::Event_Buf() {

}

void Event_Buf::push_receive_message(const Message_Stream& messageStream) {
    std::shared_ptr<Message_Stream> ptr = std::make_shared<Message_Stream>();
    *ptr = messageStream;
    push_receive_message(ptr);
}

void Event_Buf::push_receive_message(const std::shared_ptr<Message_Stream>& ptr) {
    receive_message.push_back(ptr);
}

void Event_Buf::push_send_message(const Message_Stream& messageStream) {
    std::shared_ptr<Message_Stream> ptr = std::make_shared<Message_Stream>();
    *ptr = messageStream;
    push_send_message(ptr);
}

void Event_Buf::push_send_message(const std::shared_ptr<Message_Stream>& ptr) {
    send_message.push_back(ptr);
}

std::shared_ptr<Message_Stream> Event_Buf::get_next_receive_message() {
    // 当里面没有内容的时候，则返回空
    if (!receive_message.size())
        return nullptr;
    auto temp = receive_message.front();
    receive_message.pop_front();
    return temp;
}

std::shared_ptr<Message_Stream> Event_Buf::get_next_send_message() {
    // 当里面没有内容的时候，则返回空
    if (!send_message.size())
        return nullptr;
    auto temp = send_message.front();
    send_message.pop_front();
    return temp;
}