#include "Command_Service.h"

Service Service::operator=(Service& ser) {
    _function = ser._function;
    _store = ser._store;
    _is_set = ser._is_set;
    name = ser.name;
    description = ser.description;
    format = ser.format;
    _is_continuous = ser._is_continuous;
    return *this;
}

bool Service::operator()(const std::vector<std::string>& param) {
    if (_is_set) {
        _function(param);
        return true;
    } else {
        return false;
    }
}

void Service::operator=(const std::function<void(const std::vector<std::string>&)> &func) {
    _function = func;
    _is_set = true;
}

// true表示成功执行
Service& Service::set(const std::string& command, std::string&& des) {
    if (_store.count(command) == 0) {
        Service temp;
        temp.description = std::move(des);
        temp.name = command;
        _store.emplace(command, temp);
    }
    return _store[command];
}

Service* Service::get(const std::string& command) {
    if (_store.count(command) == 0) {
        return nullptr;
    }
    return &_store[command];
}

void Service::set_format(std::string && str) {
    format = std::move(str);
}

Service& Service::set_continuous() {
    _is_continuous = true;
    return *this;
}

std::string Service::get_name() {
    return name;
}


// 分析当前的参数
void Command_Service::update(const std::vector<std::string>& command) {
    if (command.empty()) {
        alert();
        return ;
    }

    execute(head, command, 0);
}

// 设置函数
Service& Command_Service::set(const std::string& command, std::string&& des) {
    head.set(command, std::forward<std::string>(des));
    return *head.get(command);
}

Service* Command_Service::get(const std::string& command) {
    return head.get(command);
}

void Command_Service::help() {
    for (auto i : head._store) {
        help(i.second, 0);
    }
}

void Command_Service::help(const Service& now, int level) {
    std::string out_string;

    for (int i = 0; i < level; i++) out_string.push_back('\t');
    out_string += now.name;
    out_string.push_back('\n');

    for (int i = 0; i < level + 1; i++) out_string.push_back('\t');
    out_string += now.description;
    out_string.push_back('\n');

    if (!now.format.empty()) {
        for (int i = 0; i < level + 1; i++) out_string.push_back('\t');
        out_string += now.format;
        out_string.push_back('\n');
    }
    OUT << out_string;
    for (auto i : now._store) {
        help(i.second, level + 1);
    }
}

void Command_Service::execute(Service& service, const std::vector<std::string> & para, int index) {
    // 如果已经到结尾了
    if (index >= para.size()) {
        alert();
        help();
        return ;
    }
    auto ptr = service.get(para[index]);
    // 如果不存在当前的命令
    if (ptr == nullptr) {
        alert();
        help();
        return ;
    }

    // 如果设置的函数，则说明是最后一个命令了
    if ((*ptr)._is_set) {
        (*ptr)._function(para);
        return ;
    }
    // 如果没有设置函数，则存在子命令
    // 如果是可以连续执行的函数
    if ((*ptr)._is_continuous) {
        for (int i = index + 1; i < para.size(); i++) {
            execute(*ptr, para, i);
        }
        return ;
    } else {
        // 如果不是可以连续执行的函数
        execute(*ptr, para, index + 1);
        return ;
    }
    // 如果没有执行任何函数，则说明命令无效
    alert();
    help();
}
