#include "Init.h"

void Init::startInit() {
    system("figlet CLCS");
    auto file_manager = FileManager::getInstance();
    // 添加默认的文件路径
    file_manager->_dir_path["history"] = "history/";
    file_manager->_dir_path["option"] = "option/";
    file_manager->_dir_path["language_path"] = "option/language/";
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

    // 初始化线程池
    ThreadPool::InitThreadPool();

    // 初始化连接器
    Epoll_Reactor::getInstance()->Init_Epoll();

}
bool Init::init_dir(const std::string & str) {
    // 如果找不到
    if (access(str.c_str(), F_OK) == -1) {
        std::cerr << "[warn] " << str << " dir not exist\n";
        std::cerr << "[info] creating " << str << " dir...\n";
        if (mkdir(str.c_str(), 0755) == -1) {
            std::cerr << "[error] create "<< str <<" dir failed: " << strerror(errno) << "\n";
            return false;
        }
    }
    return true;
}

