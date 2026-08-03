#include "database.h"

bool Database::set(const std::string& key, const std::string& value){
    storage[key] = value;
    return true;
}

bool Database::get(const std::string& key, std::string& value)const 
{
    auto it = storage.find(key);

    if(it == storage.end()){
        return false;
    }

    value = it->second;
    return true;
}