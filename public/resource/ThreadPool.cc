#include "ThreadPool.h"

// 初始化静态变量
std::shared_ptr<ThreadPool> ThreadPool::_ptr(new ThreadPool());
threadPool_t ThreadPool::self;
std::deque<Task> ThreadPool::_task_deque;
std::set<pthread_t> ThreadPool::_thread_id;
std::shared_ptr<Log> ThreadPool::log = nullptr;
std::shared_ptr<Setting> ThreadPool::setting = nullptr;
std::shared_ptr<Language> ThreadPool::lang = nullptr;

void ThreadPool::set_max_thread(int num) {
    pthread_mutex_lock(&self._struct_lock);
    self._max_number = num;
    (*setting)["threadpool_max_thread"] = std::to_string(num);
    pthread_mutex_unlock(&self._struct_lock);
}
void ThreadPool::set_min_thread(int num) {
    pthread_mutex_lock(&self._struct_lock);
    self._min_number = num;
    (*setting)["threadpool_min_thread"] = std::to_string(num);
    pthread_mutex_unlock(&self._struct_lock);
}
void ThreadPool::set_grow_thread(int num) {
    pthread_mutex_lock(&self._struct_lock);
    self._grow_number = num;
    (*setting)["threadpool_grow_thread"] = std::to_string(num);
    pthread_mutex_unlock(&self._struct_lock);
}
void ThreadPool::set_check_time(int num) {
    pthread_mutex_lock(&self._struct_lock);
    self._check_per_time = num;
    (*setting)["threadpool_set_check_time"] = std::to_string(num);
    pthread_mutex_unlock(&self._struct_lock);
}

unsigned int ThreadPool::get_max_thread() {
    return self._max_number;
}
unsigned int ThreadPool::get_min_thread() {
    return self._min_number;
}
unsigned int ThreadPool::get_grow_thread() {
    return self._grow_number;
}
unsigned int ThreadPool::get_check_time() {
    return self._check_per_time;
}
unsigned int ThreadPool::get_working_thread() {
    return self._working_thread;
}


ThreadPool::ThreadPool() {

}

std::shared_ptr<ThreadPool> ThreadPool::ptr() {
    return _ptr;
}

void ThreadPool::startInit() {
    log = Log::ptr();
    lang = Language::ptr();
    setting = Setting::ptr();

    log->log((*lang)["threadPool_start_init"]);
    self._max_number = std::stoi((*setting)["threadpool_max_thread"]);
    self._min_number = std::stoi((*setting)["threadpool_min_thread"]);
    self._grow_number = std::stoi((*setting)["threadpool_grow_thread"]);
    self._working_thread = 0;
    self._total_thread = 0;
    self._check_per_time = std::stoi((*setting)["threadpool_set_check_time"]);
}

void ThreadPool::startThreadPool() {
    log->log((*lang)["threadPool_start_running"]);
    self._is_closed = false;
    // 初始化普通线程， 要留一个线程给守护线程
    for (auto i = 0; i < self._min_number - 1; i++) {
        pthread_t thread_id;
        int err = pthread_create(&thread_id, nullptr, thread_work, nullptr);
        if (err != 0) {
            log->log((*lang)["threadPool_create_failed"] + strerror(err));
        } else {
            _thread_id.emplace(thread_id);
            self._total_thread ++;
        }

    }

    // 初始化守护线程
    int err = pthread_create(&(self._daemon_thread), nullptr, daemon_thread, nullptr);
    if (err != 0) {
        log->log((*lang)["threadPool_create_failed"] + strerror(err) + "\n");
    } else {
        self._total_thread ++;
        self._working_thread ++;
    }

}

ThreadPool::~ThreadPool() {
    self._is_closed = true;
    // 等待守护线程退出
    pthread_join(self._daemon_thread, nullptr);

    pthread_cond_broadcast(&self._task_not_empty);
    // 等待自线程退出
    for (auto t_id : _thread_id) {
        pthread_join(t_id, nullptr);
    }
    // 单例模式， 线程池贯穿整个程序的始终，只有当程序结束的时候才会销毁， 所以无需手动delete
}


void* ThreadPool::thread_work(void * arg) {
    bool is_keep = false;
    Task task;
    while (true) {
        pthread_mutex_lock(&self._struct_lock);
        if (!is_keep) {
            while (_task_deque.size() == 0 && !self._is_closed) {
                pthread_cond_wait(&self._task_not_empty, &self._struct_lock);

                if (self._deleted_number > 0) {
                    self._deleted_number --;
                }

                if (self._total_thread > self._min_number) {
                    pthread_detach(pthread_self());
                    self._total_thread --;
                    _thread_id.erase(pthread_self());
                    pthread_mutex_unlock(&self._struct_lock);
                    pthread_exit(nullptr);
                }
            }
        }


        // 当线程池关闭的时候， 则退出
        // 关闭的时候由线程池来回收进程
        if (self._is_closed) {
            pthread_mutex_unlock(&self._struct_lock);
            pthread_exit(nullptr);
        }

        if (!is_keep) {
            task = _task_deque.front();
            _task_deque.pop_front();

            // 若为重复添加的类型， 则添加到末尾
            if (task._level == TaskLevel::DO_KEEP) {
                is_keep = true;
            }
        }

        pthread_mutex_unlock(&self._struct_lock);

        pthread_mutex_lock(&self._thread_counter);
        self._working_thread++;
        pthread_mutex_unlock(&self._thread_counter);
        // 执行任务
        task._task();

        pthread_mutex_lock(&self._thread_counter);
        self._working_thread--;
        pthread_mutex_unlock(&self._thread_counter);
    }
}

void* ThreadPool::daemon_thread(void * arg) {
    while (!self._is_closed) {
        pthread_mutex_lock(&self._thread_counter);
        auto busy_thread = self._working_thread;
        pthread_mutex_unlock(&self._thread_counter);

        double rate = (double)busy_thread / self._total_thread;
        if (rate > 0.8) {
            add_thread();
        } else if (rate < 0.2) {
            mis_thread();
        }
        sleep(self._check_per_time);
    }
    pthread_exit(nullptr);
}

void ThreadPool::add_thread() {
    pthread_mutex_lock(&self._struct_lock);
    // 当数量在允许的范围之内的时候
    if (self._total_thread + self._grow_number <= self._max_number) {
        for (int i = 0; i < self._grow_number; i++) {
            pthread_t t_id;
            int err = pthread_create(&t_id, nullptr, thread_work, nullptr);
            if (err != 0) {
                log->log((*lang)["threadPool_create_failed"] + strerror(err));
            } else {
                _thread_id.emplace(t_id);
                self._total_thread ++;
            }
        }
    }
    pthread_mutex_unlock(&self._struct_lock);
}

void ThreadPool::mis_thread() {
    pthread_mutex_lock(&self._struct_lock);
    if (self._total_thread - self._grow_number >= self._min_number) {
        self._deleted_number = self._grow_number;
        unsigned int delete_number = self._grow_number;
        pthread_mutex_unlock(&self._struct_lock);
        for (int i = 0; i < delete_number; i++) {
            pthread_cond_signal(&self._task_not_empty);
        }
        return ;
    }
    pthread_mutex_unlock(&self._struct_lock);
}

