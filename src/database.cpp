#include "database.h"

bool Database::set(const std::string& key, const std::string& value){
    storage[key] = value;
    return true;
}

bool Database::get(const std::string& key, std::string& value) 
{
    if(isExpired(key)){
        return false;
    }

    auto it = storage.find(key);

    if(it == storage.end()){
        return false;
    }

    value = it->second;
    return true;
}

bool Database::remove(const std::string& key)
{
    return storage.erase(key) > 0;
}

bool Database::exists(const std::string& key)const
{
    return storage.find(key) != storage.end();
}

bool Database::expire(const std::string& key, int seconds){
    if(!exists(key)){
        return false;
    }

    expirationTimes[key] = 
    std::chrono::system_clock::now()
    + std::chrono::seconds(seconds);

    return true;
}

bool Database::isExpired(const std::string& key) 
{
    auto it = expirationTimes.find(key);

    if(it == expirationTimes.end())
    {
        return false;
    }

    if(std::chrono::system_clock::now() >= it->second)
    {
        storage.erase(key);
        expirationTimes.erase(it);

        return true;
    }

    return false;
}

