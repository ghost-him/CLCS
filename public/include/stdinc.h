#pragma once
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/time.h>


#include <cerrno>
#include <cstring>

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

#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/err.h>

#include <uuid/uuid.h>

class DefaultOption;
class Event;
class Epoll_Reactor;
//class ReadFile;
//class ReadWithLine;
//class ReadUser;
//class WriteFile;
class WriteWithLine;
//class WriteUser;
class FileManager;
class Init;
class Language;
class Log;
class MessageHeader;
class MessageSender;
class MessageReceiver;
class MessageGenerator;
class MessageAnalysis;
class RSA_controller;
class RSA_Security;
class RSA_encrypt;
class RSA_decrypt;
class Setting;
class threadPool_t;
class TaskLevel;
class Task;
class ThreadPool;
class User;
class User_Manager;