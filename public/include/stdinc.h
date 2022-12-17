#pragma once
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/time.h>

#include <cerrno>
#include <cstring>
#include <csignal>

#include <iostream>
#include <fstream>

#include <string>
#include <chrono>
#include <locale>
#include <ctime>
#include <unordered_map>
#include <algorithm>
#include <deque>
#include <functional>
#include <thread>
#include <set>
#include <map>
#include <list>
#include <memory>
#include <typeinfo>
#include <condition_variable>

#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>

#include <uuid/uuid.h>

#include <nlohmann/json.hpp>

#include <Message_Displayer.h>

/*
 * 控制是否开启调试模式
 */

//#define DEBUG_MAIN            // 程序流程调试
//#define DEBUG_EPOLL           // epoll reactor 调试
//#define DEBUG_THREAD_POOL     // 线程池调试
//#define DEBUG_OUTPUT          // 输出数据调试
//#define DEBUG_INPUT           // 输入数据调试
//#define DEBUG_CLIENT_BUSINESS // 客户端事务调试


 /*
  * 全局变量
  */
#define OUT OUT::ptr()
#define IN IN::ptr()


class Default_Option;
class Event;
class Epoll_Reactor;
class File_Manager;
class Init;
class Language;
class Log;
class Message_Header;
class Message_Sender;
class Message_Receiver;
class Message_Generator;
class Message_Analysis;
class RSA_controller;
class RSA_Security;
class RSA_encrypt;
class RSA_decrypt;
class Setting;
class threadPool_t;
class TaskLevel;
class Task;
class Thread_Pool;
class User;
class User_Manager;

