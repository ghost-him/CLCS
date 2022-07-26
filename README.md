# CLCS

一个linux下，基于命令行的聊天软件

## CLCS项目介绍

CLCS（Command Line Chat Software）是一个linux下的命令行软件， 采用了分布式的设计， 每一台电脑均可以当作一个服务器， 也可以当作一个客户端。

CLCS分为两个应用程序，一个是客户端，一个是服务端。客户端是用户使用的程序，服务端则是为其他用户提供服务的程序。可以将服务端存放在远程服务器上。这样就可以实现用户之间的远程交流了。

CLCS采用了`c++`作为编程语言， 采用`git`来实现版本管理，使用`cmake`作为构建系统。客户端和服务端均存放在同一个目录下，由同一个`CMakeLists.txt`来管理。

客户端与服务端采用`tcp`连接，其中服务端采用了`epoll ractor`模型，同时都使用了**线程池**来辅助程序的运行。由于`epoll reactor`使用了`ET`模式，同时我的水平还没有特别高，所以在运行时可能会出现不可预料的问题，我已经尽我的努力让出错的风险降低了。如果你发现了新的bug，欢迎联系我。

目前实现的功能还是比较单一的，受时间所限，暂时无法开发出更多功能了

目前**客户端**已经实现的功能有：

1. 添加用户
2. 重命名用户
3. 给用户发送消息（加密）
4. 接收用户发送的消息
5. 重新连接服务器
6. 重新设置目标服务器的地址
7. 查看-
   1. 当前系统的时间
   2. 自己的`uuid`
   3. 当前已经添加的好友
   4. 当前程序与服务器的连接的状态
   5. 当前程序与服务器的延迟
8. 向服务器发送回调消息（服务器会将消息原封不动的返回）
9. 保存设置
10. 退出程序

目前**服务端**已经实现的功能：

1. 可操作
   1. 退出程序
2. 不可操作
   1. 连接与客户端的连接
   2. 处理客户端发来的请求
   3. 向客户端发送信息流

## 程序安装方法

### 开发环境

当前的开发环境：

* `Linux version 5.19.1-3-MANJARO`
* `gcc 版本 12.1.1 20220730 (GCC)`
* `cmake version 3.23.3`

要求的开发环境：

* 支持`c++20`的标准
* 支持`cmake`的`-B` 和 `--build`命令
* 安装`openssl`开发环境
* 安装`libuuid`库
* 安装`json`库（作者是`nlohmann`）



### 安装步骤

不论是服务器还是客户端，都分为两步：

1. 建立`cmake`的工作文件夹

```
cmake -B build
```

2. 执行`cmake`的命令

#### 服务端
```
cmake --build build --target=CLCS-server -j=8
```
`-j的值可以根据你自己电脑上的核心的数量来改变`

3. 进入 `./build/CLCS-server` 文件夹

4. 运行`CLCS-server`（也可以将当前程序移动到其他地方启动， 启动的时候，程序会在当前的目录下生成工作文件夹）

5. 进行初始化设置

```
语言： 看情况选择， 若无对应的语言文件， 则需要自己翻译， 语言文件在当前目录下的 `option/language` 文件夹中
ip与端口： 若无特殊需要， 跳过即可（ip将为当前可用ip, 端口将设置为6000）
```
6. 程序开始运行

#### 客户端
```
cmake --build build --target=CLCS-client -j=8
```
3. 进入 `build/CLCS-client` 文件夹

4. 运行`CLCS-client`（也可以将当前程序移动到其他地方启动， 启动的时候，程序会在当前的目录下生成工作文件夹）

5. 进行初始化设置
```
语言： 看情况选择， 若无对应的语言文件， 则需要自己翻译， 语言文件在当前目录下的 `option/language` 文件夹中
ip与端口： 填写目标服务器的地址（ip + 端口）
```

6. 程序开始运行

## 快速入门

### 客户端

#### 命令展示

以下是在客户端中输入`help`命令弹出来的提示信息

```
add
        添加指定的用户        
        add [uuid]

cat
        查看信息        
        cat [参数]
        connect
                与服务器的连接状态        
        latency
                与服务器的延迟
        time
                当前系统的时间
        user
                当前已经保存的用户
        uuid
                自己的uuid

help
        输出此条信息
re_connect
        重新连接目标服务器
recall
        向服务器发送回调消息        
        recall [消息]

rename
        更改用户的昵称        
        rename [uuid/旧昵称] [新昵称]

reset_target
        重新设置目标服务器的ip和地址
save
        保存设置
send
        向用户发送消息        
        send [uuid/昵称] [消息]
```

#### 主要功能一：添加好友

```
1. 好友1 和 好友2连接到同一个服务器中

2. 好友1查看自己的uuid
(好友1输入)cat uuid
此时设好友1的uuid为51c44468-329e-4df6-8771-5d6350842ec5

3. 好友2添加好友1
(好友2输入)add 51c44468-329e-4df6-8771-5d6350842ec5
这样好友2就将好友1的公钥信息保存到了自己的本地

4. 好友2 重命名 好友1的昵称
(好友2输入)rename 51c44468-329e-4df6-8771-5d6350842ec5 user1
这样，好友1的昵称就设置为了user1
```

#### 主要功能二：向好友发送hello

```
1. 好友1 和好友2连接到同一个服务器中

2. 好友2向好友1发送hello
(好友2输入)send user1 "hello"

3. 此时好友1 的屏幕中则会显示出时间，好友2的昵称和消息内容，若好友2没有被好友1添加过， 则会自动添加好友2
```

### 服务端

服务端主要的功能是用于转发客户端的消息。

#### 命令展示

有两个命令可以使用：

```
quit  --- 退出并保存当前的程序

block --- 阻塞输入
```

#### 将服务器放在后台运行

有两种办法

1. 使用`nohup`命令 
	1. `nohup ./CLCS-server `或 `nohup ./CLCS-server`
2. 直接使用`&`
	1. `./CLCS-server block &`

**阻塞了以后，如果想要重新输入，只需要输入 `CTRL + C` 即可恢复**

## 题外话

从`v0.3.0`版本开始，我就一直在重构这个软件，现在的这个软件已经基本达到了我认为还可以的样子了，越来越觉得，软件的设计比软件的编码要重要的多，一个好的设计确实可以节省很多时间。

这一个版本从重构开始，面对报错比代码多到能编译但不能运行，再到能运行，但是只能运行一段时间，最后到现在能够在我测试的期间可以稳定运行。我曾对着一个`bug`不断调试，一看就是一个晚上，数百次运行程序，从满屏幕的调试信息中推断出错误的原因 。在这期间我不断在想是不是我自己的水平真的不够，一直写这个没人用的软件真的值得嘛，为什么这个程序一直会报错，为什么我一直找不到这个错误点。我曾一度想放弃，不想折磨自己了。

> 今晚改不出来，我就不睡觉了
>
> bug, bug, bug, all of bugs

不过现在已经过去了，解放了。



不过客户端的`message`这一类的类还是可以再优化一下的，可以使用服务器的`Message_Stream`来改进一下

c++真的是越学越不会。。。难哭了。。。

## 未来发展

目前处在一个早起的版本， 仅仅实现了基础的业务逻辑， 其他的比如历史记录， 群聊等功能都暂时未开始制作

软件还没有跑过压力测试，或许以后可以想办法跑一下压力测试

### TODO

#### 当前版本（version 0.5.3）

* ALL DONE！

#### 下一个版本（version 0.6.0）

* 添加群聊的功能
* 重构客户端的代码
* 尝试将`epoll reactor`和**线程池**结合起来
