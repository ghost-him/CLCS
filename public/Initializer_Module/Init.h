#pragma once
#include "stdinc.h"
#include "File_Manager_Module/File_Manager.h"
#include "Language_Module/Language.h"
#include "Setting.h"
#include "ThreadPool_Module/Thread_Pool.h"
#include "Message_Module/Message.h"
#include "Command_Module/Command_Analysis.h"
#include "Command_Module/Command_Service.h"
#include "Command_Line_Parameter_Analysis_Module/Command_Parameter_Analysis.h"
/*
 * 初始化系统， 若程序第一次运行， 则强制显示页面
 * 若已经初始化好类页面， 则无需再次初始化
 * 单例设计模式
 */
// TODO: 规范程序返回值
// TODO: 重新排版各个模块的import
// TODO 设置执行不同类型的初始化
// TODO: 规范类的声明
class Init {
public:
    /*
     * 开始初始化
     */
    static std::function<void()> startInit;

private:
    static bool init_dir(const std::string &);

};