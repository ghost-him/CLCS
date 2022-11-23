#include "Setting.h"

nlohmann::json Setting::_json;
Setting *Setting::ptr = new Setting;
FileManager* Setting::_FileManager = nullptr;
Log* Setting::log = nullptr;
// ReadWithLine Setting::_readFile;
//WriteWithLine Setting::_writeFile;
ReadJson Setting::_read_json;
WriteJson Setting::_write_json;


void Setting::startInit() {
    log = Log::ptr;
    _FileManager = FileManager::ptr;
    // 获取当前的文件的路径
    std::string path = _FileManager->get("option") + "option.json";

    // 设置文件的保存 和 读取的路径
    _write_json.setFilePath(path);
    _read_json.setFilePath(path);
    // 设置读写的json
    _write_json.set_json_to_write(&_json);
    // 如果不存在文件，则创建文件
    if (access(path.c_str(), F_OK) == -1) {
        log->log("[warn] setting sys: can not find option file, %e\n");

        // _json 指向初始的json
        _json = DefaultOption::store;

        // 第一次运行程序
        first_time_run();
        _write_json.write_json();
        _read_json.setFilePath(path);
    } else {
        // 已经存在json文件了
        // 将json_ptr指向读取出来的 json 文件

        _read_json.set_json_ptr(&_json);
        _read_json.read_json();


        // 验证读到的json文件是不是完整的大小
        if (_json.size() != DefaultOption::store.size()) {
            log->log("[error] setting sys: Incorrect number of entries\n");
            log->exit_process();
        }
    }
}

std::string Setting::operator[](const std::string &str) {
    if (_json.contains(str)) {
        return (_json[str]).get<std::string>();
    }
    else {
        log->log("[warn] error para: " + str);
        return "str";
    }
}

void Setting::set(const std::string& key, const std::string & value) {
    if (_json.contains(key))
        _json[key] = value;
    else {
        Log::ptr->log("[error] Setting: error key not exist");
    }
}

void Setting::save_setting() {
    _write_json.write_json();
    Log::ptr->log("[info] Setting setting save success!");
}

Setting::Setting() {

}

