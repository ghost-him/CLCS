#pragma once
#include "stdinc.h"

struct Command_Parameter {
    char para;
    std::string description;
    bool has_arg;
    std::function<void(void *)> func;
};

class Command_Parameter_Analysis {
public :
    static std::shared_ptr<Command_Parameter_Analysis> ptr();
    /*
     * 添加一个参数
     * 参数，是否传参，描述，调用的函数的名字
     */
    template<class Func>
    static void add(char para, bool has_arg, const std::string& description, Func && func) {
        Command_Parameter one;
        one.para = para;
        one.has_arg = has_arg;
        one.description = description;
        one.func = std::function(std::forward<Func>(func));
        // 添加到储存中
        parameters.emplace(para, one);
        // 添加到格式列表中
        format_string.push_back(para);
        if (has_arg) {
            format_string.push_back(':');
        }
    }

    /*
     * 解析参数
     * 如果返回true, 则程序继续运行
     * 如果返回false, 则程序停止运行
     */
    static bool parse(int argc, char* argv[]);
    // 客户端和服务端的参数不同
    static std::function<void()> startInit;
private:
    static std::shared_ptr<Command_Parameter_Analysis> _ptr;
    Command_Parameter_Analysis() = default;
    static std::string format_string;
    // 输出帮助信息
    static void help(void *);
    // 储存信息, 通过参数来查找
    static std::map<char, Command_Parameter> parameters;

};
