#include <Init.h>
#include <Setting.h>
#include <Business.h>
/*
 * 用于设置服务端和客户端的不同之处的函数
 */

Command_Service command_service;
void add_command() {
    command_service.help = [](){
        std::cout << "help\n\t输出此条信息\n\n";
        std::cout << "reset_target\n\t重新设置目标服务器的ip和地址\n\n";
        std::cout << "re_connect\n\t重新连接目标服务器\n\n";
        std::cout << "cat\n\t查看信息\n\tcat [参数]\n\t\ttime 当前系统的时间\n\t\tuuid 自己的uuid\n\t\tuser 当前已经保存的用户\n\t\tconnect 与服务器的连接状态\n\t\tlatency 与服务器的延迟\n\n";
        std::cout << "recall\n\t向服务器发送回调消息\n\trecall [消息]\n\n";
        std::cout << "add\n\t添加用户信息\n\tadd [uuid]\n\n";
        std::cout << "rename\n\t更改用户的昵称\n\trename [uuid/旧昵称] [新昵称]\n\n";
        std::cout << "send\n\t向用户发送消息\n\tsend [uuid/昵称] [消息]\n\n";
        std::cout << "save\n\t保存设置\n\n";
        std::cout << "quit\n\t退出程序\n\n" << std::flush;
    };

    command_service.alert = [](){
        std::cout << "无效的指令格式或参数！！！输入help来查看当前支持的参数" << "\n";
    };

    command_service.set("help") = [](const std::vector<std::string>& param){
        command_service.help();
    };

    command_service.set("reset_target") = [](const std::vector<std::string>& param){
        if (param.size() == 1) {
            Business::reset_target_ip_port();
        } else {
            if (param.size() != 3) {
                command_service.alert();
                command_service.help();
                return ;
            }
            Business::reset_target_ip_port(param[1], std::stoi(param[2]));
        }
    };

    command_service.set("re_connect") = [](const std::vector<std::string>& param){
        Business::re_connect_server();
    };

    command_service.set_group_param("cat");
    command_service.set("cat").set("connect") = [](const std::vector<std::string>& param){
        Business::cat_connect_status();
    };

    command_service.set("cat").set("uuid") = [](const std::vector<std::string>& param){
        Business::cat_uuid();
    };

    command_service.set("cat").set("user") = [](const std::vector<std::string>& param){
        Business::cat_user();
    };

    command_service.set("cat").set("latency") = [](const std::vector<std::string>& param){
        Business::Testing_network_latency();
    };

    command_service.set("cat").set("time") = [](const std::vector<std::string>& param){
        auto a = time(nullptr);
        std::cout << ctime(&a) << std::endl;
    };

    command_service.set("recall") = [](const std::vector<std::string>& param){
        if (param.size() != 2) {
            command_service.alert();
            command_service.help();
            return ;
        }
        Business::recall_request(param[2]);
    };

    command_service.set("rename") = [](const std::vector<std::string>& param){
        if (param.size() != 3) {
            command_service.alert();
            command_service.help();
            return ;
        }
        Business::rename(param[1], param[2]);
    };

    command_service.set("send") = [](const std::vector<std::string>& param){
        if (param.size() != 3) {
            command_service.alert();
            command_service.help();
            return ;
        }

        Business::send_message(param[1], param[2]);
    };

    command_service.set("add") = [](const std::vector<std::string>& param){
        if (param.size() != 2) {
            command_service.alert();
            command_service.help();
            return ;
        }
        Business::add_user(param[1]);
    };

    command_service.set("save") = [](const std::vector<std::string>& param){
        Business::save();
    };
}

std::function<void()> Init::startInit = [](){
    system("figlet CLCS");
    auto file_manager = FileManager::getInstance();
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
    Log::getInstance()->setFilePath((std::string)"log/" + buf);

    // 初始化设置系统
    Setting::getInstance()->InitSetting();


    // TODO 初始化当前用户的uuid
    // TODO 设置当前的私钥

    // 初始化语言系统
    Language::getInstance()->InitLanguage();

    // 初始化用户管理系统
    User_Manager::getInstance()->Init_User_Manager();

    // 初始化线程池
    ThreadPool::InitThreadPool();

    /*
    // 初始化连接器
    Epoll_Reactor::getInstance()->Init_Epoll();
    */

    // 初始化服务器连接器
    Business::s_c.startInit();

    // 初始化命令分析器
    Command_Analysis::getInstance()->Init_Analysis_System();
    Command_Analysis::getInstance()->set_service(&command_service);

    // 初始化业务系统
    Business::startInit();

    // 添加命令
    add_command();
};

std::function<void()> Setting::first_time_run = [](){
    std::cout << "欢迎使用CLCS!\n接下来将进行进一步的设置， 从而来保证获得最佳的体验。" << std::endl;
    std::cout << "Welcome to CLCS! Further settings will be made to ensure the best experience.\n" << std::endl;
    std::cout << "选择语言：" << std::endl;
    std::cout << "choose language: \n" << std::endl;
    std::cout << "1. 简体中文(默认/default)\n2. english\n" << std::endl;
    char temp = getchar();
    if (temp == '2')
        _writeFile["sys_language"] = "en_US";

    std::cout << "请输入目标服务器的ip:" << std::endl;
    std::cout << "Please enter the ip of the target server:\n" << std::endl;
    std::string targetIp;
    std::cin >> targetIp;
    _writeFile["target_server_ip"] = targetIp;

    std::cout << "请输入目标服务器的端口:" << std::endl;
    std::cout << "Please enter the port of the target server:\n" << std::endl;
    int port;
    std::cin >> port;
    _writeFile["target_server_port"] = std::to_string(port);

    auto hardware_cur = std::thread::hardware_concurrency();
    _writeFile["threadpool_max_thread"] = std::to_string(hardware_cur * 4);
    _writeFile["threadpool_min_thread"] = std::to_string(hardware_cur);
    _writeFile["threadpool_grow_thread"] = std::to_string(hardware_cur);
    _writeFile["threadpool_set_check_time"] = "5";

    // 初始化uuid
    uuid_t uuid;
    uuid_generate(uuid);
    char uuid_c[37];
    uuid_unparse(uuid, uuid_c);
    _writeFile["uuid"] = uuid_c;

    // 初始化自己的密匙
    RSA_controller generator;
    FileManager *f = FileManager::getInstance();
    std::string pri_path = f->get("keys") + "self.pri";
    std::string pub_path = f->get("keys") + "self.pub";
    generator.set_private_key_path(pri_path);
    generator.set_public_key_path(pub_path);
    generator.generate_keys();

    _writeFile["pub_key_path"] = pub_path;
    _writeFile["pri_key_path"] = pri_path;

    std::cout << "初始化完成" << std::endl;
};
