#include "Command_Service.h"

Service Service::operator=(Service& ser) {
    _function = ser._function;
    _store = ser._store;
    _is_set = ser._is_set;
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



// 分析当前的参数
void Command_Service::update(const std::vector<std::string>& command) {
    if (command.empty()) {
        alert();
        return ;
    }

    if ( _group.count(command[0]) != 0) {
        // 检查当前的命令是否存在
        auto com = get(command[0]);
        if (com == nullptr) { // 不存在
            alert();
            return ;
        }
        for (auto i = 1; i < command.size(); i++) {
            // 获取当前 命令-参数go 的指针， 检查是否为空指针
            // 若为空指针， 则表示没有当前的命令
            auto temp = (*com).get(command[i]);
            if (temp == nullptr) {
                alert();
                return;
            }
            // 执行 命令-参数 这个函数
            (*(*com).get(command[i]))(command);
        }
    } else {
        auto com = get(command[0]);
        if (com == nullptr) { // 不存在
            alert();
            return ;
        }
        (*com)(command);
    }
}

// 设置函数
Service& Command_Service::set(const std::string& command, std::string&& des) {
    if (_store.count(command) == 0) {
        Service temp;
        temp.description = std::move(des);
        temp.name = command;
        _store.emplace(command, std::move(temp));
    }
    return _store[command];
}

Service* Command_Service::get(const std::string& command) {
    if (_store.count(command) == 0) {
        return nullptr;
    }
    return &_store[command];
}

void Command_Service::set_group_param(std::string && command) {
    _group.emplace(std::move(command));
}

void Command_Service::help() {
    for (auto i : _store) {
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
