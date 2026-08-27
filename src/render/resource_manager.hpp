#ifndef RENDER_RESOURCE_MANAGER_HPP
#define RENDER_RESOURCE_MANAGER_HPP

#include <functional>
#include <unordered_map>
#include <string>
#include <iostream>

// ResourceManager class to cache resource files
template<typename T>
class ResourceManager {
private:
    std::function<T(const std::string&)> loadFunc;
    std::unordered_map<std::string, T> cache;

public:
    ResourceManager(std::function<T(const std::string&)> loader)
        : loadFunc(loader) {}
    
    // Get a cached (or new) file's contents
    T get(const std::string& key) {
        auto iter = cache.find(key);
        if (iter != cache.end()) {
            return iter->second;
        }
        T resource = loadFunc(key);
        cache[key] = resource;
        return resource;
    }
};

#endif