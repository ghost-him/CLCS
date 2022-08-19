#pragma once
#include "stdinc.h"
#include "FileManager.h"
#include "Default.h"
#include "Log.h"
#include "Security.h"
#include "Setting.h"


class Setting {
public:
    static Setting* getInstance();

    /*
     * 初始化输入
     */
    void InitSetting();
    /*
     * 初始化设置
     * 程序第一次运行的时候执行（即找不到设置文件）
     */
    void first_time_run();

    std::string& operator[](const std::string &);

    // 保存设置
    void save_setting();

private:
    Setting();
    FileManager* _FileManager;
    static Setting* _pSetting;
    ReadWithLine _readFile;
    WriteWithLine _writeFile;
    Log* log;
};


