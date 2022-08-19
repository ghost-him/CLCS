#pragma once
#include "stdinc.h"
#include "Log.h"
#include "FileManager.h"
#include "Language.h"
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
    bool generate_keys(int len = 2048, int bignum = 17);
private:
    // 公钥路径
    std::string _pub_path;

    // 私钥路径
    std::string _pri_path;

    // 日志系统
    Log* log;

    // 语言系统
    Language* lang;

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
     * 开始初始化
     */
    void startInit();
    /*
     * 析构函数
     */
    virtual ~RSA_Security();
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

    virtual int size() = 0;

protected:
    // 当前的密匙
    RSA *_rsa;
    // 密匙路径
    std::string _key_path;
    // 日志系统
    Log* log;
    // 信息系统
    Language* lang;
};

/*
 * 加密模块
 */
class RSA_encrypt : public RSA_Security {
public:
    ~RSA_encrypt();
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
     * 读取被加密的信息，然后进行加密
     */
    void operator=(const char *);
    void operator=(const std::string&);

    /*
     * 强制转换类型
     */
    operator const unsigned char *();

    /*
     * 获取数组的长度
     */
    int size() override;
    /*
     * 设置被加密的信息
     */
    void set_input(const std::string&);
    /*
     * 设置被加密的信息
     */
    void set_input(const char *);
    /*
     * 获取得到的加密信息
     */
    const unsigned char * get_output();
private:
    // 输入的字符
    char _input_text[BUFSIZ];
    // 输出的字符
    unsigned char _output_text[BUFSIZ];
};
/*
 * 解密模块
 */
class RSA_decrypt : public RSA_Security {
public:
    ~RSA_decrypt();
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
    /*
     * 读取被加密的信息，然后进行解密
     */
    void operator=(const unsigned char *);
    /*
     * 强制类型转换
     */
    operator char*();
    operator std::string();

    int size() override;

    void set_input(const unsigned char *, int);
    /*
     * 获取得到的解密信息
     */
    const char * get_output();

private:
    // 输入的字符
    unsigned char _input_text[BUFSIZ];
    // 输出的字符
    char _output_text[BUFSIZ];
};

