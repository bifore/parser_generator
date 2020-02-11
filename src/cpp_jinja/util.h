#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

namespace util
{
    std::string trim(std::string s);
    std::vector<std::string> split(const std::string& s, const std::string& d);
    bool starts_with(const std::string& s, const std::string& b);
}

#endif /* ! UTIL_H */
