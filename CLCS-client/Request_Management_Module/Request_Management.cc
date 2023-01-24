#include "Request_Management.h"

std::shared_ptr<Request_Manager> Request_Manager::_ptr(new Request_Manager);

void Request::set_uuid(const std::string& uuid) {
    _uuid = uuid;
}

std::string Request::get_uuid() {
    return _uuid;
}

void Request::set_accept_function(const std::function<void()> & function) {
    _function_accept = function;
}

void Request::set_refuse_function(const std::function<void()> & function) {
    _function_refuse = function;
}

void Request::execute_accept() {
    _function_accept();
}

void Request::execute_refuse() {
    _function_refuse();
}

std::shared_ptr<Request_Manager> Request_Manager::ptr() {
    return _ptr;
}

std::string Request_Manager::create(const std::function<void()> & accept, const std::function<void()> & refuse) {
    // 生成一个新的请求模块
    std::shared_ptr<Request> one = std::make_shared<Request>();
    // 设置要执行的函数
    one->set_accept_function(accept);
    one->set_refuse_function(refuse);

    // 生成一个uuid
    uuid_t uuid;
    uuid_generate(uuid);
    char uuid_c[UUID_STR_LEN];
    uuid_unparse(uuid, uuid_c);
    // 设置这个请求的uuid
    one->set_uuid(uuid_c);

    // 开启锁
    std::lock_guard<std::mutex> guard(lock);
    // 放到当前的存储器中
    _request_store.emplace(one->get_uuid(), one);
    return uuid_c;
}

void Request_Manager::accept(const std::string & uuid) {
    // 如果存在这个请求
    if (_request_store.contains(uuid)) {
        // 读取请求
        auto ptr = _request_store[uuid];
        // 执行同意的函数
        ptr->execute_accept();
        // 从储存器中删除
        _request_store.erase(ptr->get_uuid());
    }
}

void Request_Manager::refuse(const std::string & uuid) {
    // 如果存在这个请求
    if (_request_store.contains(uuid)) {
        // 读取请求
        auto ptr = _request_store[uuid];
        // 执行拒绝的函数
        ptr->execute_refuse();
        // 从储存器中删除
        _request_store.erase(ptr->get_uuid());
    }
}
