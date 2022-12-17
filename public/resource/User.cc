#include "User.h"

User::User() {
    fd = 0;
}

User::User(uuid_t& uuid) {
    uuid_unparse(uuid, _uuid);
    memcpy(_name, _uuid, 32);
    _name[32] = '\0';
    fd = 0;
}

const char* User::get_uuid() const {
    return _uuid;
}

void User::set_uuid(uuid_t & uuid) {
    uuid_unparse(uuid, _uuid);
}

void User::set_uuid(const std::string &uuid) {
    memcpy(_uuid, uuid.c_str(), uuid.size());
    _uuid[36] = '\0';
}

const char* User::get_name() const {
    return _name;
}

void User::set_name(const char * str) {
    int len = strlen(str);
    len = std::min(len, 32);
    memcpy(_name, str, len);
    _name[len] = '\0';
}

void User::set_name(const std::string& str) {
    set_name(str.c_str());
}


bool User::operator==(User& user) {
    return !strcmp(_uuid, user._uuid);
}

void User::set_pub_path(const std::string & path) {
    _pub_path = path;
}

void User::set_pri_path(const std::string & path) {
    _pri_path = path;
}

std::string User::get_pub_path() {
    return _pub_path;
}

std::string User::get_pri_path() {
    return _pri_path;
}

const User & User::operator=(const User& user) {
    strncpy(_uuid, user._uuid, sizeof(_uuid));
    strncpy(_name, user._name, sizeof(_name));
    fd = user.fd;
    _pub_path = user._pub_path;
    _pri_path = user._pri_path;
    return *this;
}
