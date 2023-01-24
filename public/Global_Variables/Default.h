#pragma once
#include <atomic>
#include <nlohmann/json.hpp>
#include <mutex>

class Global_Option {
public:
    static const nlohmann::json store;
    static const nlohmann::json language;
    // 当前程序是否在群组中
    static std::atomic<bool> is_in_group;
    // 全局锁
    static std::mutex input_lock;
    // 是否停止了输出
    static std::atomic<bool> is_block_output;
};