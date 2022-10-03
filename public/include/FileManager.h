#pragma once
#include "stdinc.h"
#include "Log.h"
#include "User.h"

/*
 * 一行一行的读取当前的文件
 * 会自动识别注释等
 */

class ReadFile {
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

class ReadWithLine : public ReadFile {
public:
    /*
     * 查看当前的一行是否合法,若合法，则读取
     * unordered_map : 相互匹配的字符串
     *
     * 不合法的条件：
     * 1. : 在段首和段尾
     * 2. 单词之间存在空格
     */

    void readFile(std::unordered_map<std::string, std::string>&);
};

/* TODO 完成此函数
 * 读取一个用户
 */
class ReadUser : public ReadFile {
public:
    // 用户信息， 当前的信息是否有效
    std::pair<User, bool> read_user_message(std::string&&);
};

class WriteFile {
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

class WriteWithLine : public WriteFile {
public:
    /*
     * 快速读取或修改某个值
     */
    std::string& operator[](const std::string&);

    /*
     * 查看当前的一行是否合法,若合法，则读取
     */

    void writeFile();
    std::unordered_map<std::string, std::string> _store;
};

/* TODO 完成此函数
 * 存放用户
 */
class WriteUser : public WriteFile {
public:
    bool write_file(std::map<std::string, User>&);
private:
    static std::string get_user_message(User&);
};


/*
 * 设计模式：单例模式
 * 用于管理文件路径
 */

class FileManager {
    friend class Init;
public:
    static FileManager* getInstance();
    /*
     * 提取输入的字符串对应的字符串
     * 若找不到，则返回默认的路径
     */
    std::string& get(const std::string&);

private:
    FileManager();
    static FileManager* _pFileManager;
    std::unordered_map<std::string, std::string> _dir_path;
};