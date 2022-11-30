#include <Init.h>
#include <Setting.h>
#include <Business.h>
/*
 * 用于设置服务端和客户端的不同之处的函数
 */

std::shared_ptr<Command_Service> command_service = std::make_shared<Command_Service>();
void add_command() {
    command_service->alert = [](){
        OUT << "无效的指令格式或参数！！！输入help来查看当前支持的参数";
    };

    command_service->set("help", "输出此条信息") = [](const std::vector<std::string>& param){
        command_service->help();
    };

    command_service->set("reset_target", "重新设置目标服务器的ip和地址") = [](const std::vector<std::string>& param){
        if (param.size() == 1) {
            Business::reset_target_ip_port();
        } else {
            if (param.size() != 3) {
                command_service->alert();
                command_service->help();
                return ;
            }
            Business::reset_target_ip_port(param[1], std::stoi(param[2]));
        }
    };

    command_service->set("re_connect", "重新连接目标服务器") = [](const std::vector<std::string>& param){
        Business::re_connect_server();
    };

    command_service->set_group_param("cat");
    command_service->set("cat", "查看信息");
    command_service->get("cat")->set_format("cat [参数]");
    command_service->get("cat")->set("connect", "与服务器的连接状态") = [](const std::vector<std::string>& param){
        Business::cat_connect_status();
    };

    command_service->get("cat")->set("uuid", "自己的uuid") = [](const std::vector<std::string>& param){
        Business::cat_uuid();
    };

    command_service->get("cat")->set("user", "当前已经保存的用户") = [](const std::vector<std::string>& param){
        Business::cat_user();
    };

    command_service->get("cat")->set("latency" ,"与服务器的延迟") = [](const std::vector<std::string>& param){
        Business::Testing_network_latency();
    };

    command_service->get("cat")->set("time", "当前系统的时间") = [](const std::vector<std::string>& param){
        auto a = time(nullptr);
        OUT << (std::string)ctime(&a);
    };

    command_service->set("recall", "向服务器发送回调消息") = [](const std::vector<std::string>& param){
        if (param.size() != 2) {
            command_service->alert();
            command_service->help();
            return ;
        }
        Business::recall_request(param[1]);
    };
    command_service->get("recall")->set_format("recall [消息]");

    command_service->set("rename", "更改用户的昵称") = [](const std::vector<std::string>& param){
        if (param.size() != 3) {
            command_service->alert();
            command_service->help();
            return ;
        }
        Business::rename(param[1], param[2]);
    };
    command_service->get("rename")->set_format("rename [uuid/旧昵称] [新昵称]");

    command_service->set("send", "向用户发送消息") = [](const std::vector<std::string>& param){
        if (param.size() != 3) {
            command_service->alert();
            command_service->help();
            return ;
        }

        Business::send_message(param[1], param[2]);
    };
    command_service->get("send")->set_format("send [uuid/昵称] [消息]");

    command_service->set("add", "添加指定的用户") = [](const std::vector<std::string>& param){
        if (param.size() != 2) {
            command_service->alert();
            command_service->help();
            return ;
        }
        Business::add_user(param[1]);
    };
    command_service->get("add")->set_format("add [uuid]");

    command_service->set("save", "保存设置") = [](const std::vector<std::string>& param){
        Business::save();
    };
}

std::function<void()> Init::startInit = [](){
    OUT << R"(
  ____ _     ____ ____             _ _            _
 / ___| |   / ___/ ___|        ___| (_) ___ _ __ | |_
| |   | |  | |   \___ \ _____ / __| | |/ _ \ '_ \| __|
| |___| |__| |___ ___) |_____| (__| | |  __/ | | | |_
 \____|_____\____|____/       \___|_|_|\___|_| |_|\__|
)";

    auto file_manager = File_Manager::ptr();
    // 添加默认的文件路径
    file_manager->_dir_path["history"] = "history/";
    file_manager->_dir_path["option"] = "option/";
    file_manager->_dir_path["language_path"] = "option/language/";
    file_manager->_dir_path["keys"] = "keys/";
    file_manager->_dir_path["log"] = "log/";

    /*
     * 查看文件夹是否存在
     * 若文件夹不存在， 则创建文件夹
     *      若创建失败， 则退出程序
     *
     */

    init_dir("history");
    init_dir("option");
    init_dir("option/language");
    init_dir("log");
    init_dir("keys");

    // 初始化日志系统的输出位置
    time_t time_now = time(0);
    char buf[50];
    struct tm time_struct;
    time_struct = *localtime(&time_now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &time_struct);
    Log::ptr()->setFilePath((std::string)"log/" + buf + ".log");

    // 初始化设置系统
    Setting::ptr()->startInit();


    // TODO 初始化当前用户的uuid
    // TODO 设置当前的私钥

    // 初始化语言系统
    Language::ptr()->InitLanguage();

    // 初始化用户管理系统
    User_Manager::ptr()->Init_User_Manager();

    // 初始化线程池
    Thread_Pool::startInit();

    // 初始化服务器连接器
    Server_Connector::ptr()->startInit();

    // 初始化业务系统
    Business::startInit();

    // 初始化连接器
    Message_Process::ptr()->startInit();

    // 添加命令
    add_command();

    // 初始化命令分析器
    Command_Analysis::ptr()->Init_Analysis_System();
    Command_Analysis::ptr()->set_service(command_service);

};

std::function<void()> Setting::first_time_run = [](){
    OUT << "欢迎使用CLCS!\n接下来将进行进一步的设置， 从而来保证获得最佳的体验。";
    OUT << "Welcome to CLCS! Further settings will be made to ensure the best experience.\n";
    OUT << "选择语言：";
    OUT << "choose language: \n";
    OUT << "1. 简体中文(默认/default)\n2. english\n";
    char temp;
    IN >> temp;
    if (temp == '2')
        set("sys_language", "en_US");

    OUT << "请输入目标服务器的ip:";
    OUT << "Please enter the ip of the target server:\n";
    std::string targetIp;
    IN >> targetIp;
    set("target_server_ip", targetIp);

    OUT << "请输入目标服务器的端口:";
    OUT << "Please enter the port of the target server:\n";
    int port;
    IN >> port;
    set("target_server_port", std::to_string(port));

    auto hardware_cur = std::thread::hardware_concurrency();

    set("threadpool_max_thread", std::to_string(hardware_cur * 4));
    set("threadpool_min_thread", std::to_string(hardware_cur));
    set("threadpool_grow_thread", std::to_string(hardware_cur));
    set("threadpool_set_check_time", "5");


    // 初始化uuid
    uuid_t uuid;
    uuid_generate(uuid);
    char uuid_c[37];
    uuid_unparse(uuid, uuid_c);
    set("uuid", uuid_c);

    // 初始化自己的密匙
    RSA_controller generator;
    auto f = File_Manager::ptr();
    std::string pri_path = f->get("keys") + "self.pri";
    std::string pub_path = f->get("keys") + "self.pub";
    generator.set_private_key_path(pri_path);
    generator.set_public_key_path(pub_path);
    generator.generate_keys();

    set("pub_key_path", pub_path);
    set("pri_key_path", pri_path);

    OUT << "初始化完成";
};
