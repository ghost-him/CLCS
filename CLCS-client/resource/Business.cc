#include "Business.h"

std::shared_ptr<Log> Business::log = nullptr;
std::shared_ptr<Language> Business::lang = nullptr;
std::shared_ptr<Setting> Business::setting = nullptr;
std::shared_ptr<User_Manager> Business::u_m = nullptr;
bool Business::is_start = true;

Message_Receiver Business::mr;
Message_Analysis Business::ma;
std::shared_ptr<Message_Process> Business::m_process = nullptr;
std::shared_ptr<Server_Connector> Business::s_c = nullptr;
bool Business::_is_closed = false;

void Business::startInit() {
    log = Log::ptr();
    lang = Language::ptr();
    setting = Setting::ptr();
    u_m = User_Manager::ptr();

    m_process = Message_Process::ptr();
    s_c = Server_Connector::ptr();

    mr.startInit();
    ma.startInit();
}


void Business::reset_target_ip_port() {
    OUT << "重新设置目标服务器的ip和地址\n";
    std::string ip;
    int port;
    bool is_first = true;
    do {
        // 如果已经输错过一次了
        if (!is_first)
            OUT << "无效的ip和端口, 请重新输入\n";
        is_first = false;
        OUT << "请输入ip地址: ";
        IN >> ip;
        OUT << "请输入端口: ";
        IN >> port;
    } while (!s_c->set_target(ip.c_str(), port));
    // 保存设置
    Setting::ptr()->set("target_server_port", std::to_string(port));
    Setting::ptr()->set("target_server_ip", ip);

    mr.set_fd(s_c->get_socketfd());

    // 重新获取服务器的连接
    m_process->update_connection();
    m_process->init_the_server();

    OUT << (std::string)"设置完成! "+"\n当前服务器的ip为: " + (*setting)["target_server_ip"] + " 当前的端口为: " +
                 (*setting)["target_server_port"] + "\n请输入 re_connect 重新连接目标服务器\n";
}


void Business::reset_target_ip_port(const std::string &ip, int port) {
    if (!s_c->set_target(ip.c_str(), port))
        OUT << "无效的ip和端口\n";
    else
        OUT << (std::string)"设置完成! " + "\n当前服务器的ip为: " + (*setting)["target_server_ip"] + " 当前的端口为: " +
                     (*setting)["target_server_port"] << "\n请输入 re_connect 重新连接目标服务器\n";
}

void Business::re_connect_server() {
    OUT << "重新连接目标服务器\n";
    s_c->close_connect();
    is_start = true;
    sleep(1);
    if (!connect_server((*setting)["target_server_ip"], std::stoi((*setting)["target_server_port"]))) {
        //  当连接未成功的时候
        OUT << "连接失败，请查看日志，可尝试重新设置服务器的ip和端口\n";
        return;
    }

    OUT << "连接成功！当前服务器的ip为: " + (*setting)["target_server_ip"] + " 当前的端口为: " +
                 (*setting)["target_server_port"];
    mr.set_fd(s_c->get_socketfd());

    m_process->update_connection();
    m_process->init_the_server();

}


void Business::cat_connect_status() {
    if (s_c->get_connect_status())
        OUT << "当前已连接到: " + (*setting)["target_server_ip"] + ":" + ((*setting)["target_server_port"])
        ;
    else
        OUT << "当前无连接";
}

void Business::Testing_network_latency() {
    OUT << "测试延迟中";
    // 发送测试信息
    m_process->Test_Latency();

}

