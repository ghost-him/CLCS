#include "Init.h"
#include "Epoll_Reactor.h"
#include "Setting.h"

std::function<void()> Init::startInit = [](){
    OUT << R"(
  ____ _     ____ ____
 / ___| |   / ___/ ___|       ___  ___ _ ____   _____ _ __
| |   | |  | |   \___ \ _____/ __|/ _ \ '__\ \ / / _ \ '__|
| |___| |__| |___ ___) |_____\__ \  __/ |   \ V /  __/ |
 \____|_____\____|____/      |___/\___|_|    \_/ \___|_|
)";
    OUT << (std::string)"version: " + VERSION;

    signal(SIGPIPE, SIG_IGN);

    auto file_manager = File_Manager::ptr;
    // 添加默认的文件路径
    file_manager()->_dir_path["history"] = "history/";
    file_manager()->_dir_path["option"] = "option/";
    file_manager()->_dir_path["language_path"] = "option/language/";
    file_manager()->_dir_path["keys"] = "keys/";
    file_manager()->_dir_path["log"] = "log/";

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
    Log::ptr()->setFilePath((std::string)"log/" + buf);

    // 初始化设置系统
    Setting::ptr()->startInit();


    // TODO 初始化当前用户的uuid
    // TODO 设置当前的私钥

    // 初始化语言系统
    Language::ptr()->InitLanguage();

    // 初始化用户管理系统
    User_Manager::ptr()->Init_User_Manager();

    // 初始化线程池
    Thread_Pool::ptr()->startInit();

    // 初始化连接器
    Epoll_Reactor::ptr()->startInit();
};

std::function<void()> Setting::first_time_run = [](){
    std::cout << "欢迎使用CLCS!\n接下来将进行进一步的设置， 从而来保证获得最佳的体验。" << std::endl;
    std::cout << "Welcome to CLCS! Further settings will be made to ensure the best experience.\n" << std::endl;
    std::cout << "选择语言："<< std::endl;
    std::cout << "choose language: \n"<< std::endl;
    std::cout << "1. 简体中文(默认/default)\n2. english\n"<< std::endl;
    char temp = getchar();
    if (temp == '2')
        set("sys_language", "en_US");

    // 服务器的设置：
    std::cout << "是否要设置当前服务器的ip与端口?" << std::endl;
    std::cout << "Whether to set the ip and port of the current server?" << std::endl;
    std::cout << "1. 是/yes\n2. 否/no(默认)" << std::endl;
    char choose;
    std::cin >> choose;
    if (choose == '1') {
        std::cout << "请输入目标服务器的ip:" << std::endl;
        std::cout << "Please enter the ip of the target server:\n" << std::endl;
        std::string targetIp;
        std::cin >> targetIp;
        set("target_server_ip", targetIp);

        std::cout << "请输入目标服务器的端口:" << std::endl;
        std::cout << "Please enter the port of the target server:\n" << std::endl;
        int port;
        std::cin >> port;
        set("target_server_port", std::to_string(port));
    }
    // 设置线程池
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

    std::cout << "初始化完成" << std::endl;
};
