#include "Language.h"

std::shared_ptr<Language> Language::_ptr(new Language);

std::shared_ptr<Language> Language::ptr() {
    return _ptr;
}

const std::string Language::operator[](const std::string& str) {
    if (_json_ptr.contains(str)) {
        return _json_ptr[str];
    }
    // 如果找不到
    log->log("[warn] language system cannot find target: " + str + "\n");
    return str;
}

void Language::set_language(const std::string & str) {
    (*setting)["sys_language"] = str;
}


void Language::InitLanguage() {
    log = Log::ptr();
    setting = Setting::ptr();
    log->log("[info] language sys: init the language system\n");
    // 该函数不论执行多少次， 只会运行一次
    if (is_inited)
        return ;
    is_inited = true;

    reload((*setting)["sys_language"]);
}



Language::Language(){

}

void Language::reload(const std::string& lang) {
    // 检查目标语言对应文件是否存在

    std::string target_path = File_Manager::ptr()->get("language_path") + lang;
    if (access(target_path.c_str(), F_OK) == -1) {
        // 如果语言不存在， 则创建一个新的文件
        log->log("[error] language system: file not exist: %e");
        _json_ptr = Global_Option::language;
        Create_Json json_creater;
        json_creater.set_json(&_json_ptr);
        json_creater.set_target_path(target_path);
        json_creater.create_target();
        return ;
    }
    // 文件存在

    // 读文件
    // ReadWithLine rd;
    _read_json.setFilePath(target_path);
    _read_json.set_json_ptr(&_json_ptr);
    _read_json.read_json();

    // 检查读到的数量和预设的数量是否一致
    if (_json_ptr.size() != Global_Option::language.size()) {
        log->log("[error] language sys: Incorrect number of entries\n");
        log->exit_process();
    }
}


