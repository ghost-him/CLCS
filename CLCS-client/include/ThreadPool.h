#pragma once
#include <stdinc.h>
#include "Log.h"
#include "Setting.h"
#include "Language.h"

class threadPool_t {
public:
    // 结构体的锁
    pthread_mutex_t _struct_lock = PTHREAD_MUTEX_INITIALIZER;
    // 用于管理线程的数量
    pthread_mutex_t _thread_counter = PTHREAD_MUTEX_INITIALIZER;

    pthread_cond_t _task_not_empty = PTHREAD_COND_INITIALIZER;

    // 机器的默认的支持的并发数
    unsigned int _max_number = 0;           // 最大的线程数量
    unsigned int _min_number = 0;           // 最小的线程数量
    unsigned int _grow_number = 0;          // 每一次增加的线程的数量
    unsigned int _deleted_number = 0;       // 即将被删除的线程的数量

    unsigned int _working_thread = 1;       // 正在工作的线程
    unsigned int _total_thread = 0;         // 总线程

    pthread_t _daemon_thread;           // 守护线程的id

    int _check_per_time = 5;    // 每一次自检的间隔

    bool _is_closed = true;        // 线程池是否关闭

};

class TaskLevel {
public:
    enum Level {
        DO_ONCE = 1,
        DO_KEEP = 2
    };
};

class Task {
public:
    // 当前的任务
    std::function<void()> _task;
    // 当前任务的等级
    TaskLevel::Level _level;
};



class ThreadPool final {
public:
    static ThreadPool* getInstance();   // 获取实例化

    static void InitThreadPool();

    static void startThreadPool();

    template<class Func, class ...Args> // 添加任务
    static void commit(TaskLevel::Level level, Func&& function, Args&&... args) {
        auto task_func = std::bind(std::forward<Func>(function), std::forward<Args>(args)...);

        Task task;
        task._task = task_func;
        task._level = level;

        pthread_mutex_lock(&self._struct_lock);
        _task_deque.push_back(task);
        pthread_cond_signal(&self._task_not_empty);
        pthread_mutex_unlock(&self._struct_lock);
    }

    static void set_max_thread(int num);       // 设置最大的线程数
    static void set_min_thread(int num);       // 设置最小的线程数
    static void set_grow_thread(int num);      // 设置变化的线程数
    static void set_check_time(int num);       // 设置自检的时间

    static unsigned int get_max_thread();       // 获取当前最大的线程数
    static unsigned int get_min_thread();       // 获取当前最小的线程数
    static unsigned int get_grow_thread();      // 获取当前变化的线程数
    static unsigned int get_check_time();       // 获取当前自检的时间
    static unsigned int get_working_thread();   // 获取当前正在工作的线程的数量

private:

    static threadPool_t self;
    ThreadPool();
    ~ThreadPool();
    // 饿汉式
    static ThreadPool* _pThreadPool;

    // 普通线程做的任务
    static void* thread_work(void *);

    /*
     * 守护线程做的任务
     * 用于管理其他的线程
     * 检查是否需要增减线程
     *
     */
    static void* daemon_thread(void *);

    // 添加线程
    static void add_thread();

    // 减少线程
    static void mis_thread();

    static std::deque<Task> _task_deque;  // 任务队列,做一次的
    static std::set<pthread_t> _thread_id;
    static Log* log;
    static Setting &setting;
    static Language* lang;
};



