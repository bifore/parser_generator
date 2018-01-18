#include "config.h"

#include <iostream>
#include <fstream>

using namespace std;

string replace_all(string &input, string s, string r)
{
    string output(input);
    for (int i = 0;;)
    {
        i = output.find(s, i);
        if (i == string::npos)
            break;
        output.replace(i, s.size(), r);
    }
    return output;
}

Config::Config(string name)
{
    string _path = "config/" + name + ".cfg";
    ifstream file(_path.c_str());
    string line;
    while (getline(file, line))
    {
        int sep = line.find('=');
        string val_name = line.substr(0, sep);
        string value = line.substr(sep + 1);
        values[val_name] = replace_all(value, "\\n", "\n");
    }
    file.close();
}

string Config::get(string name)
{
    string value = values[name];
    return value;
}

string Config::get(string name, Node* node)
{
    string value = values[name];
    if(value.find("{{type}}") != string::npos)
    {
        string type = (node->type.size()
                ? node->type
                : get("default_type", node));
        value = replace_all(value, "{{type}}", type);
    }
    if(value.find("{{shift}}") != string::npos)
    {
        string shift, indentation = get("indentation", node);
        for(int i = 0; i < node->depth; ++i)
            shift += indentation;
        value = replace_all(value, "{{shift}}", shift);
    }
    value = replace_all(value, "{{size}}", node->array_length);
    value = replace_all(value, "{{name}}", node->name);
    return value;
}

string Config::get(string name, Node* node, char _iterator)
{
    string value = get(name, node);
    value = replace_all(value, "{{iterator}}", string(1, _iterator));
    return value;
}

string Config::get(string name, Node* node, string prefix)
{
    node->name = prefix + node->name;
    string value = get(name, node);
    node->name = node->name.substr(prefix.size());
    return value;
}

string Config::get(string name, Node* node, string prefix, char _iterator)
{
    node->name = prefix + node->name;
    string value = get(name, node);
    node->name = node->name.substr(prefix.size());
    value = replace_all(value, "{{iterator}}", string(1, _iterator));
    return value;
}
