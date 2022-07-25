#include "User.h"

char *User::get_uuid() {
    return _uuid;
}
void User::set_uuid(const char * uuid) {
    strcpy(_uuid, uuid);
}