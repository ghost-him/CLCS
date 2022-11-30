#include "Security.h"

RSA_controller::RSA_controller() {
    log = Log::ptr();
    lang = Language::ptr();
}

RSA_controller::~RSA_controller() {

}

void RSA_controller::set_private_key_path(const std::string & path) {
    _pri_path = path;
}

void RSA_controller::set_public_key_path(const std::string & path) {
    _pub_path = path;
}

bool RSA_controller::generate_keys(int len) {
    // 不能是空路径
    if (_pri_path.empty() || _pub_path.empty()) {
        log->log((*lang)["RSA_controller_invalid_key_path"]);
        return false;
    }

    EVP_PKEY * pkey = EVP_RSA_gen(len);
    if (pkey == nullptr) {
        log->log("[error] RSA_controller: rsa gen error");
        ERR_print_errors_fp(stderr);
        log->exit_process();
    }

    FILE* f_pri = fopen(_pri_path.c_str(), "wb");
    FILE* f_pub = fopen(_pub_path.c_str(), "wb");

    bool is_success = true;
    // 写入私钥
    if (f_pri != nullptr) {
        PEM_write_PrivateKey(f_pri, pkey, nullptr, nullptr,0, nullptr, nullptr);
    } else {
        log->log("[error] RSA_controller write private key error");
        is_success = false;
    }

    if (f_pub != nullptr) {
        PEM_write_PUBKEY(f_pub, pkey);
    } else {
        log->log("[error] RSA_controller write public key error");
        is_success = false;
    }

    // 关闭文件
    fclose(f_pri);
    fclose(f_pub);
    f_pri = nullptr;
    f_pub = nullptr;
    // 释放内存
    EVP_PKEY_free(pkey);
    pkey = nullptr;
    return is_success;
}



RSA_Security::RSA_Security() {
#ifdef DEBUG_MAIN
    std::cerr << "_pkey new" << std::endl;
#endif
    _pkey = nullptr;
}


RSA_Security::~RSA_Security() {
    if (_pkey != nullptr) {
#ifdef DEBUG_MAIN
        std::cerr << "_pkey delete" << std::endl;
#endif
        EVP_PKEY_free(_pkey);
    }
}


void RSA_Security::startInit() {
    log = Log::ptr();
    lang = Language::ptr();
}

void RSA_Security::init() {
    EVP_PKEY_free(_pkey);
    _pkey = EVP_PKEY_new();
}

bool RSA_Security::set_and_read_key(const std::string & str) {
    log->log("RSA_security set key path: " + str + " and read key");
    return set_key_path(str) && read_key();
}

RSA_encrypt::RSA_encrypt() {
    _output_text = std::make_shared<unsigned char[]>(BUFSIZ);
}

void RSA_encrypt::startInit() {
    // 装饰器模式
    RSA_Security::startInit();
    log->log("[info] RSA_encrypt start init");
}


bool RSA_encrypt::read_key() {
    FILE * key_path = fopen(_key_path.c_str(), "r");
    if (key_path == nullptr) {
        log->log((*lang)["RSA_encrypt_open_file_error"]);
        return false;
    }
    // 读取公钥
    _pkey = PEM_read_PUBKEY(key_path,nullptr, nullptr, nullptr);
    fclose(key_path);
    if (_pkey == nullptr) {
        key_path = nullptr;
        log->log((*lang)["RSA_encrypt_read_public_key_failed"] + ERR_reason_error_string(ERR_get_error()));
        error = ERR_reason_error_string(ERR_get_error());
        return false;
    }
    // 关闭文件
    key_path = nullptr;
    _output_len = EVP_PKEY_get_size(_pkey);
    return true;
}

bool RSA_encrypt::set_key_path(const std::string & path) {
    // 检测是否存在该文件
    if (access(path.c_str(), F_OK) != 0) {
        log->log((*lang)["RSA_encrypt_incorrect_path"] + path);
        return false;
    }
    _key_path = path;
    return true;
}

