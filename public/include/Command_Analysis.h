#include "stdinc.h"
#include "Log.h"
#include "Command_Service.h"

// TODO 整合代码

class Command_Analysis {
public:
    /*
     * 获取唯一的实例
     */
    static std::shared_ptr<Command_Analysis> ptr();

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
    void set_service(const std::shared_ptr<Command_Service> &);


private:
    Command_Analysis(){};

    // 服务指针
    std::shared_ptr<Command_Service> _service;

    // 当前的指针
    static std::shared_ptr<Command_Analysis> _ptr;

    std::shared_ptr<Log> _log;
};
