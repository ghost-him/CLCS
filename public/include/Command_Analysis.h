#include "stdinc.h"
#include "Log.h"
#include "Command_Service.h"

// TODO 整合代码

class Command_Analysis {
public:
    static Command_Analysis* ptr;

    /*
     * 初始化分析系统
     */
    void Init_Analysis_System();
    /*
     * 分析当前的命令
     */
    void check(std::string &&);
    /*
     * 设置要执行的服务
     */
    void set_service(Command_Service*);


private:
    Command_Analysis(){};

    // 指针，用于单例模式


    // 服务指针
    Command_Service* _service;

    Log* log;
};