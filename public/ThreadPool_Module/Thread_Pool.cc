#include "Thread_Pool.h"

// 初始化静态变量
std::shared_ptr<Thread_Pool> Thread_Pool::_ptr(new Thread_Pool());
//std::deque<Task> Thread_Pool::_task_deque;
//std::set<pthread_t> Thread_Pool::_thread_id;
std::shared_ptr<Log> Thread_Pool::log = nullptr;
std::shared_ptr<Setting> Thread_Pool::setting = nullptr;
std::shared_ptr<Language> Thread_Pool::lang = nullptr;

Thread_Pool::Thread_Pool() {

}

std::shared_ptr<Thread_Pool> Thread_Pool::ptr() {
    return _ptr;
}

void Thread_Pool::startInit() {
    log = Log::ptr();
    lang = Language::ptr();
    setting = Setting::ptr();

    log->log((*lang)["threadPool_start_init"]);
    // 设置最大并发数和最小并发数
    min_thread = std::stoi((*setting)["threadpool_min_thread"]);
    max_thread = std::stoi((*setting)["threadpool_max_thread"]);
    grow_thread = std::stoi((*setting)["threadpool_grow_thread"]);

    // 设置当前线程池的属性
    total_thread = std::stoi((*setting)["threadpool_min_thread"]);
    living_thread = 1;
    destroyed_thread = 0;
    checking_time = std::stoi((*setting)["threadpool_set_check_time"]);

    is_inited = true;
}

void Thread_Pool::startThreadPool() {
    log->log((*lang)["threadPool_start_running"]);
    if (!is_inited) {
        return;
    }
    is_closed = false;
    // 启动线程
    for (int i = 0; i < min_thread - 1; i++) {
        std::thread new_thread = std::thread(&Thread_Pool::common_thread, this);
        thread.emplace(new_thread.get_id(), std::move(new_thread));
    }
    // 启动守护线程
    daemon = std::move(std::thread(&Thread_Pool::daemon_thread, this));
}

Thread_Pool::~Thread_Pool() {
#ifdef DEBUG_THREAD_POOL
    std::cerr << "start end threadpool" << std::endl;
#endif
    // 单例模式， 线程池贯穿整个程序的始终，只有当程序结束的时候才会销毁， 所以无需手动delete
    is_closed = true;
    try {
        daemon.join();
    } catch(...) {

    }




#ifdef DEBUG_THREAD_POOL
    std::cerr << "daemon thread end" << std::endl;
#endif
    task_mutex.notify_all();
    for (auto &item: thread) {
        item.second.join();
    }
    thread.clear();
    task_deque.clear();
    is_inited = false;
#ifdef DEBUG_THREAD_POOL
    std::cerr << "finish end threadpool" << std::endl;
#endif
}


void Thread_Pool::common_thread() {
    bool is_keep = false;
    Task task;
    while (true) {
        // 如果不是一直执行的, 获取一个任务
        if (!is_keep) {
            // 更新任务的时候，需要将线程池锁住
            std::unique_lock<std::mutex> thread_lock(total_mutex);
            while (task_deque.empty() && !is_closed) {
                task_mutex.wait(thread_lock);
                if (destroyed_thread > 0) {
                    destroyed_thread--;

                    if (total_thread > min_thread) {
                        thread[std::this_thread::get_id()].detach();
                        total_thread--;
                        thread.erase(std::this_thread::get_id());
                        return;
                    }
                }

            }

            if (is_closed) {
                total_thread--;
                return;
            }


            // 获取下一个要执行的函数
            task = task_deque.front();
            task_deque.pop_front();
            // 若为重复添加的类型， 则添加到末尾
            if (task._level == TaskLevel::DO_KEEP) {
                is_keep = true;
            }
        } else {
            if (is_closed) {
                total_thread--;
                return;
            }
        }

        living_thread++;
#ifdef DEBUG_THREAD_POOL
        std::cerr << "threadpool start a task" << std::endl;
#endif
        // 执行任务
        task._task();
#ifdef DEBUG_THREAD_POOL
        std::cerr << "threadpool end a task" << std::endl;
#endif
        living_thread--;
    }
}

void Thread_Pool::daemon_thread() {
    // 获取当前的id
    while (1) {
        // 等待checking_time秒后再检测
        std::this_thread::sleep_for(std::chrono::seconds(checking_time));
        std::lock_guard<std::mutex> daemon_lock(total_mutex);
        // 如果关闭了
        if (is_closed) {
            return ;
        }
        // 检测是否需要添加或者减少线程
        double proportion = static_cast<double>(living_thread / total_thread);
        if (proportion > 0.8) {             // 工作的线程达到总线程的80%，则添加线程
            add_thread();
        } else if (proportion < 0.2) {      // 工作的线程小于总线程的20%，则减少线程
            mis_thread();
        }
    }
}

void Thread_Pool::add_thread() {
    unsigned int cp_grow = grow_thread;
    unsigned int cp_max = max_thread;
    // 如果当前的线程数还没到最大的线程数
    for (int i = 0; i < cp_grow; i++) {
        if (total_thread >= cp_max)
            continue;
        total_thread ++;
        std::thread new_thread = std::thread(&Thread_Pool::common_thread, this);
        thread.emplace(new_thread.get_id(), std::move(new_thread));
    }
}

void Thread_Pool::mis_thread() {
    if (total_thread <= min_thread)
        return ;
    destroyed_thread = grow_thread;
    for (int i = 0; i < destroyed_thread; i++) {
        task_mutex.notify_one();
    }
}

