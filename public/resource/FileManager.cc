#include "FileManager.h"

void ReadFile::setFilePath(const std::string &str) {
    _file_path = str;
}

std::string ReadFile::getFilePath() const {
    return _file_path;
}

bool ReadFile::is_error() const {
    return _is_exist_error;
}

void ReadJson::read_json() {
    std::ifstream ifs;
    ifs.open(_file_path);
    if(ifs.is_open()) {
        // 如果打开文件
        _is_exist_error = false;
        // 读取文件
        ifs >> (*_json);
    } else {
        // 如果没有打开文件
        _is_exist_error = true;
    }
    ifs.close();
}

void ReadJson::set_json_ptr(nlohmann::json* json) {
    _json = json;
}

std::pair<User, bool> ReadUser::read_user_message(std::string && user_massage) {
    std::pair<User, bool> user;
    std::stringstream ss(user_massage);
    std::string str;
    // 读取uuid
    if (ss.eof()) {
        user.second = false;
        return user;
    }
    ss >> str;
    user.first.set_uuid(str);
    // 读取公共路径信息
    if (ss.eof()) {
        user.second = false;
        return user;
    }
    ss >> str;
    user.first.set_pub_path(str);
    // 读取昵称
    if (ss.eof()) {
        user.second = true;
        return user;
    }
    ss >> str;
    user.first.set_name(str);
    user.second = true;
    return user;
}



void WriteFile::setFilePath(const std::string &str) {
    _file_path = str;
}

std::string WriteFile::getFilePath() const {
    return _file_path;
}

bool WriteFile::is_error() const {
    return _is_exist_error;
}

void WriteJson::set_json_to_write(nlohmann::json *json_ptr) {
    _json_ptr = json_ptr;
}

void WriteJson::write_json() {
    std::ofstream ofs;
    if (_file_path.empty()) {
        Log::ptr->log("[warn] invalid path: " + _file_path);
        return ;
    }

    ofs.open(_file_path, std::ios::trunc | std::ios::out);

    if (ofs.is_open()) {
        ofs << (*_json_ptr).dump(4);
    } else {
        Log::ptr->log("[warn] can not open the target path: " + _file_path);
    }

    ofs.close();
}

bool WriteUser::write_file(std::map<std::string, User> & user_store) {
    auto log = Log::ptr;
    std::ofstream ofs;
    ofs.open(_file_path, std::ios::out | std::ios::trunc);
    if (!ofs.is_open()) {
        log->log("[error] WriteUser: write_file error: can not open the target: " + _file_path);
        return false;
    }
    // 一个一个的写如文件
    for (auto& i : user_store) {
        ofs << get_user_message(i.second) << "\n";
#ifdef IS_DEBUG
        std::cerr << "write user message to file " << get_user_message(i.second) << std::endl;
#endif
    }
    // 关闭文件
    ofs.close();
    return true;
}



std::string WriteUser::get_user_message(User & user) {
    std::string res;
    res += user.get_uuid();
    res += " ";
    res += user.get_pub_path();
    res += " ";
    res += user.get_name();
    return std::move(res);
}

void CreateFile::set_target_path(const std::string &path) {
    _file_path = path;
}

void CreateJson::create_target() {
    std::ofstream ofs;
    ofs.open(_file_path);

    // 检查文件是否已经打开了
    if (ofs.is_open()) {
        ofs << _json;
    } else {
        Log::ptr->log("[error] createJson: invalid path:" + _file_path);
    }
    // 关闭文件
    ofs.close();
}

void CreateJson::set_json(const nlohmann::json &json) {
    _json = json;
}

FileManager* FileManager::ptr = new FileManager;

FileManager::FileManager() {
    // 设置默认值
    _dir_path["default_path"] = "";
}

std::string& FileManager::get(const std::string& dir_path) {
    if (_dir_path.count(dir_path) == 0) {
        Log::ptr->log("[warn] %c target file not exist: " + dir_path);
        return _dir_path["default_path"];
    }
    return _dir_path[dir_path];
}