void Business::start_accept_from_server() {
#ifdef DEBUG_CLIENT_BUSINESS
    std::cerr << "start accept from server";
#endif
    // 检查当前的连接状态
    if (!_is_closed && !s_c->get_connect_status()) {
        sleep(5);
        // 如果手动刷新，则此时已经连上了
        if (s_c->get_connect_status())
            return ;

        log->log("[error] business: connect error , try to reconnect");
        OUT << "连接失败， 尝试重新连接";
        re_connect_server();
        return;
    } else {
    }
    // 尝试读取消息， 查看是否可以成功读取
    int ret;
    if (!_is_closed && (ret = ma.get_command(&mr), ret) <= 0) {
        if (ret == 0) {
            log->log("business accept server : close connection");
            s_c->close_connect();
            return;
        }

        if (errno == EAGAIN) {
            return ;
        }

        log->log("[error] business: read message error, %e");
        OUT << "读取消息失败，尝试重新连接";
        s_c->close_connect();
        return;
    } else {
    }

    if (_is_closed)
        return;

    // 判断消息是否在自己的信息库中
    User *source_user = u_m->find_user(ma.get_uuid2());
    if (source_user == nullptr &&
        ma.get_uuid2() != User_Manager::get_server_uuid() &&
        ma.get_uuid2() != User_Manager::get_self_uuid()) {
        // 如果不是第一次运行， 则发送消息
        if (!is_start)
            add_user(ma.get_uuid2());
    }
    // 不是第一次运行了
    is_start = false;
    //  根据内容来执行相应的命令
    auto level = ma.get_level();
    switch (level) {
        case Message_Header::RECALL: {
            display_recall(ma.get_raw(), ma.get_message_len());
            break;
        }
        case Message_Header::KEY_TEST: {
            break;
        }
        case Message_Header::TEXT_CHAT: {
#ifdef DEBUG_CLIENT_BUSINESS
            std::cerr << "receive message";
#endif
            // 接受到了消息
            OUT << "---receive message---";
            // 将当前接受到的消息显示出来
            Business::display_user_message(ma.get_uuid2(), ma.get_content());
            break;
        }
        case Message_Header::TEXT_SYS: {
            if (ma.get_para1() == Message_Header::INIT_SYS) {
                // 初始化自己在服务器中的消息
                u_m->set_server_uuid(ma.get_uuid2());
            } else if (ma.get_para1() == Message_Header::ADD_USER) {
                // 添加一个用户
                add_user();
            } else if (ma.get_para1() == Message_Header::TIME) {
                // 显示延迟
                display_latency();
            }
            break;
        }
        default: {
            // 日志系统输出错误日志：错误的指令
            log->log((*lang)["Business_invalid_level"] + level);
        }
    }

#ifdef DEBUG_CLIENT_BUSINESS
    std::cerr << "end accept from server";
#endif

}


void Business::recall_request(const std::string &message) {
    m_process->recall_mes(u_m->get_server_uuid(), message);
}

void Business::send_message(const std::string &name, const std::string &message) {

    User *user = u_m->find_user(name);
    if (user == nullptr) {
        OUT << "找不到该用户";
#ifdef DEBUG_CLIENT_BUSINESS
        std::cerr << "找不到用户：" << name;
#endif
        return;
    }
    // 找到了用户
    m_process->send_user_message(user->get_uuid(), message);
}

void Business::save() {
    // 保存设置信息
    setting->save_setting();
    // 保存用户信息
    u_m->save();
    OUT << "保存成功";
}

void Business::cat_uuid() {
    OUT << u_m->get_self_uuid() + "\n";
}

void Business::cat_user() {
    auto &database = u_m->get_database();
    OUT << "uuid\t\t\t\t\t\t昵称";
    // 遍历已经存在的用户信息
    for (auto &i: database) {
        const User &temp = i.second;
        OUT << (std::string)temp.get_uuid() + "\t" + temp.get_name();
    }
    OUT << "\n";
}

void Business::rename(const std::string &old, const std::string &new_name) {
    // 重命名
    if (u_m->rename(old, new_name))
        OUT << "成功改名\n";
    else
        OUT << "改名失败， 用户不存在\n";
}

void Business::add_user(const std::string &uuid) {
    // 添加用户
    if (!User_Manager::check_is_uuid(uuid)) {
        log->log("[error] business : invalid uuid: " + uuid);
        OUT << "无效的uuid\n";
        return;
    }
    // 发送请求数据
    m_process->add_user(uuid);
}

