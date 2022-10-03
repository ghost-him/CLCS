#include "Init.h"

bool Init::init_dir(const std::string & str) {
    // 如果找不到
    if (access(str.c_str(), F_OK) == -1) {
        std::cerr << "[warn] " << str << " dir not exist\n";
        std::cerr << "[info] creating " << str << " dir...\n";
        if (mkdir(str.c_str(), 0755) == -1) {
            std::cerr << "[error] create "<< str <<" dir failed: " << strerror(errno) << "\n";
            return false;
        }
    }
    return true;
}