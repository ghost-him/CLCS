#include "Command_Parameter_Analysis.h"

std::shared_ptr<Command_Parameter_Analysis> Command_Parameter_Analysis::_ptr(new Command_Parameter_Analysis);

std::shared_ptr<Command_Parameter_Analysis> Command_Parameter_Analysis::ptr() {
    return _ptr;
}

std::string Command_Parameter_Analysis::format_string;
std::map<char, Command_Parameter> Command_Parameter_Analysis::parameters;

bool Command_Parameter_Analysis::parse(int argc, char **argv) {
    // 添加对应的参数
    add('h', false, "输出此条提示信息", help);
    startInit();
    // 开始解析
    int o;
    while ((o = getopt(argc, argv, format_string.c_str())) != -1) {
        if (o == 'h') {
            help(optarg);
            return false;
        }
        if (parameters.contains(o)) {
            // 如果有这个参数
            const Command_Parameter& temp = parameters[o];
            temp.func(optarg);
        } else {
            //没有这个参数
            help(optarg);
            return false;
        }
    }
    return true;
}

void Command_Parameter_Analysis::help(void *) {
    for (auto item : parameters) {
        // -t [] ""
        std::string out;
        out += "-";
        out.push_back(item.second.para);
        out += " ";
        if (item.second.has_arg)
            out += "[参数] ";
        out += item.second.description;
        OUT << out;
    }
}
