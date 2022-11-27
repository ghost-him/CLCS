#pragma once
#include "stdinc.h"

/*
 * 用于输出日志
 * 单例设计模式，饿汉式，多线程安全
 *
 */


class Log final {
public:
    /*
     * 获取唯一的实例
     */
    static std::shared_ptr<Log> ptr();

    ~Log();

    /*
     * 输出log日志
     */
    void log(const std::string& str);

    /*
     * 获取目标的文件路径
     */
    std::string getFilePath() const;

    /*
     * 设置目标文件的路径，并且将输出重定位到它
     * 当路径名为 console的时候， 才会将log信息输出到控制台
     */
    void setFilePath(const std::string& file_path);

    /*
     * 获取当前的输出目标文件
     */
    const std::ostream& getTarget() const;

    /*
     * 输出当前的输出对象是否是控制台
     */
    bool isConsole() const;

    void exit_process();

private:
    static std::shared_ptr<Log> _ptr;
    // 用于锁住文件输出流
    pthread_mutex_t _stream_lock;
    Log();

    // 输出的位置
    std::ostream& _ost;
    std::ofstream _ofs;

    // 当前的文件路径
    std::string _file_path;

    // 当前输出的是否是控制台
    bool _is_console;

    // 记录程序刚开始运行的事件
    std::chrono::steady_clock::time_point _start_time = std::chrono::steady_clock::now();
};

