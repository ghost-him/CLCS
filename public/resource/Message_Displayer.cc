#include "Message_Displayer.h"

std::mutex STREAM::lock;
std::shared_ptr<OUT> OUT::_ptr(new OUT());
std::shared_ptr<IN> IN::_ptr(new IN());

std::shared_ptr<OUT> OUT::ptr() {
    return _ptr;
}

OUT::OUT() {
    std::cout << "CLCS > " << std::flush;
}

OUT &OUT::print_string(const std::string & content) {
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


std::shared_ptr<IN> IN::ptr() {
    return _ptr;
}

void IN::check_and_fix() {
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
    }
}

