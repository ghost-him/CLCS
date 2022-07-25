#include "Business.h"

void Business::start_accept_from_server() {
    MessageReceiver* mr = MessageReceiver::getInstance();
    mr->receive_message();
    std::cout << mr->get_content() << std::endl;
}


