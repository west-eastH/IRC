
#ifndef IRC_CHANNEL_HPP
#define IRC_CHANNEL_HPP

#include <iostream>
#include <map>

class Channel {
private:
    std::string _name;
    std::string _key;
    std::string _topic;
    long long   _limit;
    size_t      _userCount;

public:
    Channel();
    Channel(const std::string &name, const std::string &key, const std::string &topic, long long int limit,
            size_t userCount);
    virtual ~Channel();

};


#endif //IRC_CHANNEL_HPP
