//
// Created by Jongmin Han on 1/21/24.
//

#include "Channel.hpp"

Channel::Channel(const std::string &name, const std::string &key, const std::string &topic, long long int limit,
                 size_t userCount) : _name(name), _key(key), _topic(topic), _limit(limit), _userCount(userCount) {}

Channel::~Channel() {

}

Channel::Channel() {}


