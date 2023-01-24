#include "Log.h"


/*
 * 创建一个对象
 */
std::shared_ptr<Log> Log::_ptr(new Log());

std::shared_ptr<Log> Log::ptr() {
    return _ptr;
}

// TODO 优化此结构， 以方便以后添加新的功能
void Log::log(const std::string& str) {
    std::string answer;
    bool is_in_opt = false;
    for (auto i : str) {
        // 若已经为%模式
        if (is_in_opt) {
            if (i == '%') {
                answer.append(1, '%');
            } else {
                std::string temp;
                switch (i) {    // 查看%后面的字符
                    case 'p':
                        temp = std::to_string(getpid());
                        break;
                    case 't':
                        temp = std::to_string(pthread_self());
                        break;
                    case 'c': {
                        time_t val = time(0);
                        temp = ctime(&val);
                        temp = temp.substr(0, temp.size() - 1);
                        break;
                    }
                    case 'm': {
                        std::chrono::duration<double, std::milli> interval = std::chrono::steady_clock::now() - _start_time;
                        temp = std::to_string(interval.count());
                        break;
                    } case 'e': {
                        temp = strerror(errno);
                        break;
                    }
                    default:
                        temp = "%";
                        temp += i;
                        break;
                }
                answer.append(temp);
            }
            is_in_opt = false;

        } else {        // 若不是%模式
            if (i == '%') {
                is_in_opt = true;
            } else {
                answer.append(1, i);
            }
        }
    }

    // 发送消息， 确保消息可以完整发送， 需要加锁
    pthread_mutex_lock(&_stream_lock);
    if (_is_console) {
        _ost << answer << "\n" << std::flush;
    } else {
        _ofs << answer << "\n" << std::flush;
    }
    pthread_mutex_unlock(&_stream_lock);
}

std::string Log::getFilePath() const {
    return _file_path;
}

void Log::setFilePath(const std::string& file_path) {
    pthread_mutex_lock(&_stream_lock);
    // 若文件路径为console的时候， 则输出到console
    if (file_path == "console") {
        _is_console = true;
        return ;
    }

    auto backup = _file_path;

    if (_ofs.is_open()) // 若文件已经打开了， 那么就关闭该文件
        _ofs.close();

    _ofs.open(file_path, std::ios::out);
    if (_ofs.is_open()) {       // 若成功打开了
        _file_path = file_path; // 更新文件路径
        _is_console = false;    // 表示不是在控制台输出了
    } else {
        if (!_is_console) { // 当之前已经打开文件的时候
            _ofs.open(backup, std::ios::out | std::ios::app);
        }
    }
    pthread_mutex_unlock(&_stream_lock);
}

const std::ostream& Log::getTarget() const {
    if (_is_console) {
        return _ost;
    } else {
        return _ofs;
    }
}

bool Log::isConsole() const {
    return _is_console;
}

void Log::exit_process() {
    log("[error] time: %c run time: %m, pid: %p, threadId: %t, errno: %e");
    exit(1) ;
}

Log::Log():
    _ost(std::cerr), _is_console(true) {
    _stream_lock = PTHREAD_MUTEX_INITIALIZER;
}

Log::~Log() {
    // 刷新缓冲区, 关闭文件
    if (_ofs.is_open()) {
        _ofs.close();
    }
}