void Business::init_server() {
    // 初始化服务器
    OUT << "连接目标服务器中...";
    // 尝试连接
    std::string ip = (*setting)["target_server_ip"];
    int port = std::stoi((*setting)["target_server_port"]);
    while (!connect_server(ip, port)) {
        OUT << "连接失败，请重新输入目标服务器的ip:";
        IN >> ip;
        OUT << "端口:";
        IN >> port;
    };

    // 保存设置
    Setting::ptr()->set("target_server_port", std::to_string(port));
    Setting::ptr()->set("target_server_ip", ip);

    // 初始化消息读取器
    mr.set_fd(s_c->get_socketfd());
    OUT << "服务器连接成功";

    // 初始化消息发送器
    m_process->update_connection();
    m_process->init_the_server();
}

bool Business::connect_server(const std::string &ip, int port) {
    s_c->set_target(ip.c_str(), port);
    s_c->start_connect();
    return s_c->get_connect_status();
}

void Business::display_recall(const std::shared_ptr<unsigned char[]> &message, size_t len) {
    OUT << "------ recall message ------\n";
    OUT->print(message.get(), len);
}

void Business::display_user_message(const std::string &uuid, const std::string &message) {
#ifdef DEBUG_CLIENT_BUSINESS
    std::cerr << "display user message :" << "uuid " << uuid << " message " << message;
#endif

    auto ti = time(nullptr);
    // 查找用户
    auto ptr = u_m->find_user(const_cast<std::string &>(uuid));
    // 若可以找到用户， 则显示昵称
    if (ptr != nullptr)
        OUT << ctime(&ti) << static_cast<std::string>(ptr->get_name()) + ": " + message;
    else // 若找不到用户， 则显示uuid
        OUT << ctime(&ti) << uuid + ": " + message;
    OUT;
}

void Business::add_user() {
    // 如果长度过低， 则说明未找到用户
    if (ma.get_message_len() < 43) {
        OUT << "未找到该用户";
        return;
    }
    // 将unsigned char*指针 转换为 char*
    auto con = std::reinterpret_pointer_cast<char *>(ma.get_raw());
    std::string target_uuid(36, '\0');
    // 读取uuid
    // TODO 优化读取的过程
    for (int i = 0; i < 36; i++) {
        target_uuid[i] = static_cast<char>(ma.get_raw().get()[i]);
    }
    // 检查该uuid是否符合uuid的格式
    if (!User_Manager::check_is_uuid(target_uuid)) {
        log->log("[warn] invalid uuid:" + target_uuid);
    }
    auto key_len = ma.get_message_len();
    // 新建一个用户
    User new_user;
    // 设置该用户的公钥的路径
    std::string pub_path = File_Manager::ptr()->get("keys") + target_uuid + ".pub";
    // 创建该公钥
    int fd = open(pub_path.c_str(), O_CREAT | O_RDWR, 0700);
    if (fd < 0) {
        log->log("[error] business: can not open the file: " + pub_path + " %e");
        return;
    }
    // 将公钥写入文件
    auto ret = write(fd, ma.get_raw().get() + 37, key_len - 37);
    if (ret < 0) {
        log->log("[error] business: can not write the file: %e");
        return;
    }
    close(fd);
    new_user.set_pub_path(pub_path);
    new_user.set_uuid(target_uuid);
    new_user.set_name(target_uuid.substr(0, 8));
    u_m->add_user(new_user);
}

void Business::display_latency() {
    // 读取微秒数
    std::string time(ma.get_message_len() + 1, '\0');
    for (int i = 0; i < ma.get_message_len(); i++) {
        time[i] = ma.get_raw().get()[i];
    }
    time[ma.get_message_len()] = '\0';

    // 读取当前的微秒数
    struct timeval now_time;
    gettimeofday(&now_time, nullptr);
    // 显示延迟
    OUT << "延迟为: " + std::to_string((double) (now_time.tv_usec - std::stoull(time)) / 1000) + "毫秒\n";
}

