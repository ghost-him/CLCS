#pragma once
#include "stdinc.h"
#include "FileManager.h"
#include "Default.h"
#include "Log.h"


class Setting {
public:
    static Setting* getInstance();

    /*
     * 初始化输入
     */
    void InitSetting();
    /*
     * 初始化设置
     *
     */
    void first_time_run();

    std::string& operator[](const std::string &);

private:
    Setting();
    FileManager* _FileManager;
    static Setting* _pSetting;
    ReadWithLine _readFile;
    WriteWithLine _writeFile;
    Log* log;
};


