# CLCS 技术文档

## 文件夹结构

**文件工作结构**

```
CLCS项目结构演示
   ├── 运行程序
   ├── history
   │   ├── user1
   │   └── user2
   ├── log
   └── option
       ├── language
       │   ├── standard.txt
       │   └── zh_CN.txt
       └── options.txt

```

```
history： 存放聊天的内容
option：	 存放程序的一系列设置
log:	  存放日志
```

**项目文件夹**

```
doc： 			存放参考文档
include： 		存放头文件
resources： 		存放.cc文件
```



## 文档格式

**变量命名规范：**

1. 清楚明了的知道这个词在程序中的作用
2. 简洁，但不会让人误解



**语言模块，设置模块文件格式：**

```
"字符串1"："字符串2"
```

```
字符串1：程序中原字符串
字符串2：显示的字符串
```

**命令行指令格式**

```
[动作名字] [参数]
```

**信息流格式**

```
[流类型] [额外参数] [消息主体]
```

* RECALL
  * SEND：表示当前的消息是由发送端发送
  * RECV：表示当前的消息是用来回应发端的消息


```
RECALL SEND [消息]
RECALL RECV [消息]
```

* KEY_TEST
  * SEND：表示当前的消息是由发送端发送
  * RECV：表示当前的消息是用来回应发端的消息

```
KEY_TEST SEND OK(加密信息)
KEY_TEST RECV OK
```

* TEXT_CHAT

```
TEXT_CHAT [目标UUID] [消息](使用目标的公钥加密)
```

* TEXT_SYS
  * USER_LIST

```
TEST_SYS USER_LIST [用户1的uuid 用户2的uuid ...]
```



## 类的说明

### `Init`类

作用：初始化整个模块， 承担了 **初始化各大模块** 和 **初始化工作文件夹** 的职责

设计模式： 单例设计模式



### `Log`类

作用：程序的日志模块， 负责向一个**指定的位置**输出经过**格式化的数据**

设计模式： 单例模式



信息格式

```
%t：当前线程的id
%p：当前进程的id
%c：当前系统的时间
%m：程序运行的时间
%e：当前程序errno的值
```

默认是在`log`文件夹下创建一个当前时间的日志文件



### `FileManager`类

作用：程序的路径管理模块

设计模式：单例模式



### `Default`类

作用：

1. 为其他的类创建文件时提供一个初始化模板
2. 用于校验读到的数据是否存在丢失



### `Language`类

作用：程序的语言管理模块

通过获取当前语言，然后去指定的文件读取相应的语言

* 若不存在该语言， 则根据`Default`类，创建一个以该语言命令的文件，作为该语言的文件
* 若存在该语言，但是文件中的记录与`Default`类中的记录不匹配， 则日志系统将其记录下来，然后结束程序



### `Setting`类

作用：为其他的模块提供参数的**初始化的值**， 同时也会保存设置的值，在程序下一次运行的时候依然会保持不变

若程序是第一次运行，则会要求用户进行设置



### `Epoll_Reactor`类

作用： 在服务端上用来与客户端做连接。

可以不设置本地的`ip`和端口，若不设置本地的`ip`,则会自动选择可用的`ip`, 若不选择端口， 则会自动选择`6000`端口

设计模式：单例模式

使用`linux`的epoll函数为基础，封装出来的



### `Server_Connector`类

作用：在客户端上，向指定的服务器做连接。

必须指定服务器的端口， 若`ip`或端口设置的不正确，则日志系统将输出错误报告， 程序退出

设计模式：单例模式



### `ThreadPool`类

作用：程序的线程池， 会自动扩充线程和销毁线程。用于快速处理程序的数据。

通过向线程池插入一个一个的任务来完成对数据的处理

设计模式：单例模式

任务有**两个**模式

| 模式      | 效果                                   |
| --------- | -------------------------------------- |
| `DO_ONCE` | 函数执行完一次以后就销毁               |
| `DO_KEEP` | 为当前函数单独分配一个线程，不断的执行 |

使用`linux`的pthread库封装出来的



### Business类

作用：程序的业务逻辑

里面所有的函数均为**静态函数**， 变量均为**静态变量**

---

### `ReadFile`类

作用：读文件的父类， 当前是`ReadWithLine`的父类

### `WriteFile` 类

作用：写文件的父类，当前是`WriteWithLine`的父类

---

### `ReadWithLine ` 类

作用：从某一个给定的文件中以一个`特定的格式`一行一行地读取文件，然后存放到哈希表中

与`WriteWithLine`相对应

### `WriteWithLine` 类

作用：向某一个给定的文件一行一行地写入**特定格式**的数据

与`ReadWithLine`相对应

---

### User类

描述一个用户的类

```
char _uuid[37];		// 当前用户的uuid
const char *_name;	// 当前用户的昵称
```



### `Event`类

描述epoll反应堆的一个事务

```
std::list<Event>::iterator _index;          // 当前的点在维护数组中的位置
int _socketfd = 0;       		// 当前的操作符
int _events = 0;        		// 当前事件的状态
std::function<void()> _func;	// 执行的函数
time_t _last_active = 0;    	// 上一次运行的事件
char _buf[BUFSIZ];       		// 缓冲区
int _len = 0;            		// 缓冲区的大小
```



### `MessageHeader`类

描述信息的属性

```
typedef char Level;
static const char RECALL = 1;		// 回环属性
static const char TEXT_CHAT = 2;	// 聊天信息
static const char TEXT_SYS = 3;		// 系统信息
static const char KEY_TEST = 4;		// 密钥信息

typedef char Option;
static const char NOMEAN = 0;		// 无意义
static const char SEND = 1;			// 当前信息是由发送端发送的
static const char RECV = 2;			// 当前信息是用来回应发送端的
static const char USER_LIST = 3;	// 数据的内容为当前的在线用户
```





## 后期优化

1. 使用`move`， `forword`等函数优化拷贝复制操作符
2. 添加系统请求限制， 防止请求过高导致服务器卡顿
2. 完善日志系统的输出功能
2. 完善所有函数的错误处理
