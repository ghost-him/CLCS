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
    Service& set(const std::string&, std::string&&);
    // 获取函数
    Service* get(const std::string&);

    std::function<void(const std::vector<std::string>&)> _function;

    void set_format(std::string &&);

    // 储存
    std::map<std::string, Service> _store;

    // 当前命令的描述
    std::string description;
    // 当前命令的名字
    std::string name;
    // 当前命令的格式
    std::string format;

    bool _is_set = false;
};

class Command_Service {
public:
    // 显示帮助页面
    void help();

    std::function<void()> alert;

    // 分析当前的参数
    void update(const std::vector<std::string>&);

    // 设置函数
    Service& set(const std::string&, std::string&&);
    Service* get(const std::string&);

    void set_group_param(std::string &&);
private:
    void help(const Service&, int);

    std::map<std::string, Service> _store;
    std::set<std::string> _group;
};