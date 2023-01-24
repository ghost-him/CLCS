#pragma once
#include "stdinc.h"
#include "Logger_Module/Log.h"
#include "User_Manager_Module/User.h"

/*
 * 一行一行的读取当前的文件
 * 会自动识别注释等
 */

class Read_File {
public:
    /*
     * 设置文件的路径
     * 不检查文件路径的有效性
     */
    void setFilePath(const std::string& str);

    // 获取当前的文件的路径
    std::string getFilePath() const;

    /*
     * 查询是否存在异常
     * 传出参数，传出错误的类型
     */
    bool is_error() const;
protected:
    // 带读取的文件路径
    std::string _file_path;

    // 是否存在异常
    bool _is_exist_error = false;

};

/*
 * 读取一个json文件
 * 获取当前json文件的指针
 */

class Read_Json : public Read_File {
public :
    void read_json();
    void set_json_ptr(nlohmann::json*);
private:
    // nlohmann::json* _json;
    nlohmann::json* _json;
};

/* TODO 完成此函数
 * 读取一个用户
 */
class Read_User : public Read_File {
public:
    // 用户信息， 当前的信息是否有效
    std::pair<User, bool> read_user_message(std::string&&);
};

class Write_File {
public:
    /*
     * 设置文件的路径
     * 不检查文件路径的有效性
     */
    void setFilePath(const std::string& str);

    // 获取当前的文件的路径
    std::string getFilePath() const;

    /*
     * 查询是否存在异常
     * 传出参数，传出错误的类型
     */
    bool is_error() const;
protected:
    // 带读取的文件路径
    std::string _file_path;

    // 是否存在异常
    bool _is_exist_error = false;
};

class Write_Json : public Write_File {
public:
    void set_json_to_write(nlohmann::json*);
    void write_json();
private:
    nlohmann::json* _json_ptr;
};


/* TODO 完成此函数
 * 存放用户
 */
class Write_User : public Write_File {
public:
    bool write_file(std::map<std::string, User>&);
private:
    static std::string get_user_message(User&);
};

class Create_File {
public:
    // 设置目标路径
    void set_target_path(const std::string & path);

    virtual void create_target() = 0;

protected:
    // 目标地址
    std::string _file_path;
};

class Create_Json : public Create_File {
public :
    // 创建目标json
    void create_target() override;
    // 设置要创建的json
    void set_json(nlohmann::json *json);
private:
    nlohmann::json* _json;

};

/*
 * 设计模式：单例模式
 * 用于管理文件路径
 */

class File_Manager {
    friend class Init;
public:
    static std::shared_ptr<File_Manager> ptr();
    /*
     * 提取输入的字符串对应的字符串
     * 若找不到，则返回默认的路径
     */
    std::string& get(const std::string&);

private:
    static std::shared_ptr<File_Manager> _ptr;

    File_Manager();

    std::unordered_map<std::string, std::string> _dir_path;
};