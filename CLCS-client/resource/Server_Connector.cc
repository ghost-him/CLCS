#include "Server_Connector.h"

std::shared_ptr<Server_Connector> Server_Connector::_ptr(new Server_Connector());

std::shared_ptr<Server_Connector> Server_Connector::ptr() {
    return _ptr;
}

void Server_Connector::startInit() {
    // 初始化内部的变量
    log = Log::ptr();
    lang = Language::ptr();
    log->log((*lang)["ServerConnector_start_init"]);
    setting = Setting::ptr();
    _socket_fd = 0;
    _is_connected = false;

    // 初始化ip和端口
    _ip = (*setting)["target_server_ip"];
    _port = std::stoi((*setting)["target_server_port"]);
    // TODO 使用语言系统来完善
    log->log("[info] ServerConnector: target ip: " + _ip + " port: " + std::to_string(_port));

    _serv_addr.sin_family = AF_INET;
    _serv_addr.sin_port = htons(_port);
    int ret = inet_pton(AF_INET, _ip.c_str(), &_serv_addr.sin_addr.s_addr);
    if (ret == 0) {
        log->log((*lang)["ServerConnector_ip_not_exist"]);
        return ;
    } else if (ret == -1) {
        log->log((*lang)["ServerConnector_ip_convert_error"]);
        return ;
    }
}

bool Server_Connector::set_target(const char * ip, int port) {
    _ip = ip;
    _port = port;

    _serv_addr.sin_family = AF_INET;
    _serv_addr.sin_port = htons(port);
    int ret = inet_pton(AF_INET, ip, &_serv_addr.sin_addr.s_addr);
    if (ret == 0) {
        log->log((*lang)["ServerConnector_ip_not_exist"]);
        return false;
    } else if (ret == -1) {
        log->log((*lang)["ServerConnector_ip_convert_error"]);
        return false;
    }
    (*setting)["target_server_ip"] = ip;
    (*setting)["target_server_port"] = std::to_string(port);
    return true;
}

bool Server_Connector::start_connect() {
    _socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    set_target(_ip.c_str(), _port);
    if (_socket_fd == -1) {
        log->log((*lang)["ServerConnector_create_socket_failed"]);
        return false;
    }

    int ret = connect(_socket_fd, (struct sockaddr*)&_serv_addr, sizeof(_serv_addr));
    if (ret != 0) {
        log->log((*lang)["ServerConnector_connect_socket_failed"] + _ip + ":" + std::to_string(_port) + " , errno: %e\n");
        return false;
    }
    // 连接成功
    _is_connected = true;
    return true;
}


void Server_Connector::close_connect() {
    shutdown(_socket_fd, 2);
    // 断开了连接
    _is_connected = false;
}

int Server_Connector::get_socketfd() {
    return _socket_fd;
}

bool Server_Connector::get_connect_status() {
    return _is_connected;
}

void Server_Connector::wait_for_connection() {

}

Server_Connector::Server_Connector() {

}

