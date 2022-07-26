# CLCS

[简体中文](https://github.com/ghost-him/CLCS/blob/main/README.md)

A linux, command-line based chat software

## CLCS Project Introduction

CLCS (Command Line Chat Software) is a linux command line software, using a distributed design, each computer can be used as a server, but also as a client.

CLCS uses `c++` as the programming language, git for versioning, and cmake as the build system. Both the client and server have their own cmake and git repositories

There is a `doc` folder on both the server and the client, where the current software plan is stored, but of course, the current plan is still very immature and will be modified in the course of development

## Current functionality

The current function **is only **the server will read the words and then **intact** back to the client

The client and server will automatically create a folder and set a series of options when they are first started.

## How to install the program

First make sure your computer's environment is linxu
Then check if you have the c++ compiler and cmake tools.

### Server side
1. go to the `CLCS-server` folder

2. Enter
```
cmake -B build
cmake --build build
```
3. Go to the `build` folder

4. Run CLCS

5. Do the initial setup

```
Language: Choose the language you want, if you don't have a language file, you will need to translate it yourself, the language file is in the `option/language` folder in the current directory
ip and port: if you don't need it, just skip it (ip will be the current available ip, port will be set to 6000)
``` 

6. Start the program

### Client

1. Go to the `CLCS-client` folder

2. Enter

```
cmake -B build
cmake --build build
```

3. Go to the `build` folder

4. Run CLCS

5. Do the initial setup
```
Language: Choose the language, if there is no language file, you need to translate it yourself, the language file is in the `option/language` folder in the current directory
ip and port: fill in the address of the target server (ip + port)
``` 

6. Start the program

## Points to note

* Run the server first, then run the client, otherwise unexpected bugs will appear (can be fixed, but not necessary for the current version, it is still in the framework building period)

## Off-topic

CLCS is my first official project, through this project, to practice linux system programming, linux network programming, and the use of other tools

The more I learn C++, the less I know how to use it. It's hard to cry.

## Future development

It is still in a very early state, there are still a lot of functions not implemented, and the code written so far is also very inefficient.

### TODO

#### Current version (version 0.1.0)

* Improve the translation of the language system

#### Next release (version 0.2.0)

* Add command line parsing module
* Add encryption module
* Integrate these modules into one
* Add more hints

Translated with www.DeepL.com/Translator (free version)
