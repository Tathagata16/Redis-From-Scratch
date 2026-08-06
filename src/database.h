#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <unordered_map>
#include <chrono>

class Database
{
public:
    bool set(const std::string& key, const std::string& value);

    bool get(const std::string& key, std::string& value) ;

    bool remove(const std::string& key);

    bool exists(const std::string& key)const;

    bool expire(const std::string& key, int seconds);

    bool isExpired(const std::string& key);

private:
    std::unordered_map<std::string, std::string> storage;

    std::unordered_map<std::string, std::chrono::system_clock::time_point> expirationTimes;
};



#endif