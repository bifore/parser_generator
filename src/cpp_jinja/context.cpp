#include "context.h"

using namespace std;

namespace jinja
{
    void Context::set(string key, string value)
    {
        Context* context = new Context();
        context->value = value;
        children[key] = context;
    }

    void Context::set(string key, vector<string> values)
    {
        Context* context = new Context();
        for(string value : values)
        {
            Context* c = new Context();
            c->value = value;
            context->array.push_back(c);
        }
        children[key] = context;
    }

    void Context::set(string key, Context* value)
    {
        children[key] = value;
    }

    string Context::get()
    {
        return value;
    }

    vector<Context*> Context::get_array()
    {
        return array;
    }

    Context* Context::get(const string& key)
    {
        size_t pos = key.find('.');
        if(pos != string::npos)
            return children[key.substr(0, pos)]->get(key.substr(pos + 1));
        return children[key];
    }

    void Context::erase(const string& key)
    {
        children.erase(key);
    }
}
