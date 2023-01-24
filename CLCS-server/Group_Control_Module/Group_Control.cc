#include "Group_Control.h"

Group_Control::Group_Control() {
    // 默认最大的人数为50
    _max_user = 50;
}

bool Group_Control::set_epoll_reactor(std::shared_ptr<Epoll_Reactor> ptr) {
    _epoll_reactor = ptr;
    return true;
}


bool Group_Control::add(std::shared_ptr<Event> event) {
    std::lock_guard<std::mutex> guard(_group_lock);
    // 当群组满人的时候，就不能进入了
    if (_user_store.size() >= _max_user) {
        return false;
    }
    // 添加到群组中
    _user_store.emplace(std::move(event->get_uuid()), event);
    // 状态设置为在群组中
    event->set_in_group(true);
    return true;
}

bool Group_Control::erase(std::shared_ptr<Event> event) {
    std::lock_guard<std::mutex> guard(_group_lock);
    // 删除一个用户
    _user_store.erase(std::move(event->get_uuid()));
    // 状态设置为不在群组中
    event->set_in_group(false);
    return true;
}

void Group_Control::speak(std::shared_ptr<Event> event, std::shared_ptr<Message_Stream> message) {
    std::lock_guard<std::mutex> guard(_group_lock);
    // 自己继续监听，目标用户更改状态
#ifdef ET
    event->set_event_status(EPOLLIN | EPOLLET);
#else
    ptr->set_event_status(EPOLLIN);
#endif
    event->set_function(Epoll_Reactor_Service::receive_message, event);
    _epoll_reactor->epoll_add_event(event);


    for (auto item : _user_store) {
        // 目标用户
        std::shared_ptr<Event>& target = item.second;
        // 如果发送的是自己，则跳过
        if (target == event) {
            continue;
        }
        // 获取目标消息的缓冲区
        auto tar_buf = target->get_message_buf();
        // 添加消息
        tar_buf->push_send_message(message->copy());

        // 将目标客户由接收状态改为发送状态
        // 先删除
        _epoll_reactor->epoll_erase_event(target);
        // 改为发送状态
#ifdef ET
        target->set_event_status(EPOLLOUT | EPOLLET);
#else
        target->set_event_status(EPOLLOUT);
#endif
        target->set_function(Epoll_Reactor_Service::send_message, target);
        _epoll_reactor->epoll_add_event(target);
    }

}

void Group_Control::set_max_user(unsigned int nums) {
    _max_user = nums;
}

unsigned int Group_Control::get_max_user() {
    return _max_user;
}