#pragma once
#include "stdinc.h"
#include "FileManager.h"
#include "Default.h"
#include "Log.h"
#include "Security.h"
#include "Setting.h"


class Setting {
public:
    static Setting* ptr;

    /*
     * 初始化输入
     */
    static void startInit();
    /*
     * 初始化设置
     * 程序第一次运行的时候执行（即找不到设置文件）
     */
    static std::function<void()> first_time_run;

    std::string operator[](const std::string &);

    static void set(const std::string&, const std::string&);

    // 保存设置
    static void save_setting();

private:
    Setting();
    static nlohmann::json _json;
    static FileManager* _FileManager;
    // static ReadWithLine _readFile;
    static ReadJson _read_json;
    // static WriteWithLine _writeFile;
    static WriteJson _write_json;
    static Log* log;
};

