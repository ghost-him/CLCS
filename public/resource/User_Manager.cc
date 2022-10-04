#include "User_Manager.h"

User_Manager* User_Manager::ptr = new User_Manager;
User User_Manager::self;
User User_Manager::server;
const std::string User_Manager::empty_uuid = "00000000-0000-0000-0000-000000000000";

// TODO 初始化user_manager
void User_Manager::Init_User_Manager() {
    /*
     * 1. 初始化自身的user
     * 2. 初始化维护的用户组
     */
    setting = Setting::ptr;
    log = Log::ptr;
    fm = FileManager::ptr;
    self.set_pub_path((*setting)["pub_key_path"]);
    self.set_pri_path((*setting)["pri_key_path"]);
    self.set_uuid((*setting)["uuid"]);

    // 读取用户信息
    ReadUser ru;
    std::string file_path = (*fm).get("option") + "user.txt";
    std::ifstream ifs;
    if (access(file_path.c_str(), F_OK) == -1) {
        log->log("[error] User_Manager: can not find target file:" + file_path);
        int ret = creat(file_path.c_str(), 0700);
        if (ret < 0) {
            log->log("[error] User_Manager: can not create new file: %e");
            return;
        }
        close(ret);
        return;
    }

    ifs.open(file_path, std::ios::in);
    if (!ifs.is_open()) {
        log->log("[error] ReadUser: can not open target file: " + file_path);
        return;
    }

    std::string user_message;
    std::pair<User, bool> user;
    // 如果有数据的时候
    while (!ifs.eof()) {
        std::getline(ifs, user_message);
        user = std::move(ru.read_user_message(std::move(user_message)));
        if (!user.second) {
            log->log("[warn] ReadUser: invalid user message");
            continue;
        }
        // 添加用户
        add_user(user.first);
    }
}

std::string User_Manager::get_self_uuid() {
    return self.get_uuid();
}

std::string User_Manager::get_self_pri_path() {
    return self.get_pri_path();
}

std::string User_Manager::get_self_pub_path() {
    return self.get_pub_path();
}

std::string User_Manager::get_server_uuid() {
    return server.get_uuid();
}

void User_Manager::set_server_uuid(const std::string & uuid) {
    server.set_uuid(uuid);
}

std::string User_Manager::get_pub_key_path(const std::string & uuid) {
    std::string res;
    if (_user_store.count(uuid) == 0) {
        log->log("User_Manager: new uuid");
        return res;
    }
    return _user_store[uuid].get_pub_path();
}

std::string User_Manager::get_key_content(const std::string & path) {
    std::string res;
    char buf[BUFSIZ];
    ssize_t ret = 0;
    int fd = open(path.c_str(), O_RDONLY);
    if (fd == -1) {
        log->log("[error] User_Manager open file: " + path + " failed");
        return res;
    }
    while ((ret = read(fd, buf, BUFSIZ)) > 0) {
        buf[ret] = '\0';
        res += buf;
    }
    if (ret == -1) {
        log->log("[error] User_Manager read file: " + path + "error, %e");
        return "";
    }
    return res;
}

User* User_Manager::operator[](std::string & uuid) {
    return find_user(uuid);
}

void User_Manager::add_user(User& user) {
    _user_store.emplace(user.get_uuid(), user);
    _uuid_store.emplace(user.get_name(), user.get_uuid());
    log->log((std::string)"[info] User_Manager: add the user, uuid: " + user.get_uuid() + " name: " + user.get_name());
}

void User_Manager::delete_user(std::string & uuid) {
    // 无当前的用户
    if (_user_store.count(uuid) == 0) {
        return ;
    }
    std::string name = _user_store[uuid].get_name();
    _user_store.erase(uuid);
    _uuid_store.erase(name);
    log->log("[info] User_Manager: erase the user, uuid: " + uuid + " name: " + name);
}

User* User_Manager::find_user(const std::string & name) {
    std::string uuid;
    // 如果输入的是名字， 则返回uuid
    if (_uuid_store.count(name) != 0) {
        uuid = _uuid_store[name];
    } else {
        uuid = name;
    }

    // 查看uuid来获取用户信息
    if (_user_store.count(uuid) != 0) {
        return &_user_store[uuid];
    }
    log->log("[warn] User_Manager: can not find user: " + name);
    return nullptr;
}

bool User_Manager::rename(const std::string & user, const std::string& name) {
    User* target = find_user(user);
    if (target == nullptr) {
        return false;
    }

    _uuid_store.erase(target->get_name());
    target->set_name(name);
    _uuid_store.emplace(target->get_name(), target->get_uuid());
    return true;
}

const std::map<std::string, User>& User_Manager::get_database() {
    return _user_store;
}

void User_Manager::save() {
    WriteUser wf;
    wf.setFilePath((*fm).get("option") + "user.txt");
    wf.write_file(_user_store);
}

bool User_Manager::check_is_uuid(const std::string &uuid) {
    return uuid.size() == empty_uuid.size() &&
           uuid[8] == '-' &&
           uuid[13] == '-' &&
           uuid[18] == '-' &&
           uuid[23] == '-';
}