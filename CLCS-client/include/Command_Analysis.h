#include "stdinc.h"
#include "Log.h"
#include "Business.h"

// TODO 整合代码

class Command_Analysis {
public:
    static Command_Analysis* getInstance();

    /*
     * 初始化分析系统
     */
    void Init_Analysis_System();

    void check(const std::string &);


private:
    Command_Analysis(){};
    /*
     * 输出帮助页面
     */
    static void help();

    // 指针
    static Command_Analysis* _pCommand_Analysis;

    Log* log;
};


