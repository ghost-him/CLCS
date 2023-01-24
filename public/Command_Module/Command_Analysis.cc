#include "Command_Analysis.h"

#include <utility>
// 初始化静态变量
std::shared_ptr<Command_Analysis> Command_Analysis::_ptr(new Command_Analysis());

std::shared_ptr<Command_Analysis> Command_Analysis::ptr() {
    return _ptr;
}

void Command_Analysis::Init_Analysis_System() {
    _log = Log::ptr();
    _service = nullptr;
    _log->log("[info] command analysis init end");
}

void Command_Analysis::check(std::string && str) {
    if (str.empty())
        return;

    std::vector<std::string> command;
    // 0存命令， 其他位存参数

    int left = 0, right = 0;

    str.push_back(' ');

    while (str[left] == ' ') left ++;
    right = left;

    bool in_word = false;
    while (right < str.size()) {
        if (!in_word && str[right] == ' ') {
            std::string sub = str.substr(left, right - left);
            if (!sub.empty() && sub != " ")
                command.push_back(std::move(sub));
            left = right + 1;
        } else if (str[right] == '"') {
            if (in_word) {
                std::string sub = str.substr(left, right - left);
                if (!sub.empty()) {
                    command.push_back(std::move(sub));
                }
                in_word = false;
            } else {
                in_word = true;
            }
            left = right + 1;
        }
        right ++;
    }
    _service->update(command);
}

void Command_Analysis::set_service(const std::shared_ptr<Command_Service> & service) {
    _service = service;
}
