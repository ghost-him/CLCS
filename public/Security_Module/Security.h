#pragma once
#include "stdinc.h"
#include "Logger_Module/Log.h"
#include "File_Manager_Module/File_Manager.h"
#include "Language_Module/Language.h"

class Language;
/*
 * 用于生成RSA密匙
 */
class RSA_controller {
public:
    RSA_controller();
    ~RSA_controller();
    /*
     * 设置私钥路径
     */
    void set_private_key_path(const std::string &);
    /*
     * 设置公钥路径
     */
    void set_public_key_path(const std::string &);
    /*
     * 生成密钥
     */
    bool generate_keys(int len = 2048);
private:
    // 公钥路径
    std::string _pub_path;

    // 私钥路径
    std::string _pri_path;

    // 日志系统
    std::shared_ptr<Log> log;

    // 语言系统
    std::shared_ptr<Language> lang;

};

/*
 * RSA加密与解密的父类
 */
class RSA_Security {
public:
    /*
     * 构造函数
     */
    RSA_Security();
    /*
     * 析构函数
     */
    virtual ~RSA_Security();
    /*
     * 开始初始化
     */
    virtual void startInit();
    /*
     * 初始化为最开始的状态
     */
    void init();

    /*
     * 读取密匙
     */
    virtual bool read_key() = 0;
    /*
     * 设置密匙的路径
     */
    virtual bool set_key_path(const std::string &) = 0;
    /*
     * 设置并读取
     */
    virtual bool set_and_read_key(const std::string &);
    /*
     * 执行转换
     */
    virtual bool convert() = 0;

    /*
     * 获取密钥支持的长度
     */
    virtual size_t size() = 0;

    // 获取错误信息
    std::string get_error() { return error;};

protected:
    // 当前的密匙
    EVP_PKEY *_pkey;
    // 密匙路径
    std::string _key_path;
    // 日志系统
    std::shared_ptr<Log> log;
    // 信息系统
    std::shared_ptr<Language> lang;
    // 错误信息
    std::string error;
};

/*
 * 加密模块
 */
class RSA_encrypt : public RSA_Security {
public:
    RSA_encrypt();
    /*
     * 初始化
     */
    void startInit() override;
    /*
     * 读取公钥
     */
    bool read_key() override;
    /*
     * 读取公钥的路径
     */
    bool set_key_path(const std::string &) override;
    /*
     * 加密信息
     */
    bool convert() override;

    /*
     * 获取数组的长度
     */
    size_t size() override;
    /*
     * 设置被加密的信息
     */
    void set_input(const std::string&);
    /*
     * 设置被加密的信息
     */
    void set_input(std::shared_ptr<char[]>&, size_t);
    /*
     * 获取得到的加密信息
     */
    std::shared_ptr<unsigned char[]> get_output();
private:
    // 输入的字符
    std::shared_ptr<char[]> _input_text;
    // 输入的字符的长度
    size_t _input_len;
    // 输出的字符
    std::shared_ptr<unsigned char[]>  _output_text;
    // 输出的字符的长度
    size_t _output_len;
};
/*
 * 解密模块
 */
class RSA_decrypt : public RSA_Security {
public:
    RSA_decrypt();

    void startInit() override;
    /*
     * 读取私钥
     */
    bool read_key() override;
    /*
     * 设置私钥的路径
     */
    bool set_key_path(const std::string &) override;
    /*
     * 将加密的信息解密
     * TODO 当信息解密失败的时候， 则重新向目标用户发送公匙
     */
    bool convert() override;

    size_t size() override;

    /*
     * 设置输入的字符串
     */
    void set_input(const unsigned char*, size_t);
    void set_input(std::shared_ptr<unsigned char[]>& str, size_t len);
    /*
     * 获取得到的解密信息
     */
    std::shared_ptr<char[]> get_output();

private:
    // 输入的字符
    std::shared_ptr<unsigned char[]> _input_text;
    // 输入的长度
    size_t _input_len;
    // 输出的字符
    std::shared_ptr<char[]> _output_text;
    // 输出的长度
    size_t _output_len;

};

