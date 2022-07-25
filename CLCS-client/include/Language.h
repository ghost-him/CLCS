#pragma once
#include "stdinc.h"
#include "Log.h"
#include "FileManager.h"
#include "Default.h"
#include "Setting.h"
/*
 * 设计模式：单例模式， 用于管理语言
 */


class Language {
public:
    static Language* getInstance();

    /*
     * 获取指定字符串的ip
     */
    const std::string& operator[](const std::string&);

    /*
     * 设置目标的语言系统
     */
    void set_language(const std::string&);

    /*
     * 初始化语言系统
     */
    void InitLanguage();

private:
    static Language* _pLanguage;
    Language();
    // 重新加载语言
    void reload();
    /*
     * 重新加载指定的语言
     */
    void reload(const std::string&);
    // 存取模块
    WriteWithLine _writeLine;
    // 是否以及被初始化过
    bool is_inited = false;
    Log* log;
    Setting*setting;
};