#include "config.h"

#include <iostream>
#include <fstream>

#include "cpp_jinja/context.h"
#include "cpp_jinja/jinja.h"

using namespace std;

string replace_all(string &input, string s, string r)
{
    string output(input);
    for (size_t i = 0;;)
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
    string _path = "./config/" + name + ".cfg";
    ifstream file(_path.c_str());
    string line;
    string vname;
    string value;
    while (getline(file, line))
    {
        if(line[0] != ' ')
        {
            if(value.size() && vname.size())
                values[vname] = value.substr(0, value.size() - 1);
            vname = line;
            value = "";
            continue;
        }
        value += line.substr(4) + "\n";
    }
    if(value.size() && vname.size())
        values[vname] = value.substr(0, value.size() - 1);
    file.close();
}

string Config::get(string name)
{
    string value = values[name];
    return value;
}

jinja::Context get_context(Node* node, Config* config)
{
    jinja::Context* cnode = new jinja::Context();
    string default_type = config->get("default_type");
    cnode->set("type", node->type.size() ? node->type : default_type);
    string shift, indentation = config->get("indentation");
    cnode->set("depth", to_string(node->depth));
    for(int i = 0; i < node->depth; ++i)
        shift += indentation;
    cnode->set("shift", shift);
    cnode->set("size", node->array_length);
    cnode->set("name", node->name);
    jinja::Context context;
    context.set("node", cnode);
    return context;
}

string Config::get(string name, Node* node)
{
    string value = values[name];
    jinja::Context context = get_context(node, this);
    return jinja::parse(value, context);
}

string Config::get(string name, Node* node, char _iterator)
{
    string value = values[name];
    jinja::Context context = get_context(node, this);
    context.set("iterator", string(1, _iterator));
    return jinja::parse(value, context);
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
    string value = values[name];
    node->name = prefix + node->name;
    jinja::Context context = get_context(node, this);
    context.set("iterator", string(1, _iterator));
    string result = jinja::parse(value, context);
    node->name = node->name.substr(prefix.size());
    return result;
}