bool RSA_encrypt::convert() {
    // 清空上次的内容
#ifdef DEBUG_MAIN
    std::cerr << "rsa_encrypt start convert" << std::endl;
#endif

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(_pkey, nullptr);
    EVP_PKEY_encrypt_init(ctx);


    if (!EVP_PKEY_encrypt(ctx, _output_text.get(), &_output_len, reinterpret_cast<const unsigned char *>(_input_text.get()), _input_len)){
        log->log((*lang)["RSA_encrypt_convert_error"] + ERR_reason_error_string(ERR_get_error()));
        error = ERR_reason_error_string(ERR_get_error());
        return false;
    }
    return true;
}

size_t RSA_encrypt::size() {
    return EVP_PKEY_get_size(_pkey);
}

void RSA_encrypt::set_input(const std::string& str) {
    // 创建一个指定大小的空间
    auto temp = std::make_shared<char[]>(str.size());
    // 复制空间的内容
    memcpy(temp.get(), str.c_str(), str.size());
    set_input(temp, str.size());
}

void RSA_encrypt::set_input(std::shared_ptr<char[]>& str, size_t len) {
    _input_text = str;
    _input_len = len;
}

std::shared_ptr<unsigned char[]> RSA_encrypt::get_output() {
    return _output_text;
}

RSA_decrypt::RSA_decrypt() {
    _input_text = std::make_shared<unsigned char[]>(BUFSIZ);
    _output_text = std::make_shared<char[]>(BUFSIZ);
}

void RSA_decrypt::startInit() {
    // 装饰器模式
    RSA_Security::startInit();
    log->log("[info] RSA_decrypt start init");
}

/*
 * 读取私钥
 */
bool RSA_decrypt::read_key() {
    FILE * key_path = fopen(_key_path.c_str(), "r");
    if (key_path == nullptr) {
        log->log((*lang)["RSA_decrypt_open_file_error"]);
        return false;
    }
    // 读取私钥
    _pkey = PEM_read_PrivateKey(key_path, nullptr, nullptr, nullptr);
    fclose(key_path);

    if (_pkey == nullptr) {
        key_path = nullptr;
        log->log((*lang)["RSA_decrypt_read_public_key_failed"] + ERR_reason_error_string(ERR_get_error()));
        error = ERR_reason_error_string(ERR_get_error());
        return false;
    }
    key_path = nullptr;
    return true;
}
    /*
     * 设置私钥的路径
     */
bool RSA_decrypt::set_key_path(const std::string & path) {
    // 检测是否存在该文件
    if (access(path.c_str(), F_OK) != 0) {
        log->log((*lang)["RSA_decrypt_incorrect_path"] + path);
        return false;
    }
    _key_path = path;
    return true;
}
/*
* 将加密的信息解密
*/
bool RSA_decrypt::convert() {
#ifdef DEBUG_MAIN
    std::cerr << "rsa_decrypt start convert" << std::endl;
#endif
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(_pkey, nullptr);
    EVP_PKEY_decrypt_init(ctx);

    _output_len = BUFSIZ;
    // 转换信息
    if (!EVP_PKEY_decrypt(ctx, reinterpret_cast<unsigned char *>(_output_text.get()), &_output_len, _input_text.get(), _input_len)){
        log->log((*lang)["RSA_decrypt_convert_error"] + ERR_reason_error_string(ERR_get_error()));
        error = ERR_reason_error_string(ERR_get_error());
        return false;
    }
    // 在末尾打上结束, 以防止上一次的结果对这次的结果产生影响
    _output_text[_output_len] = '\0';

    return true;
}

size_t RSA_decrypt::size() {
    return EVP_PKEY_get_size(_pkey);
}


void RSA_decrypt::set_input(const unsigned char* str, size_t len) {
    memcpy(_input_text.get(), str, len);
    _input_len = len;
}

void RSA_decrypt::set_input(std::shared_ptr<unsigned char[]>& str, size_t len) {
    _input_text = str;
    _input_len = len;
}

std::shared_ptr<char[]> RSA_decrypt::get_output() {
    return _output_text;
}

