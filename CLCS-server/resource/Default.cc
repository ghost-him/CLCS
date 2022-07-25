#include "Default.h"

const std::unordered_map<std::string, std::string> DefaultOption::store = {
        {"sys_language", "zh_CN"},      // 语言

        {"target_server_ip", "default"},       // 若为空， 则默认为本地有效ip
        {"target_server_port", "6000"}, // 目标服务器的端口
        {"epoll_reactor_max_connect", "4096"},  // 反应堆最大的连接数
        {"epoll_reactor_max_events", "8192"},   // 反应堆最大支持的事件数

        // TODO 在first_run_time的时候填写到文件中
        {"threadpool_max_thread", ""},
        {"threadpool_min_thread", ""},
        {"threadpool_grow_thread", ""},
        {"threadpool_set_check_time", "5"}
};

const std::unordered_map<std::string, std::string> DefaultOption::language = {
        {"epoll_reactor_create_tree_failed", "[error] epoll_reactor: create epoll tree failed: %e"},
        {"epoll_reactor_create_socket_failed", "[error] epoll_reactor: create socket error: %e"},
        {"epoll_reactor_invalid_ip", "[warn] epoll_reactor: invalid ip:"},
        {"epoll_reactor_convert_ip_error", "[error] epoll_reactor: convert ip error :%e"},
        {"epoll_reactor_bind_addr_failed", "[error] epoll_reactor bind addr error: %e"},
        {"epoll_reactor_listen_error", "[error] epoll_reactor listen error: %e"},
        {"epoll_reactor_add_event_error", "[warn] epoll_reactor: event add failed, errno: %e"},
        {"epoll_reactor_accept_error", "[error] epoll_reactor accept error: %e"},
        {"epoll_reactor_set_no_block_error", "[error] epoll_reactor: set O_NONBLOCK failed, %e"},
        {"epoll_reactor_new_connection", "[info] epoll_reactor: new connect ip: "},
        {"epoll_reactor_client_close", "[info] epoll_reactor client closed"},
        {"epoll_reactor_read_error", "[error] epoll_reactor read error , %e"},
        {"epoll_reactor_write_error", "[error] epoll_reactor write error , %e"},
        {"epoll_reactor_start_listen", "[info] epoll_reactor: start_listen"},
        {"epoll_reactor_wait_error", "[error] epoll_reactor wait error: %e"},

        {"threadPool_start_init", "[info] ThreadPool start init "},
        {"threadPool_start_running", "[info] ThreadPool running "},
        {"threadPool_create_failed", "[error] ThreadPool create thread failed: "},

        {"ReadWithLine_read_failed", "[error] ReadWithLine: read file failed, location: "},
        {"ReadWithLine_read_invalid", "[warn] invalid line"},
        {"WriteWithLine_invalid_path", "[error] ReadWithLine: invalid path"},

        {"MessageGenerator_invalid_level", "[warn] MessageGenerator: message level not exist :"},
        {"MessageSender_message_too_long", "[error] MessageSender: string to be send up to max size"},
        {"MessageSender_send_error", "[error] MessageSender: send error: %e"},
        {"MessageReceiver_read_error", "[error] MessageReceive: read file error"},
        {"MessageReceiver_close_connect", "[info] MessageReceiver: closing the connect"},

        {"ServerConnector_start_init", "[info] server_connector: start init"},
        {"ServerConnector_ip_not_exist", "[error] ip is not exist: %e"},
        {"ServerConnector_ip_convert_error", "[error] Server_connector ip convert failed : %e"},
        {"ServerConnector_create_socket_failed", "[error] Server_connector socket created failed, %e"},
        {"ServerConnector_connect_socket_failed", "[error] Server_connector socket connect error: "}
};
