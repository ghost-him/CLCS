#include "Setting.h"

Setting* Setting::_pSetting = new Setting;

Setting *Setting::getInstance() {
    return _pSetting;
}

void Setting::InitSetting() {
    log = Log::getInstance();
    _FileManager = FileManager::getInstance();
    std::string path = _FileManager->get("option") + "option.txt";
    _readFile.setFilePath(path);
    // 如果不存在文件，则创建文件
    if (access(path.c_str(), F_OK) == -1) {
        log->log("[warn] setting sys: can not find option file, %e\n");
        _writeFile.setFilePath(path);
        _writeFile._store = DefaultOption::store;
        first_time_run();
        _writeFile.writeFile();
    } else {
        _readFile.readFile(_writeFile._store);
        if (_writeFile._store.size() != DefaultOption::store.size()) {
            log->log("[error] setting sys: Incorrect number of entries\n");
            log->exit_process();
        }
    }
}
// TODO 完成此函数
void Setting::first_time_run() {
    std::cout << "欢迎使用CLCS!\n接下来将进行进一步的设置， 从而来保证获得最佳的体验。" << std::endl;
    std::cout << "Welcome to CLCS! Further settings will be made to ensure the best experience.\n" << std::endl;
    std::cout << "选择语言："<< std::endl;
    std::cout << "choose language: \n"<< std::endl;
    std::cout << "1. 简体中文(默认/default)\n2. english\n"<< std::endl;
    char temp = getchar();
    if (temp == '2')
        (*this)["sys_language"] = "en_US";

    std::cout << "请输入目标服务器的ip:" << std::endl;
    std::cout << "Please enter the ip of the target server:\n" << std::endl;
    std::string targetIp;
    std::cin >> targetIp;
    std::cout << targetIp;
    (*this)["target_server_ip"] = targetIp;

    std::cout << "请输入目标服务器的端口:" << std::endl;
    std::cout << "Please enter the port of the target server:\n" << std::endl;
    int port;
    std::cin >> port;
    (*this)["target_server_port"] = std::to_string(port);

    // 设置线程池
    auto hardware_cur = std::thread::hardware_concurrency();
    (*this)["threadpool_max_thread"] = std::to_string(hardware_cur * 4);
    (*this)["threadpool_min_thread"] = std::to_string(hardware_cur);
    (*this)["threadpool_grow_thread"] = std::to_string(hardware_cur);
    (*this)["threadpool_set_check_time"] = "5";
    std::cout << "初始化完成" << std::endl;
}

std::string &Setting::operator[](const std::string & str) {
    return _writeFile[str];
}

Setting::Setting() {

}

