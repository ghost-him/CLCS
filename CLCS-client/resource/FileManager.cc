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
        Log::getInstance()->log("[warn] MessageGenerator: message level not exist :" + _file_path);
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
            Log::getInstance()->log("[warn] invalid line");
        else
            target[a] = b;
    }
    ifs.close();
}

void Write::setFilePath(const std::string &str) {
    _file_path = str;
}

std::string Write::getFilePath() const {
    return _file_path;
}

bool Write::is_error() const {
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
    std::ofstream ofs;
    if (_file_path.empty()) {
        _is_exist_error = true;
        Log::getInstance()->log("[error] ReadWithLine: invalid path" + _file_path + "\n");
        return;
    }

    _is_exist_error = false;

    ofs.open(_file_path, std::ios::trunc | std::ios::out);

    for (auto i : _store) {
        ofs << '"' << i.first << "\":\"" << i.second << '"' << std::endl;
    }

    ofs.close();
}





























FileManager* FileManager::_pFileManager = new FileManager;

FileManager::FileManager() {
    // 设置默认值
    _dir_path["default_path"] = "";
}

FileManager* FileManager::getInstance() {
    return _pFileManager;
}

std::string& FileManager::get(const std::string& dir_path) {
    if (_dir_path.count(dir_path) == 0) {
        Log::getInstance()->log("[warn] %c target file not exist: " + dir_path);
        return _dir_path["default_path"];
    }
    return _dir_path[dir_path];
}
