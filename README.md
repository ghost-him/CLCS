# CLCS

[english](https://github.com/ghost-him/CLCS/blob/main/english.md)

一个linux下，基于命令行的聊天软件

缓慢开发中。。。最近没啥时间。。。

## CLCS项目介绍

CLCS（Command Line Chat Software）是一个linux下的命令行软件， 采用了分布式的设计， 每一台电脑均可以当作一个服务器， 也可以当作一个客户端。

CLCS采用了`c++`作为编程语言， 采用git来实现版本管理，使用cmake作为构建系统。客户端和服务端均有自己的cmake和git仓库

在服务端和客户端均有一个`doc`文件夹， 里面存放的是目前软件的规划，当然了，现在的这个规划还是非常不成熟的，以后在开发的过程中也会不断发生修改的

## 当前实现的功能

目前的功能**只是**服务器将读到的话再**原封不动的**回传给客户端

客户端和服务器在第一次启动的时候会自动创建文件夹以及设置一系列的选项

## 程序安装方法

首先先确保自己的电脑的环境为linxu
然后检查是否有c++编译器，cmake这两个工具

### 服务端
1. 进入 `CLCS-server` 文件夹

2. 输入
```
cmake -B build
cmake --build build
```
3. 进入 `build` 文件夹

4. 运行CLCS

5. 进行初始化设置
```
语言： 看情况选择， 若无对应的语言文件， 则需要自己翻译， 语言文件在当前目录下的 `option/language` 文件夹中
ip与端口： 若无特殊需要， 跳过即可（ip将为当前可用ip, 端口将设置为6000）
```
6. 程序开始运行

### 客户端
1. 进入 `CLCS-client` 文件夹

2. 输入
```
cmake -B build
cmake --build build
```
3. 进入 `build` 文件夹

4. 运行CLCS

5. 进行初始化设置
```
语言： 看情况选择， 若无对应的语言文件， 则需要自己翻译， 语言文件在当前目录下的 `option/language` 文件夹中
ip与端口： 填写目标服务器的地址（ip + 端口）
```

6. 程序开始运行

## 注意点

* 先运行服务器，再运行客户端，不然会出现意料以外的bug（可以修复， 但是对于当前的版本来说没必要，目前还处在搭建框架的时期）

## 题外话

CLCS是我的第一个正式开始写的项目， 通过此项目， 来练习linux系统编程， linux网络编程， 还有其他工具的使用

c++真的是越学越不会。。。难哭了。。。

## 未来发展

目前还处在一个非常早期的状态， 还有非常多的功能没有实现， 目前写出来的代码的运行效率也是很低的。

### TODO

#### 当前版本（version 0.1.0）

* ALL DONE！

#### 下一个版本（version 0.2.0）

* 添加命令行解析模块      DONE
* 添加加密模块           DONE
* 编写业务逻辑           DOING
* 将这些模块整合到一起    DOING
* 添加更多的提示

[![Stargazers over time](https://starchart.cc/ghost-him/CLCS.svg)](https://starchart.cc/ghost-him/CLCS)

