# CLCS

一个linux下，基于命令行的聊天软件

缓慢开发中。。。最近没啥时间。。。

## CLCS项目介绍

CLCS（Command Line Chat Software）是一个linux下的命令行软件， 采用了分布式的设计， 每一台电脑均可以当作一个服务器， 也可以当作一个客户端。

CLCS采用了`c++`作为编程语言， 采用`git`来实现版本管理，使用`cmake`作为构建系统。客户端和服务端均有自己的cmake和git仓库。

在服务端和客户端均有一个`doc`文件夹， 里面存放的是目前软件的规划，当然了，现在的这个规划还是非常不成熟的，以后在开发的过程中也会不断发生修改的。



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

### 添加好友

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

### 向好友发送hello

```
1. 好友1 和好友2连接到同一个服务器中

2. 好友2向好友1发送hello
(好友2输入)send user1 "hello"

3. 此时好友1 的屏幕中则会显示出时间，好友2的昵称和消息内容，若好友2没有被好友1添加过， 则会自动添加好友2
```


## 题外话

CLCS是我的第一个正式开始写的项目， 通过此项目， 来练习linux系统编程， linux网络编程， 还有其他工具的使用

最近在看[libhv](https://github.com/ithewei/libhv),发现了里面有一些用起来还不错的小技巧，所以我这次也用了`Command_Analysis`和`Command_Service`这两个类就是参照那个里面的`http`相关的类的写法写出来的，算作是一种尝试吧，发现`c++`最近蛮多的好用的特性的，下次的时候去尝试的写一下

c++真的是越学越不会。。。难哭了。。。

## 未来发展

目前处在一个早起的版本， 仅仅实现了基础的业务逻辑， 其他的比如历史记录， 群聊等功能都暂时未开始制作

同时软件的可维护性非常的底下， 急需重构代码

### TODO

#### 当前版本（version 0.3.0）

* ALL DONE！

#### 下一个版本（version 0.4.0）

* 尝试使用`json`来代替纯文件操作来保存应用的配置
* 使用`mariadb`数据库来保存用户的数据

[![Stargazers over time](https://starchart.cc/ghost-him/CLCS.svg)](https://starchart.cc/ghost-him/CLCS)

