#ifndef CONTEXT_H
#define CONTEXT_H

#include <unordered_map>
#include <string>
#include <vector>

namespace jinja
{
    class Context
    {
        std::string value;
        std::unordered_map<std::string, Context*> children;
        std::vector<Context*> array;

    public:
        void set(std::string key, std::string value);
        void set(std::string key, std::vector<std::string> values);
        void set(std::string key, Context* value);
        std::string get();
        std::vector<Context*> get_array();
        Context* get(const std::string& key);
        void erase(const std::string& key);
    };
}

#endif /* ! CONTEXT_H */
