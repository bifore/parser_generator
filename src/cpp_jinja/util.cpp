#include "util.h"

#include <algorithm> 

using namespace std;

string util::trim(string s)
{
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](int ch)
    {
        return !isspace(ch);
    }));
    s.erase(find_if(s.rbegin(), s.rend(), [](int ch)
    {
        return !isspace(ch);
    }).base(), s.end());
    return s;
}

vector<string> util::split(const string& s, const string& delimiter)
{
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = s.find(delimiter, prev);
        if(pos == string::npos)
            pos = s.length();
        string token = s.substr(prev, pos - prev);
        if(!token.empty())
            tokens.push_back(token);
        prev = pos + delimiter.length();
    }
    while(pos < s.length() && prev < s.length());
    return tokens;
}

bool util::starts_with(const string& s, const string& b)
{
    if(s.size() < b.size())
        return false;
    for(size_t i = 0; i < b.size(); ++i)
        if(s[i] != b[i])
            return false;
    return true;
}
