#include "Server_Connector.h"

Server_Connector* Server_Connector::_pConnector = new Server_Connector;


Server_Connector *Server_Connector::getInstance() {
    return _pConnector;
}

void Server_Connector::startInit() {
    log = Log::getInstance();
    lang = Language::getInstance();
    log->log((*lang)["ServerConnector_start_init"]);
    setting = Setting::getInstance();
    _socketfd = 0;
    // 初始化ip和端口
    _ip = (*setting)["target_server_ip"];
    _port = std::stoi((*setting)["target_server_port"]);
}

void Server_Connector::set_target(const char * ip, int port) {
    _ip = ip;
    _port = port;

    _serv_addr.sin_family = AF_INET;
    _serv_addr.sin_port = htons(port);
    int ret = inet_pton(AF_INET, ip, &_serv_addr.sin_addr.s_addr);
    if (ret == 0) {
        log->log((*lang)["ServerConnector_ip_not_exist"]);
        return ;
    } else if (ret == -1) {
        log->log((*lang)["ServerConnector_ip_convert_error"]);
        return ;
    }
    (*setting)["target_server_ip"] = ip;
    (*setting)["target_server_port"] = std::to_string(port);

}

bool Server_Connector::start_connect() {
    _socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_socketfd == -1) {
        log->log((*lang)["ServerConnector_create_socket_failed"]);
        return false;
    }

    int ret = connect(_socketfd, (struct sockaddr*)&_serv_addr, sizeof(_serv_addr));
    if (ret != 0) {
        log->log((*lang)["ServerConnector_connect_socket_failed"] + _ip + ":" + std::to_string(_port) + " , errno: %e\n");
        return false;
    }
    return true;
}


void Server_Connector::close_connect() {
    close(_socketfd);
}

int Server_Connector::get_socketfd() {
    return _socketfd;
}
Server_Connector::Server_Connector() {

}

