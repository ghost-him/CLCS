#include "Business.h"

std::shared_ptr<Log> Business::log = nullptr;
std::shared_ptr<Language> Business::lang = nullptr;
std::shared_ptr<Setting> Business::setting = nullptr;
std::shared_ptr<User_Manager> Business::u_m = nullptr;
bool Business::is_start = true;

MessageReceiver Business::mr;
MessageAnalysis Business::ma;
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
    std::cout << "重新设置目标服务器的ip和地址" << std::endl;
    std::string ip;
    int port;
    bool is_first = true;
    do {
        // 如果已经输错过一次了
        if (!is_first)
            std::cout << "无效的ip和端口, 请重新输入" << std::endl;
        is_first = false;
        std::cout << "请输入ip地址: " << std::flush;
        std::cin >> ip;

        std::cout << "请输入端口: " << std::flush;
        std::cin >> port;
    } while (!s_c->set_target(ip.c_str(), port));
    mr.set_fd(s_c->get_socketfd());

    // 重新获取服务器的连接
    m_process->update_connection();
    m_process->init_the_server();

    std::cout << "设置完成! "
              << "\n当前服务器的ip为: " + (*setting)["target_server_ip"] + " 当前的端口为: " +
                 (*setting)["target_server_port"] << "\n请输入 re_connect 重新连接目标服务器" << std::endl;
}


void Business::reset_target_ip_port(const std::string &ip, int port) {
    if (!s_c->set_target(ip.c_str(), port))
        std::cout << "无效的ip和端口" << std::endl;
    else
        std::cout << "设置完成! "
                  << "\n当前服务器的ip为: " + (*setting)["target_server_ip"] + " 当前的端口为: " +
                     (*setting)["target_server_port"] << "\n请输入 re_connect 重新连接目标服务器" << std::endl;
}

void Business::re_connect_server() {
    std::cout << "重新连接目标服务器" << std::endl;
    s_c->close_connect();
    is_start = true;
    sleep(1);
    if (!connect_server((*setting)["target_server_ip"], std::stoi((*setting)["target_server_port"]))) {
        //  当连接未成功的时候
        std::cout << "连接失败，请查看日志，可尝试重新设置服务器的ip和端口" << std::endl;
        return;
    }

    std::cout << "连接成功！当前服务器的ip为: " + (*setting)["target_server_ip"] + " 当前的端口为: " +
                 (*setting)["target_server_port"] << std::endl;
    mr.set_fd(s_c->get_socketfd());

    m_process->update_connection();
    m_process->init_the_server();

}


void Business::cat_connect_status() {
    if (s_c->get_connect_status())
        std::cout << "当前已连接到: " + (*setting)["target_server_ip"] + ":" + ((*setting)["target_server_port"])
                  << std::endl;
    else
        std::cout << "当前无连接" << std::endl;
}

void Business::Testing_network_latency() {
    std::cout << "测试延迟中" << std::endl;
    // 发送测试信息
    m_process->Test_Latency();

}

