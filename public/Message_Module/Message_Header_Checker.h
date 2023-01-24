#pragma once
#include "stdinc.h"
#include "Message.h"

struct valid_header {
    char level;
    char option1;
    char option2;
    bool operator<(const valid_header& t) const {
        return level < t.level;
    }
};

class Message_Header_Checker {
public:
    /*
     * 检查是否有效
     */
    static bool check(valid_header &&);

private:
    Message_Header_Checker();

    static std::set<valid_header> store;
};
