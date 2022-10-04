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

void ReadWithLine::readFile(std::unordered_map<std::string, std::string> &target) {
    std::ifstream ifs;
    ifs.open(_file_path, std::ios::in);
    if (!ifs.is_open()) {
        _is_exist_error = true;
        Log::ptr->log("[warn] MessageGenerator: message level not exist :" + _file_path);
        return ;
    }
    _is_exist_error = false;

    std::string line;
    while (!ifs.eof()) {
        std::getline(ifs, line);
        bool is_valid = true;           // 当前的行是否合法
        int mid = line.find_first_of(':');
        int start = line.find_first_of('"');
        int end = line.find_last_of('"');
        std::string a, b;
        do{
            // 当缺少 : 或 " 的时候
            if (mid == -1 || start == -1 || end == -1) {
                is_valid = false;
                break;
            }
            if (mid - start - 2 <= 0 || end - mid - 2 <= 0) {
                is_valid = false;
                break;
            }
            a = line.substr(start + 1, mid - start - 2);
            b = line.substr(mid + 2, end - mid - 2);

            if (mid + 1 <= line.size() && (line[mid - 1] == ' ' || line[mid + 1] == ' ')) {
                is_valid = false;
                break;
            }

        }while(0);
        if (!is_valid)
            Log::ptr->log("[warn] invalid line");
        else
            target[a] = b;
    }
    ifs.close();
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

std::string &WriteWithLine::operator[](const std::string & str) {
    if (_store.count(str) == 0) {
        _store[str] = "";
        return _store[str];
    } else {
        return _store[str];
    }
}


void WriteWithLine::writeFile() {
    auto log = Log::ptr;
    std::ofstream ofs;
    if (_file_path.empty()) {
        _is_exist_error = true;
        log->log("[error] WriteWithLine: invalid path" + _file_path + "\n");
        return;
    }

    _is_exist_error = false;

    ofs.open(_file_path, std::ios::trunc | std::ios::out);

    if (!ofs.is_open()) {
        log->log("[error] WriteWithLine: can not open the file");
        return ;
    }

    for (auto& i : _store) {
        ofs << '"' << i.first << "\":\"" << i.second << '"' << std::endl;
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


