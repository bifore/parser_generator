#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <ctype.h>

#include "config.h"
#include "node.h"

using namespace std;

void get_token(vector<string> &tokens, string file_path)
{
    ifstream file(file_path.c_str());
    string current_token;
    char c;
    while(file.get(c))
    {
        if(isspace(c))
        {
            if(current_token.size())
                tokens.push_back(current_token);
            current_token = "";
            continue;
        }
        switch(c)
        {
            case '[': case ']': case '{': case '}': case ':':
                if(current_token.size())
                    tokens.push_back(current_token);
                tokens.push_back(string(1, c));
                current_token = "";
                break;
            default:
                current_token.push_back(c);
                break;
        }
    }
    if(current_token.size())
        tokens.push_back(current_token);
    file.close();
}

Node* generateSyntaxTree(vector<string> &tokens)
{
    Node* current_node = new Node();
    string type;
    for(int i = 0; i < tokens.size(); ++i)
    {
        string token = tokens[i];
        if(token == "{")
            current_node = current_node->children.back();
        else if(token == "}")
        {
            current_node = current_node->parent;
            if(tokens[i + 1] == "[")
            {
                i += 3;
                current_node->children.back()->array_length = tokens[i - 1];
            }
        }
        else if(tokens[i + 1] == ":")
        {
            type = token;
            ++i;
        }
        else if(tokens[i + 1] == "[")
        {
            Node* new_node = new Node(token, type, current_node);
            type = "";
            current_node->children.push_back(new_node);
            ++i;
        }
        else if(tokens[i + 1] == "]")
        {
            current_node->children.back()->array_length = token;
            ++i;
        }
        else
        {
            Node* new_node = new Node(token, type, current_node);
            type = "";
            current_node->children.push_back(new_node);
        }
    }
    return current_node;
}

void declare(Node* root, Config cfg)
{
    for(Node* node : root->children)
        if(node->children.size())
        {
            cout << cfg.get("struct_begin", node);
            declare(node, cfg);
            cout << cfg.get("struct_end", node);
        }
    for(Node* node : root->children)
        if(node->name[0] != '_')
        {
            if(node->array_length.size())
                cout << cfg.get("array_declaration", node);
            else
                cout << cfg.get("var_declaration", node);
        }
}

void parser(Node * root, string prefix, char i, Config cfg)
{
    for(Node* node : root->children)
    {
        if(node->array_length.size())
        {
            cout << cfg.get("array_init", node, prefix);
            cout << cfg.get("loop_begin", node, i);
            if(node->children.size())
            {
                string new_prefix = cfg.get("array_element", node, i);
                parser(node, prefix + new_prefix, i + 1, cfg);
            }
            else
                cout << cfg.get("fetch_array", node, prefix, i);
            cout << cfg.get("loop_end", node);
            continue;
        }
        if(node->name[0] == '_')
        {
            cout << cfg.get("var_declaration", node);
            cout << cfg.get("fetch_var", node);
            continue;
        }
        if(node->children.size())
        {
            string new_prefix = node->name + cfg.get("struct_element", node);
            parser(node, prefix + new_prefix, i + 1, cfg);
        }
        else
            cout << cfg.get("fetch_var", node, prefix);
    }
}

void shift_tree_depth(Node* root)
{
    for(Node* child : root->children)
        shift_tree_depth(child);
    ++root->depth;
}

int main()
{
    Config cfg("python");
    vector<string> tokens;
    get_token(tokens, "goal");
    Node* root = generateSyntaxTree(tokens);
    cout << cfg.get("include", root);
    declare(root, cfg);
    shift_tree_depth(root);
    cout << cfg.get("main_func_begin", root);
    parser(root, "", 'i', cfg);
    cout << cfg.get("main_func_end", root);
}
