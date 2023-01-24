#include "Message_Generator.h"

Message_Generator::Message_Generator()
        : _level(Message_Header::RECALL), _para1(Message_Header::NOMEAN), _para2(Message_Header::NOMEAN){
    _len = 0;
    _stream = std::make_shared<Message_Stream>();
}

void Message_Generator::startInit() {
    log = Log::ptr();
    lang = Language::ptr();
    setting = Setting::ptr();
    u_m = User_Manager::ptr();
    enc.startInit();
}

void Message_Generator::set_level_options(Message_Header::Level level, Message_Header::Option para1, Message_Header::Option para2) {
    _level = level;
    _para1 = para1;
    _para2 = para2;
}

void Message_Generator::set_target_user_uuid(const std::string& uuid) {
    _target_user_uuid = uuid;
    enc.init();
    if (!enc.set_and_read_key(u_m->get_pub_key_path(uuid))) {
#ifdef DEBUG_MAIN
        std::cerr << "set target user uuid: " << uuid << std::endl;
#endif
        log->log("[error] Message_Generator: set and read key error: " + u_m->get_pub_key_path(uuid));
    }
}

const Message_Generator & Message_Generator::operator=(const Message_Generator& t) {
    _stream = t._stream->copy();
    _level = t._level;
    _para1 = t._para1;
    _para2 = t._para2;
    _target_user_uuid = t._target_user_uuid;
    _len = t._len;
    _content = t._content;
    lang = t.lang;
    return *this;
}


bool Message_Generator::set_content(const std::string& str, bool is_encrypt) {
    // 如果加密当前的内容
    if (is_encrypt) {
        int unit = enc.size();
        int str_len = unit - 11;
        int num = str.size() / str_len + 1;

        // 设置目标消息数据的长度， 并在合适的时候变动数组的长度
        set_content_len(num * unit);
        _content = str;
        // 开辟一个指定的空间的大小

        for (int index = 0; index < num; index ++) {
            // 转换一部分字符串的片段
            enc.set_input(str.substr(str_len * index, str_len));
            enc.convert();
            // 将当前的片段输出到指定的位置
            memcpy(_stream->get_content().get() + index * unit, enc.get_output().get(), unit);
            _stream->get_content().get()[index * unit + unit] = '\0';
        }
    } else {
        // 如果不加密
        set_content_len(str.size());
        memcpy(_stream->get_content().get(), str.c_str(), _len);
    }
    return true;
}

void Message_Generator::set_content_len(int len) {
    // 拼接头标记
    _len = len;
    // 设置内容的长度
    _stream->set_content_len(len);
}

std::shared_ptr<Message_Stream> Message_Generator::get_message() {
    std::string header = get_header();
    // 复制头标记的长度和内容
    memcpy(_stream->get_header().get(), header.c_str(), header.size());
    _stream->set_header_len(header.size());
    return _stream;
}

std::string Message_Generator::get_header() {
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
