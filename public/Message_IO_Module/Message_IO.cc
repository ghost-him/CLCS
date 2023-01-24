#include "Message_IO.h"

std::mutex STREAM::lock;
std::shared_ptr<OUT> OUT::_ptr(new OUT());
std::shared_ptr<IN> IN::_ptr(new IN());

std::shared_ptr<OUT> OUT::ptr() {
    return _ptr;
}

OUT::OUT() {
    std::cout << "CLCS >  " << std::flush;
}

OUT &OUT::print_string(std::string && content) {
    lock.lock();
    for (int i = 0; i < content.size(); i++) {
        std::cout << content[i];
        if (content[i] == '\n') {
            std::cout << "CLCS >  ";
        }
    }
    std::cout << "\nCLCS >  " << std::flush;
    lock.unlock();
    return *this;
}


void OUT::print() {
    // 如果已经阻塞输入了
    if (Global_Option::is_block_output)
        return ;

    while (!_store.empty()) {
        std::string tar = std::move(_store.front());
        _store.pop_front();
        // 输出
        print_string(std::move(tar));
    }
}

std::shared_ptr<IN> IN::ptr() {
    return _ptr;
}

void IN::check_and_fix() {
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
    }
}

