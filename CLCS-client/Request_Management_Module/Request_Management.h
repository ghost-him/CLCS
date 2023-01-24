#pragma once
#include "stdinc.h"

class Request {
public :
    /*
     * 设置uuid
     */
    void set_uuid(const std::string&);
    /*
     * 获取uuid
     */
    std::string get_uuid();
    /*
     * 设置同意时要执行的函数
     */
    void set_accept_function(const std::function<void()> &);
    /*
     * 执行同意时的函数
     */
    void execute_accept();

    /*
     * 设置失败时要执行的函数
     */
    void set_refuse_function(const std::function<void()> &);
    /*
     * 执行失败时的函数
     */
    void execute_refuse();

private:
    // 当前请求的编号
    std::string _uuid;
    // 成功时要执行的函数
    std::function<void()> _function_accept;
    // 失败时要执行的函数
    std::function<void()> _function_refuse;
};

/*
 * 请求管理器
 * 用于管理发送的请求
 */
class Request_Manager {
public :
    /*
     * 单例模式
     */
    static std::shared_ptr<Request_Manager> ptr();
    /*
     * 创建一个请求
     * 传入的值：同意时执行的函数和失败时执行的函数
     * 返回值是当前请求的编号
     */
    std::string create(const std::function<void()> &, const std::function<void()> &);

    /*
     * 接受一个请求
     */
    void accept(const std::string &);

    /*
     * 拒绝一个请求
     */
    void refuse(const std::string &);

private:
    Request_Manager() = default;
    static std::shared_ptr<Request_Manager> _ptr;
    // 用于存储发送的请求
     std::map<std::string, std::shared_ptr<Request>> _request_store;
    // 锁，使多线程安全
    std::mutex lock;
};
