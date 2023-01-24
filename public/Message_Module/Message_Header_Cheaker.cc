#include "Message_Header_Checker.h"

std::set<valid_header> Message_Header_Checker::store;
/*
 * 检查是否有效
 */
bool Message_Header_Checker::check(valid_header && header) {
    if (store.contains(std::move(header)))
        return true;
    return false;
}

Message_Header_Checker::Message_Header_Checker() {
    store.emplace(std::move((valid_header){Message_Header::RECALL, Message_Header::NOMEAN, Message_Header::NOMEAN}));

    store.emplace(std::move((valid_header){Message_Header::TEXT_CHAT, Message_Header::USER, Message_Header::NOMEAN}));
    store.emplace(std::move((valid_header){Message_Header::TEXT_CHAT, Message_Header::GROUP, Message_Header::NOMEAN}));

    store.emplace(std::move((valid_header){Message_Header::TEXT_SYS, Message_Header::INIT_SYS, Message_Header::NOMEAN}));
    store.emplace(std::move((valid_header){Message_Header::TEXT_SYS, Message_Header::TIME, Message_Header::NOMEAN}));
    store.emplace(std::move((valid_header){Message_Header::TEXT_SYS, Message_Header::ADD_USER, Message_Header::NOMEAN}));
    store.emplace(std::move((valid_header){Message_Header::TEXT_SYS, Message_Header::GROUP, Message_Header::JOIN}));
    store.emplace(std::move((valid_header){Message_Header::TEXT_SYS, Message_Header::GROUP, Message_Header::LEAVE}));

    store.emplace(std::move((valid_header){Message_Header::REQUEST, Message_Header::ACCESS, Message_Header::NOMEAN}));
    store.emplace(std::move((valid_header){Message_Header::REQUEST, Message_Header::REJECT, Message_Header::NOMEAN}));


}
