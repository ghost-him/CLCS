#include "Message_Process.h"

std::shared_ptr<Message_Process> Message_Process::_ptr(new Message_Process());

std::shared_ptr<Message_Process> Message_Process::ptr() {
    return _ptr;
}

void Message_Process::startInit() {
    _sc = Server_Connector::ptr();
    _ms = Client_Message_Sender::ptr();
    _logger = Log::ptr();
    _um = User_Manager::ptr();

    _ms->startInit();
    _ms->set_fd(_sc->get_socketfd());
    _mg.startInit();

    _logger->log("[info] Message_process: inited %c");
}

void Message_Process::Test_Latency() {
    _logger->log("[info] Message_process: test_latency %c");
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    std::chrono::nanoseconds now_time = now.time_since_epoch();
    // 获取当前的微秒数
    set_and_send(Message_Header::TEXT_SYS, Message_Header::TIME, Message_Header::NOMEAN,
                 User_Manager::get_server_uuid(),
                 std::to_string(now_time.count()),
                 false);
}

void Message_Process::recall_mes(const std::string& uuid, const std::string & message) {
    _logger->log("[info] Message_process: send echo message %c");
    set_and_send(Message_Header::RECALL, Message_Header::NOMEAN, Message_Header::NOMEAN,
                 uuid,
                 message,
                 false);

}

void Message_Process::send_user_message(const std::string & uuid, const std::string & content) {
    _logger->log("[info] Message_process: send user message %c");
    set_and_send(Message_Header::TEXT_CHAT, Message_Header::USER, Message_Header::NOMEAN,
                 uuid,
                 content,
                 true);
}

void Message_Process::send_group_message(const std::string & content) {
    _logger->log("[info] Message_process: send group message %c");
    set_and_send(Message_Header::TEXT_CHAT, Message_Header::GROUP, Message_Header::NOMEAN,
                 User_Manager::get_server_uuid(),
                 content,
                 false);
}

void Message_Process::init_the_server() {
    _logger->log("[info] Message_process: send init server message %c");
    set_and_send(Message_Header::TEXT_SYS, Message_Header::INIT_SYS, Message_Header::NOMEAN,
                 User_Manager::empty_uuid,
                 _um->get_key_content(_um->get_self_pub_path()),
                 false);
}

void Message_Process::add_user(const std::string & uuid) {
    _logger->log("[info] Message_process: send user add message %c");
    set_and_send(Message_Header::TEXT_SYS, Message_Header::ADD_USER, Message_Header::NOMEAN,
                 User_Manager::get_server_uuid(),
                 uuid,
                 false);
}

void Message_Process::update_connection() {
    _logger->log("[info] Message_process: update connection info %c");
    _ms->set_fd(_sc->get_socketfd());
}

void Message_Process::join_group(const std::string& message) {
    _logger->log("[info] request to join the group");
    set_and_send(Message_Header::TEXT_SYS, Message_Header::GROUP, Message_Header::JOIN,
                 User_Manager::get_server_uuid(),
                 message,
                 false);
}

void Message_Process::leave_group() {
    set_and_send(Message_Header::TEXT_SYS, Message_Header::GROUP, Message_Header::LEAVE,
                 User_Manager::get_server_uuid(),
                 "ok",
                 false);
}

void Message_Process::set_and_send(Message_Header::Level level, Message_Header::Option para1, Message_Header::Option para2,
                                   const std::string& uuid,
                                   const std::string& message,
                                   bool is_encrypt) {
    pthread_mutex_lock(&_is_send_message);
    _mg.set_level_options(level, para1, para2);
    _mg.set_target_user_uuid(uuid);
    _mg.set_content(message, is_encrypt);

    // 发送消息
    _ms->send_message(_mg.get_message());

    pthread_mutex_unlock(&_is_send_message);
}