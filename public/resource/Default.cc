#include "Default.h"

const std::unordered_map<std::string, std::string> DefaultOption::store = {
        {"sys_language", "zh_CN"},      // 语言

        {"target_server_ip", "default"},       // 若为空， 则默认为本地有效ip
        {"target_server_port", "6000"}, // 目标服务器的端口
        {"epoll_reactor_max_connect", "4096"},  // 反应堆最大的连接数
        {"epoll_reactor_max_events", "8192"},   // 反应堆最大支持的事件数

        {"threadpool_max_thread", ""},
        {"threadpool_min_thread", ""},
        {"threadpool_grow_thread", ""},
        {"threadpool_set_check_time", "5"},

        {"uuid", ""},
        {"pub_key_path", ""},
        {"pri_key_path", ""}
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
        {"MessageReceiver_read_error", "[error] MessageReceiver read socket error, %e"},
        {"MessageReceiver_read_content_error", "[debug] MessageReceive exec read_content() error"},

        {"ServerConnector_start_init", "[info] server_connector: start init"},
        {"ServerConnector_ip_not_exist", "[error] ip is not exist: %e"},
        {"ServerConnector_ip_convert_error", "[error] Server_connector ip convert failed : %e"},
        {"ServerConnector_create_socket_failed", "[error] Server_connector socket created failed, %e"},
        {"ServerConnector_connect_socket_failed", "[error] Server_connector socket connect error: "},
        {"ServerConnector_set_fd_nonblock_error", "[error] Server_connector set socket nonblock error: %e"},

        {"RSA_controller_invalid_key_path", "[error] RSA_controller invalid keys path"},
        {"RSA_controller_generate_error", "[error] RSA_controller generator error: "},
        {"RSA_controller_write_public_key_error", "[error] RSA_controller write public key error"},
        {"RSA_controller_write_private_key_error", "[error] RSA_controller write private key error"},
        {"RSA_encrypt_open_file_error", "[error] RSA_encrypt open file error, %e"},
        {"RSA_encrypt_read_public_key_failed", "[error] RSA_encrypt read public key failed: "},
        {"RSA_encrypt_incorrect_path", "[warn] RSA_encrypt incorrect path: %e"},
        {"RSA_encrypt_convert_error", "[error] RSA_encrypt convert error "},
        {"RSA_decrypt_open_file_error", "[error] RSA_decrypt open file error, %e"},
        {"RSA_decrypt_read_public_key_failed", "[error] RSA_decrypt read public key failed: "},
        {"RSA_decrypt_incorrect_path", "[warn] RSA_decrypt incorrect path: %e"},
        {"RSA_decrypt_convert_error", "[error] RSA_decrypt convert error "},

        {"Business_invalid_level", "[debug] Business invalid level: "}
};
