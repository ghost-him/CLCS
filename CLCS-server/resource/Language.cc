#include "Language.h"

Language* Language::_pLanguage = new Language;

Language *Language::getInstance() {
    return _pLanguage;
}

const std::string& Language::operator[](const std::string& str) {
    // 如果找不到目标
    if (_writeLine._store.count(str) == 0) {
       log->log("[warn] language system cannot find target: " + str + "\n");
    }
    return _writeLine[str];
}

void Language::set_language(const std::string & str) {
    (*setting)["sys_language"] = str;
}


void Language::InitLanguage() {
    log = Log::getInstance();
    setting = Setting::getInstance();
    log->log("[info] language sys: init the language system\n");
    // 该函数不论执行多少次， 只会运行一次
    if (is_inited)
        return ;
    is_inited = true;

    reload((*setting)["sys_language"]);
}



Language::Language(){

}

void Language::reload() {
    reload("zh_CN");
}

void Language::reload(const std::string& lang) {
    // 检查目标语言对应文件是否存在

    std::string target_path = FileManager::getInstance()->get("language_path") + lang;
    if (access(target_path.c_str(), F_OK) == -1) {
        // 如果语言不存在， 则创建一个新的文件
        log->log("[error] language system: file not exist: %e");
        _writeLine.setFilePath(target_path);
        _writeLine._store = DefaultOption::language;
        _writeLine.writeFile();
        return ;
    }
    // 文件存在

    // 清空文件
    _writeLine._store.clear();
    // 读文件
    ReadWithLine rd;
    rd.setFilePath(target_path);
    rd.readFile(_writeLine._store);
    // 检查读到的数量和预设的数量是否一致
    if (_writeLine._store.size() != DefaultOption::language.size()) {
        log->log("[error] language sys: Incorrect number of entries\n");
        log->exit_process();
    }
}


