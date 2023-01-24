#include "Message_Stream.h"

Message_Stream::Message_Stream() {
    _header = std::make_shared<char[]>(256);
    _header_len = std::make_shared<size_t>(256);
    _content = std::make_shared<unsigned char[]>(BUFSIZ);
    _content_len = std::make_shared<size_t>(BUFSIZ);
    _max_len = std::make_shared<size_t>(BUFSIZ);

    analysis = std::make_shared<Message_Header_Analysis>();
    analysis->set_header(_header);
}

Message_Stream::Message_Stream(const Message_Stream & message_stream) {
    _header = message_stream._header;
    _content = message_stream._content;
    _content_len = message_stream._content_len;
}

void Message_Stream::set_header_len(size_t header_len) {
    *_header_len = header_len;
    // 如果小于开辟的空间
    if (header_len < 256)
        _header[header_len] = '\0';
}

size_t Message_Stream::get_header_len() {
    return *_header_len;
}

void Message_Stream::set_content_len(size_t content_len) {
    if (content_len > (*_max_len)) {
        _content = std::make_shared<unsigned char[]>(*_max_len);
        *_max_len = content_len;
    }
    *_content_len = content_len;
}

size_t Message_Stream::get_content_len() {
    return *_content_len;
}

std::shared_ptr<char[]> Message_Stream::get_header() {
    return _header;
}

std::shared_ptr<unsigned char[]> Message_Stream::get_content() {
    return _content;
}

std::shared_ptr<Message_Stream> Message_Stream::copy() {
    std::shared_ptr<Message_Stream> temp = std::make_shared<Message_Stream>();
    // 复制参数
    *(temp->_header_len) = *_header_len;
    temp->set_content_len(*_content_len);
    // 复制数据
    memcpy(temp->_header.get(), _header.get(), *_header_len);
    memcpy(temp->_content.get(), _content.get(), *_max_len);
    // 复制解析
    temp->analysis->convert();
    // 返回副本
    return temp;
}
