#pragma once
#include "stdinc.h"

/*
 * 管理用户 ，既可以是当前使用该软件的用户， 也可以是目标连接的用户
 *
 */

class User {
public:
    char *get_uuid();
    void set_uuid(const char *);
protected:
    // 当前用户的uuid
    char _uuid[37];
    // 当前用户的
    const char *_name;
    // 公钥
    // TODO 设置公钥和私钥
    // char _pub_key;

};