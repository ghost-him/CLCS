/*
 * 作用：用于管理Command_Analysis分析出来的命令所要执行的动作
 */

/*
 * 执行 cat user 命令
 * store["cat"]["user"]();
 */

#pragma once
#include "stdinc.h"

class Service {
public:
    Service(){}
    Service operator=(Service&);

    // 运行当前的函数
    bool operator()(const std::vector<std::string>&);
    // 设置绑定的函数
    void operator=(const std::function<void(const std::vector<std::string>&)> &);
    // 有内容则表示返回成功
    /*
     * 设置子命令
     * 子命令的 名字，描述
     */
    Service& set(const std::string&, std::string&&);
    // 获取子命令
    Service* get(const std::string&);

    // 绑定的函数
    std::function<void(const std::vector<std::string>&)> _function;

    // 设置自己的执行格式的说明
    void set_format(std::string &&);

    // 设置允许连续执行
    Service& set_continuous();

    // 获取自己名字
    std::string get_name();

    // 子函数储存，通过名字来获取并访问
    std::map<std::string, Service> _store;

    // 当前命令的描述
    std::string description;
    // 当前命令的名字
    std::string name;
    // 当前命令的格式
    std::string format;
    // 是否设置了函数
    bool _is_set = false;
    // 是否允许连续执行(cat user time uuid)
    bool _is_continuous = false;
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
private:
    void help(const Service&, int);

    /*
     * 递归执行命令
     * 当前递归的列表, 命令列表，当前递归的下标
     */
    void execute(Service& , const std::vector<std::string> &, int);

    Service head;
};