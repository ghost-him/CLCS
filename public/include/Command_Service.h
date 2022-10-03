/*
 * 作用：用于管理Command_Analysis分析出来的命令所要执行的动作
 */

/*
 * store["cat"]["user"]();
 */


#pragma once
#include "stdinc.h"

class Service {
public:
    Service() = default;
    Service operator=(Service&);

    // 运行当前的函数
    bool operator()(const std::vector<std::string>&);

    void operator=(const std::function<void(const std::vector<std::string>&)> &);

    // 有内容则表示返回成功
    // 设置函数
    Service& set(const std::string&);
    // 获取函数
    Service* get(const std::string&);

    std::function<void(const std::vector<std::string>&)> _function;

    // 储存
    std::map<std::string, Service> _store;

    bool _is_set = false;
};

class Command_Service {
public:
    // 显示帮助页面
    std::function<void()> help;

    std::function<void()> alert;

    // 分析当前的参数
    void update(const std::vector<std::string>&);

    // 设置函数
    Service& set(const std::string&);
    Service* get(const std::string&);

    void set_group_param(std::string &&);
private:
    std::map<std::string, Service> _store;
    std::set<std::string> _group;
};