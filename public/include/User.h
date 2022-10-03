#pragma once
#include "stdinc.h"

/*
 * 用户 ，既可以是当前使用该软件的用户， 也可以是目标连接的用户
 * 描述了用户的一系列属性
 */

class User {
public:
    User();
    User(uuid_t&);
    /*
     * 获取uuid
     */
    const char* get_uuid() const;
    /*
     * 设置uuid
     */
    void set_uuid(uuid_t &);
    void set_uuid(const std::string &);
    /*
     * 获取昵称
     */
    const char* get_name() const;
    /*
     * 设置昵称
     */
    void set_name(const char *);
    void set_name(const std::string&);
    /*
     * 判断两个人是否相等
     */
    bool operator==(User&);
    /*
     * 设置公钥路径
     */
    void set_pub_path(const std::string &);
    /*
     * 设置私钥路径
     */
    void set_pri_path(const std::string &);
    /*
     * 获取公钥路径
     */
    std::string get_pub_path();
    /*
     * 获取私钥路径
     */
    std::string get_pri_path();

    const User & operator=(const User&);
protected:
    // 当前用户的uuid
    char _uuid[37];
    // 当前用户的昵称
    char _name[33];
    // 对应的文件描述符
    int fd;
    // 公钥
    // TODO 设置公钥和私钥
    std::string _pub_path;
    std::string _pri_path;

};