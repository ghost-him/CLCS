#pragma once
#include "stdinc.h"
#include "Logger_Module/Log.h"
#include "File_Manager_Module/File_Manager.h"
#include "Global_Variables/Default.h"
#include "Initializer_Module/Setting.h"
/*
 * 设计模式：单例模式， 用于管理语言
 */
class Setting;

class Language {
public:
    /*
     * 获取唯一的实例
     */
    static std::shared_ptr<Language> ptr();
    /*
     * 获取指定字符串的ip
     */
    const std::string operator[](const std::string&);

    /*
     * 设置目标的语言系统
     */
    void set_language(const std::string&);

    /*
     * 初始化语言系统
     */
    void InitLanguage();

private:
    static std::shared_ptr<Language> _ptr;

    Language();
    // 重新加载语言
    void reload(const std::string& lang = "zh_CN");
    // 存取模块
    // WriteWithLine _writeLine;
    nlohmann::json _json_ptr;
    // 读取 json 文件
    Read_Json _read_json;
    // 是否以及被初始化过
    bool is_inited = false;
    std::shared_ptr<Log> log;
    std::shared_ptr<Setting> setting;
};