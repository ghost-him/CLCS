#pragma once
#include "stdinc.h"
#include "FileManager.h"
#include "Language.h"
#include "Setting.h"
#include "ThreadPool.h"
#include "Epoll_Reactor.h"
#include "Server_Connector.h"
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
    static void startInit();


private:
    static bool init_dir(const std::string &);

};