#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include <memory>
#include <deque>
#include <Global_Variables/Default.h>

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
    OUT &operator<<(const std::string & content) {
        _store.emplace_back(content);
        print();
        return *this;
    }

private:
    static std::shared_ptr<OUT> _ptr;
    OUT();
    /*
     * 输出一个字符串
     */
    OUT& print_string(std::string &&);
    /*
     * 开始输出字符串
     */
    void print();

    // 如果阻塞了输出，则输出到此
    std::deque<std::string> _store;
};

//// 可以通过智能指针来输出
template<class var>
OUT &operator<<(const std::shared_ptr<OUT>& out, const var& content) {
    (*out) << content;
    return *out;
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