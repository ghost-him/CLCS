#include "Setting.h"

Setting *Setting::_pSetting = new Setting;
FileManager* Setting::_FileManager = nullptr;
Log* Setting::log = nullptr;
ReadWithLine Setting::_readFile;
WriteWithLine Setting::_writeFile;

Setting *Setting::getInstance() {
    return _pSetting;
}

void Setting::InitSetting() {
    log = Log::getInstance();
    _FileManager = FileManager::getInstance();
    std::string path = _FileManager->get("option") + "option.txt";
    _readFile.setFilePath(path);
    // 如果不存在文件，则创建文件
    if (access(path.c_str(), F_OK) == -1) {
        log->log("[warn] setting sys: can not find option file, %e\n");
        _writeFile.setFilePath(path);
        _writeFile._store = DefaultOption::store;
        first_time_run();
        _writeFile.writeFile();
    } else {
        _readFile.readFile(_writeFile._store);
        if (_writeFile._store.size() != DefaultOption::store.size()) {
            log->log("[error] setting sys: Incorrect number of entries\n");
            log->exit_process();
        }
    }
}

std::string &Setting::operator[](const std::string &str) {
    return _writeFile[str];
}

void Setting::save_setting() {
    _writeFile.writeFile();
}

Setting::Setting() {

}


