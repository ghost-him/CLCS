#include "Message_Header_Analysis.h"

void Message_Header_Analysis::set_header(const std::shared_ptr<char[]> & ptr) {
    _header = ptr;
}

bool Message_Header_Analysis::convert() {
    std::stringstream ss(_header.get());
    // 读取头文件
    if (ss.eof()) {
        return false;
    }
    ss >> _level;
    if (ss.eof()) {
        return false;
    }
    ss >> _para1;
    if (ss.eof()) {
        return false;
    }
    ss >> _para2;
    if (ss.eof()) {
        return false;
    }
    ss >> _uuid1;
    if (ss.eof()) {
        return false;
    }
    ss >> _uuid2;
    if (ss.eof()) {
        return false;
    }
    ss >> _message_size;
    return true;
}

MessageHeader::Level Message_Header_Analysis::get_level() {
    return _level;
}

MessageHeader::Option Message_Header_Analysis::get_para1() {
    return _para1;
}
MessageHeader::Option Message_Header_Analysis::get_para2() {
    return _para2;
}

const std::string& Message_Header_Analysis::get_target_uuid() const {
    return _uuid1;
}

const std::string& Message_Header_Analysis::get_source_uuid() const {
    return _uuid2;
}

int Message_Header_Analysis::get_content_len() {
    return _message_size;
}