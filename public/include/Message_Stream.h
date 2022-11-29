#pragma once
#include "stdinc.h"
#include "Message_Header_Analysis.h"

class Message_Stream {
public:
    Message_Stream();

    Message_Stream(const Message_Stream &);

    /*
     * 设置头标记的长度
     */
    void set_header_len(size_t);
    /*
     * 获取头标记的长度
     */
    size_t get_header_len();

    /*
     * 设置数据的长度
     */
    void set_content_len(size_t);
    /*
     * 获取数据的长度
     */
    size_t get_content_len();
    /*
     * 获取头标记的内容
     */
    std::shared_ptr<char[]> get_header();
    /*
     * 获取数据的内容
     */
    std::shared_ptr<unsigned char[]> get_content();

    std::shared_ptr<Message_Stream> copy();

    /*
     * 解析器
     */
    Message_Header_Analysis analysis;
private:
    std::shared_ptr<char[]> _header;
    std::shared_ptr<size_t> _header_len;
    std::shared_ptr<unsigned char[]> _content;
    std::shared_ptr<size_t> _content_len;
    std::shared_ptr<size_t> _max_len;
};