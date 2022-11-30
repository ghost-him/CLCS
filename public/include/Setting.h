#pragma once
#include "stdinc.h"
#include "File_Manager.h"
#include "Default.h"
#include "Log.h"
#include "Security.h"
#include "Setting.h"


class Setting {
public:
    static std::shared_ptr<Setting> ptr();

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
    static std::shared_ptr<Setting> _ptr;
    static nlohmann::json _json;
    static std::shared_ptr<File_Manager> _FileManager;
    // static ReadWithLine _readFile;
    static Read_Json _read_json;
    // static WriteWithLine _writeFile;
    static Write_Json _write_json;
    static std::shared_ptr<Log> log;
};


