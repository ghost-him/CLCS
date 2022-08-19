#include "Command_Analysis.h"
// 初始化静态变量
Command_Analysis *Command_Analysis::_pCommand_Analysis = new Command_Analysis;

Command_Analysis *Command_Analysis::getInstance() {
    return _pCommand_Analysis;
}

void Command_Analysis::Init_Analysis_System() {
    log = Log::getInstance();

}

void Command_Analysis::check(const std::string & str) {
    std::stringstream ss(str);
    std::string func;
    ss >> func;
    // TODO 优化这个判断结构
    // 1. 易读性
    // 2. 效率
    if (func == "help") {
        help();
    } else if (func == "reset_target") {
        // 更改ip和端口
        if (ss.eof()) {
            Business::reset_target_ip_port();
        } else {
            std::string ip;
            int port;
            ss >> ip;
            ss >> port;
            Business::reset_target_ip_port(ip, port);
        }
    } else if (func == "re_connect") {
        // 重新连接
        Business::re_connect_server();
    } else if (func == "cat") {
        // 查看命令
        std::string para;
        while (!ss.eof()) {
            ss >> para;
            Business::cat_func(para);
        }
    } else if (func == "recall") {
        // 发送回调消息
        std::string content;
        int first = str.find_first_of('"') + 1;
        int last = str.find_last_of('"');
        if ((first - 1) == last || last == -1) {
            std::cout << "无效的格式" << std::endl;
            return;
        }
        content = str.substr(first , last - first);
        Business::recall_request(content);
    } else if (func == "rename") {
        // 重命名用户
        std::string old_name;
        if (ss.eof()) {
            std::cout << "无效的指令格式, 请查看help" << std::endl;
            return;
        }
        ss >> old_name;
        std::string new_name;
        if (ss.eof()) {
            std::cout << "无效的指令格式, 请查看help" << std::endl;
            return;
        }
        ss >> new_name;
        Business::rename(old_name, new_name);
    } else if (func == "send") {
        // 向用户发送消息
        // 读取uuid
        std::string uuid;
        if (ss.eof()) {
            std::cout << "无效的指令格式, 请查看help" << std::endl;
            return ;
        }
        ss >> uuid;

        // 发送的消息
        std::string content;
        int first = str.find_first_of('"') + 1;
        int last = str.find_last_of('"');
        if ((first - 1) == last || last == -1) {
            std::cout << "无效的格式" << std::endl;
            return;
        }
        content = str.substr(first , last - first);
        Business::send_message(uuid, content);
    } else if (func == "add") {
        // 添加用户
        std::string uuid;
        if (ss.eof()) {
            std::cout << "无效的指令格式，请查看help" << std::endl;
            return ;
        }
        ss >> uuid;
        Business::add_user(uuid);

    } else if (func == "save") {
        Business::save();
    } else {
        std::cout << "无效的指令: " << str << "\n";
        std::cout << "输入help查看全部指令\n" << std::flush;
    }
}

void Command_Analysis::help() {
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

}
