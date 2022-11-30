#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include <memory>

/*
 * 普通的输出
 */

class STREAM {
public:
    STREAM() = default;
protected:
    static std::mutex lock;
};

class OUT : public STREAM{

public :
    static std::shared_ptr<OUT> ptr();

    // 输出字符串
    OUT &operator<<(const char * content) {
        return print_string(content);
    }

    // 输出字符串
    OUT &operator<<(const std::string & content) {
        return print_string(content);
    }

    // 输出指定的信息
    template<class var>
    OUT &print(var *content, size_t len) {
        lock.lock();
        write(STDOUT_FILENO, content, len);
        std::cout << "\nCLCS >  " << std::flush;
        lock.unlock();
        return *this;
    }

private:
    static std::shared_ptr<OUT> _ptr;
    OUT();

    OUT& print_string(const std::string &);
};

// 可以通过智能指针来输出
template<class var>
OUT &operator<<(const std::shared_ptr<OUT>& out, var&& content) {
    return (*out) << content;
}


// 可以通过智能指针来输出
template<class var>
OUT &operator<<(const std::shared_ptr<OUT>& out, var content[]) {
    return (*out) << content;
}


class IN : public STREAM{

public :
    static std::shared_ptr<IN> ptr();

    // 输入到一个变量中
    template<class var>
    void operator>>(var && content) {
        std::cin >> content;
        check_and_fix();
        lock.lock();
        std::cout << "CLCS >  " << std::flush;
        lock.unlock();
    }


    // 输入一行数据到字符串中
    template<class var>
    void get_line(var && content) {
        getline(std::cin, content);
        check_and_fix();
        lock.lock();
        std::cout << "CLCS >  " << std::flush;
        lock.unlock();
    }

private:

    static std::shared_ptr<IN> _ptr;
    IN() = default;

    void check_and_fix();
};

// 可以通过智能指针来输入
template<class var>
void operator>>(const std::shared_ptr<IN>& in, var&& content) {
    (*in) >> content;
}