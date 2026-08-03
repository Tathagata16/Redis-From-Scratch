#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <unordered_map>

class Database
{
public:
    bool set(const std::string& key, const std::string& value);

    bool get(const std::string& key, std::string& value) const;

    bool remove(const std::string& key);

    bool exists(const std::string& key)const;

private:
    std::unordered_map<std::string, std::string> storage;
};



#endif