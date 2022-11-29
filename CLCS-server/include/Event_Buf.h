#pragma once
#include "stdinc.h"
#include "Message_Stream.h"

/*
 * event_buf 类
 * 用于描述一个事件的数据，有要发送的数据和已经接收到的数据
 */

class Event_Buf {
public :
    Event_Buf();
    Event_Buf(const Event_Buf &) = delete;

    void push_receive_message(const std::shared_ptr<Message_Stream>& );
    void push_receive_message(const Message_Stream& );

    void push_send_message(const std::shared_ptr<Message_Stream>& );
    void push_send_message(const Message_Stream& );

    std::shared_ptr<Message_Stream> get_next_receive_message();
    std::shared_ptr<Message_Stream> get_next_send_message();
private:
    // 从客户端收到的消息
    std::deque<std::shared_ptr<Message_Stream>> receive_message;
    // 要发送给客户端的消息
    std::deque<std::shared_ptr<Message_Stream>> send_message;

};