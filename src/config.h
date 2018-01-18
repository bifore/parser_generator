#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <unordered_map>

#include "node.h"

struct Config
{
    std::unordered_map<std::string, std::string> values;

    Config(std::string name);
    std::string get(std::string name);
    std::string get(std::string name, Node* node);
    std::string get(std::string name, Node* node, char _iterator);
    std::string get(std::string name, Node* node, std::string prefix);
    std::string get(std::string name, Node* node, std::string prefix, char _iterator);
};

#endif /* CONFIG_H */