void Business::start_accept_from_server() {
#ifdef DEBUG
    std::cerr << "start accept from server" << std::endl;
#endif
    // 检查当前的连接状态
    if (!_is_closed && !s_c->get_connect_status()) {
        sleep(5);
        log->log("[error] business: connect error , try to reconnect");
        std::cout << "连接失败， 尝试重新连接" << std::endl;
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
        std::cout << "读取消息失败，尝试重新连接" << std::endl;
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
        case MessageHeader::RECALL: {
            /*
            // 获取参数1
            auto para = ma.get_para1();
            if (para == MessageHeader::SEND) {
                // 发送回应数据
                return_recall(ma.get_source_uuid(), ma.get_raw(), ma.get_content_len());

            } else {
                std::cout << ma.get_content();
            }
             */
            display_recall(ma.get_raw(), ma.get_message_len());
            break;
        }
        case MessageHeader::KEY_TEST: {
            /*
            auto para = ma.get_para1();
            if (para == MessageHeader::SEND) {
                // 解密， 查看结果是否为ok, 如果是ok则返回消息
            } else {
                // 验证成功 使用信号量来控制这两个消息
            }
            break;
             */
            break;
        }
        case MessageHeader::TEXT_CHAT: {
#ifdef DEBUG
            std::cerr << "receive message" << std::endl;
#endif
            // 接受到了消息
            std::cout << "---receive message---" << std::endl;
            // 将当前接受到的消息显示出来
            Business::display_user_message(ma.get_uuid2(), ma.get_content());
            break;
        }
        case MessageHeader::TEXT_SYS: {
            if (ma.get_para1() == MessageHeader::INIT_SYS) {
                // 初始化自己在服务器中的消息
                u_m->set_server_uuid(ma.get_uuid2());
            } else if (ma.get_para1() == MessageHeader::ADD_USER) {
                // 添加一个用户
                add_user();
            } else if (ma.get_para1() == MessageHeader::TIME) {
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

#ifdef DEBUG
    std::cerr << "end accept from server" << std::endl;
#endif

}


void Business::recall_request(const std::string &message) {
    m_process->recall_mes(u_m->get_server_uuid(), message);
}

void Business::send_message(const std::string &name, const std::string &message) {

    User *user = u_m->find_user(name);
    if (user == nullptr) {
        std::cout << "找不到该用户" << std::endl;
#ifdef DEBUG
        std::cerr << "找不到用户：" << name << std::endl;
#endif
        return;
    }
    // 找到了用户
    m_process->send_user_message(user->get_uuid(), message);
}

void Business::save() {
    setting->save_setting();
    u_m->save();
    std::cout << "保存成功" << std::endl;
}

void Business::cat_uuid() {
    std::cout << u_m->get_self_uuid() << std::endl;
}

void Business::cat_user() {
    auto &database = u_m->get_database();
    std::cout << "uuid\t\t\t\t\t\t昵称\n";
    // 遍历已经存在的用户信息
    for (auto &i: database) {
        const User &temp = i.second;
        std::cout << temp.get_uuid() << "\t" << temp.get_name() << "\n";
    }
}

void Business::rename(const std::string &old, const std::string &new_name) {
    // 重命名
    if (u_m->rename(old, new_name))
        std::cout << "成功改名" << std::endl;
    else
        std::cout << "改名失败， 用户不存在" << std::endl;
}

void Business::add_user(const std::string &uuid) {
    // 添加用户
    if (!User_Manager::check_is_uuid(uuid)) {
        log->log("[error] business : invalid uuid: " + uuid);
        std::cout << "无效的uuid" << std::endl;
        return;
    }
    // 发送请求数据
    m_process->add_user(uuid);
}

void Business::init_server() {
    // 初始化服务器
    std::cout << "连接目标服务器中..." << std::endl;
    // 尝试连接
    std::string ip = (*setting)["target_server_ip"];
    int port = std::stoi((*setting)["target_server_port"]);
    while (!connect_server(ip, port)) {
        // TODO 用语言管理器管理
        std::cout << "连接失败，请重新输入目标服务器的ip:" << std::endl;
        std::cin >> ip;
        std::cout << "端口:" << std::endl;
        std::cin >> port;
    };
    // 初始化消息读取器
    mr.set_fd(s_c->get_socketfd());
    std::cout << "服务器连接成功" << std::endl;

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
    std::cout << "------ recall message ------\n" << std::flush;
    write(STDOUT_FILENO, message.get(), len);
    std::cout << "\n";
}

void Business::display_user_message(const std::string &uuid, const std::string &message) {
#ifdef DEBUG
    std::cerr << "display user message :" << "uuid " << uuid << " message " << message << std::endl;
#endif

    auto ti = time(nullptr);
    // 查找用户
    auto ptr = u_m->find_user(const_cast<std::string &>(uuid));
    // 若可以找到用户， 则显示昵称
    if (ptr != nullptr)
        std::cout << ctime(&ti) << ptr->get_name() << ": " << message << "\n";
    else // 若找不到用户， 则显示uuid
        std::cout << ctime(&ti) << uuid << ": " << message << "\n";
    std::cout << std::endl;
}

void Business::add_user() {
    // 如果长度过低， 则说明未找到用户
    if (ma.get_message_len() < 43) {
        std::cout << "未找到该用户" << std::endl;
        return;
    }
    // 将unsigned char*指针 转换为 char*
    auto con = std::reinterpret_pointer_cast<char *>(ma.get_raw());
    std::string target_uuid(36, '\0');
    // 读取uuid
    // TODO 优化读取的过程
    for (int i = 0; i < 37; i++) {
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
    std::string pub_path = FileManager::ptr()->get("keys") + target_uuid + ".pub";
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
    std::cout << "延迟为: " << (double) (now_time.tv_usec - std::stoull(time)) / 1000 << "毫秒" << std::endl;
}

