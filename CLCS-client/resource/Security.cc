#include "Security.h"

RSA_controller::RSA_controller() {
    log = Log::getInstance();
    lang = Language::getInstance();
}

RSA_controller::~RSA_controller() {

}

void RSA_controller::set_private_key_path(const std::string & path) {
    _pri_path = path;
}

void RSA_controller::set_public_key_path(const std::string & path) {
    _pub_path = path;
}

bool RSA_controller::generate_keys(int len, int bignum) {
    // 不能是空路径
    if (_pri_path.empty() || _pub_path.empty()) {
        log->log((*lang)["RSA_controller_invalid_key_path"]);
        return false;
    }
    RSA *rsa = RSA_new();
    BIGNUM *e = BN_new();

    BN_set_word(e, bignum);
    int ret = RSA_generate_key_ex(rsa, len, e, nullptr);
    if (ret == 0) {
        log->log((*lang)["RSA_controller_generate_error"] + ERR_reason_error_string(ERR_get_error()));
        return false;
    }

    FILE* f_pri = fopen(_pri_path.c_str(), "wb");
    FILE* f_pub = fopen(_pub_path.c_str(), "wb");
    // 写入公钥
    ret = PEM_write_RSAPublicKey(f_pub, rsa);
    if (ret == 0) {
        log->log((*lang)["RSA_controller_write_public_key_error"]);
        return false;
    }
    // 写入私钥
    ret = PEM_write_RSAPrivateKey(f_pri, rsa, 0, 0, 0, 0, 0);
    if (ret == 0) {
        log->log((*lang)["RSA_controller_write_private_key_error"]);
        return false;
    }
    // 关闭文件
    fclose(f_pri);
    fclose(f_pub);
    f_pri = nullptr;
    f_pub = nullptr;
    // 释放内存
    RSA_free(rsa);
    BN_free(e);
    return true;
}



RSA_Security::RSA_Security() {

}

void RSA_Security::startInit() {
    _rsa = RSA_new();
    log = Log::getInstance();
    lang = Language::getInstance();
}

void RSA_Security::init() {
    RSA_free(_rsa);
    _rsa = RSA_new();
}

RSA_Security::~RSA_Security() {
    RSA_free(_rsa);
}

bool RSA_Security::set_and_read_key(const std::string & str) {
    return set_key_path(str) && read_key();
}

RSA_encrypt::~RSA_encrypt() {

}

bool RSA_encrypt::read_key() {
    FILE * key_path = fopen(_key_path.c_str(), "r");
    if (key_path == nullptr) {
        log->log((*lang)["RSA_encrypt_open_file_error"]);
        return false;
    }
    // 读取公钥
    _rsa = PEM_read_RSAPublicKey(key_path, &_rsa, 0, 0);
    if (_rsa == nullptr) {
        fclose(key_path);
        key_path = nullptr;
        log->log((*lang)["RSA_encrypt_read_public_key_failed"] + ERR_reason_error_string(ERR_get_error()));
        return false;
    }
    // 关闭文件
    fclose(key_path);
    key_path = nullptr;
    return true;
}

bool RSA_encrypt::set_key_path(const std::string & path) {
    // 检测是否存在该文件
    if (access(path.c_str(), F_OK) != 0) {
        log->log((*lang)["RSA_encrypt_incorrect_path"]);
        return false;
    }
    _key_path = path;
    return true;
}

bool RSA_encrypt::convert() {
    int padding = RSA_PKCS1_PADDING;
    int ret = RSA_public_encrypt((int)strlen(_input_text), reinterpret_cast<const unsigned char *>(_input_text), _output_text, _rsa, padding);
    if (ret == -1) {
        log->log((*lang)["RSA_encrypt_convert_error"] + ERR_reason_error_string(ERR_get_error()));
        return false;
    }
    return true;
}

void RSA_encrypt::operator=(const char * str) {
    int len = strlen(str);
    memcpy(_input_text, str, len);
    // 将末尾置为0
    _input_text[len] = '\0';
    // 转换
    convert();
}

void RSA_encrypt::operator=(const std::string& str) {
    operator=(str.c_str());
}

RSA_encrypt::operator const unsigned char *() {
    return _output_text;
}

int RSA_encrypt::size() {
    return RSA_size(_rsa);
}

void RSA_encrypt::set_input(const std::string& str) {
    set_input(str.c_str());
}

void RSA_encrypt::set_input(const char * str) {
    strcpy(_input_text, str);
}

const unsigned char * RSA_encrypt::get_output() {
    return _output_text;
}


RSA_decrypt::~RSA_decrypt() {

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
    _rsa = PEM_read_RSAPrivateKey(key_path, &_rsa, 0, 0);
    if (_rsa == nullptr) {
        fclose(key_path);
        key_path = nullptr;
        log->log((*lang)["RSA_decrypt_read_public_key_failed"] + ERR_reason_error_string(ERR_get_error()));
        return false;
    }
    // 关闭文件
    fclose(key_path);
    key_path = nullptr;
    return true;
}
    /*
     * 设置私钥的路径
     */
bool RSA_decrypt::set_key_path(const std::string & path) {
    // 检测是否存在该文件
    if (access(path.c_str(), F_OK) != 0) {
        log->log((*lang)["RSA_decrypt_incorrect_path"]);
        return false;
    }
    _key_path = path;
    return true;
}
/*
* 将加密的信息解密
*/
bool RSA_decrypt::convert() {
    int padding = RSA_PKCS1_PADDING;
    // 清空上一次的信息
    memset(_output_text, '\0', sizeof _output_text);
    // 转换信息
    int ret = RSA_private_decrypt(RSA_size(_rsa), _input_text, reinterpret_cast<unsigned char *>(_output_text), _rsa, padding);
    // 查看返回值
    if (ret == -1) {
        log->log((*lang)["RSA_decrypt_convert_error"] + ERR_reason_error_string(ERR_get_error()));
        return false;
    }
    return true;
}
/*
* 读取被加密的信息，然后进行解密
*/
void RSA_decrypt::operator=(const unsigned char * str) {
    memcpy(_input_text, str, RSA_size(_rsa));
    convert();
}

/*
 * 强制类型转换
 */
RSA_decrypt::operator char*() {
    return _output_text;
}

RSA_decrypt::operator std::string() {
    return _output_text;
}

int RSA_decrypt::size() {
    return RSA_size(_rsa);
}


void RSA_decrypt::set_input(const unsigned char * str, int len) {
    memcpy(_input_text, str, len);
}

const char * RSA_decrypt::get_output() {
    return _output_text;
}

