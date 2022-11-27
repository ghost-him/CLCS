#pragma once
#include "stdinc.h"
#include "User.h"
#include "Log.h"
#include "Language.h"
/*
 * 用于自身的管理
 * 用于服务器对象的管理
 * 单例模式， 只有一个管理器对象
 */
class User_Manager {
public:
    static std::shared_ptr<User_Manager> ptr();
    /*
     * 初始化用户管理器
     * 如果没有相应的储存文件， 则创建一个新的文件
     * 包括为自己创建一个新的uuid
     *
     */
    void Init_User_Manager();
    /*
     * 获取自己的uuid
     */
    static std::string get_self_uuid();
    /*
     * 获取自己的公钥路径和私钥路径
     */
    static std::string get_self_pri_path();
    static std::string get_self_pub_path();
    /*
     * 获取服务器的uuid
     */
    static std::string get_server_uuid();
    /*
     * 设置服务器的uuid
     */
    static void set_server_uuid(const std::string &);
    /*
     * 通过uuid来获取公钥路径
     */
    std::string get_pub_key_path(const std::string &);
    /*
     * 通过路径来获取公钥的内容
     */
    std::string get_key_content(const std::string &);

    /*
     * 快速获取指定的用户
     */
    User* operator[](std::string &);
    /*
     * 添加用户
     */
    void add_user(User&);
    /*
     * 删除用户
     */
    void delete_user(std::string&);
    /*
     * 查找用户
     */
    User* find_user(const std::string &);
    /*
     * 重命名用户
     */
    bool rename(const std::string&, const std::string&);
    /*
     * 获取数据库
     */
    const std::map<std::string, User>& get_database();
    /*
     * 保存数据
     */
    void save();
    /*
     * 判断是不是一个uuid
     */
    static bool check_is_uuid(const std::string&);

    const static std::string empty_uuid;
private:
    static std::shared_ptr<User_Manager> _ptr;

    User_Manager(){};


    // 自身
    static User self;
    static User server;

    // 通过uuid查找用户
    std::map<std::string, User> _user_store;
    // 通过昵称查找uuid
    std::map<std::string, std::string> _uuid_store;

    // 日志系统
    std::shared_ptr<Log> log;
    // 设置系统
    std::shared_ptr<Setting> setting;
    // 文件管理器
    std::shared_ptr<FileManager> fm;
};

