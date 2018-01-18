#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <ctype.h>

using namespace std;

struct Node
{
    string name, type, array_length;
    vector<Node*> children;
    Node* parent;

    Node() {};
    Node(string name, string type, Node* parent)
    {
        this->name = name;
        this->type = type;
        this->parent = parent;
    };
};

void get_token(vector<string> &tokens, string file_path)
{
    ifstream file(file_path.c_str());
    string current_token = "";
    char c;
    while(file.get(c))
    {
        if(isspace(c))
        {
            if(current_token.size() != 0)
                tokens.push_back(current_token);
            current_token = "";
            continue;
        }
        switch(c)
        {
            case '[': case ']': case '{': case '}': case ':':
                if(current_token.size() != 0)
                    tokens.push_back(current_token);
                tokens.push_back(string(1, c));
                current_token = "";
                break;
            default:
                current_token.push_back(c);
                break;
        }
    }
    if(current_token.size() != 0)
        tokens.push_back(current_token);
    file.close();
}

Node* generateSyntaxTree(vector<string> &tokens)
{
    Node* current_node = new Node();
    string type = "";
    for(int i = 0; i < tokens.size(); ++i)
    {
        string token = tokens[i];
        if(token == "{")
        {
            current_node = current_node->children.back();
            continue;
        }
        else if(token == "}")
        {
            current_node = current_node->parent;
            if(tokens[i + 1] == "[")
            {
                i += 3;
                current_node->children.back()->array_length = tokens[i - 1];
            }
            else
                continue;
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

void declare(Node* root, string ind)
{
    for(Node* node : root->children)
        if(node->children.size())
        {
            cout << ind << "struct ";
            cout << (node->type.size() ? node->type : "int");
            cout << endl << ind << "{" << endl;
            declare(node, ind + "    ");
            cout << ind << "};" << endl;
        }
    for(Node* node : root->children)
        if(node->name[0] != '_')
        {
            if(node->array_length.size())
            {
                cout << ind << "vector<";
                cout << (node->type.size() ? node->type : "int") << ">";
            }
            else
                cout << ind << (node->type.size() ? node->type : "int");
            cout << " " << node->name;
            cout << ";" << endl;
        }
}

void parser(Node * root, string prefix, char i, string ind)
{
    for(Node* node : root->children)
    {
        if(node->array_length.size())
        {
            cout << ind << prefix << node->name << ".resize(";
            cout << node->array_length << ");" << endl;
            cout << ind << "for(int " << i << " = 0; " << i << " < ";
            cout << node->array_length << "; ++" << i << ")";
            cout << endl << ind << "{" << endl;
            if(node->children.size())
            {
                string new_prefix = node->name + "[" + to_string(i) + "].";
                parser(node, prefix + new_prefix, i + 1, ind + "    ");
            }
            else
            {
                cout << ind << "    " << "cin >> " << prefix << node->name;
                cout << "[" << i << "];" << endl;
            }
            cout << ind << "}" << endl;
            continue;
        }
        if(node->name[0] == '_')
        {
            cout << ind << (node->type.size() ? node->type : "int");
            cout << " " << node->name << ";" << endl;
            cout << ind << "cin >> " << node->name << ";" << endl;
            continue;
        }
        if(node->children.size())
            parser(node, prefix + node->name + ".", i + 1, ind);
        else
            cout << ind << "cin >> " << prefix << node->name << ";" << endl;
    }
}

int main()
{
    vector<string> tokens;
    get_token(tokens, "goal");
    Node* root = generateSyntaxTree(tokens);
    cout << "#include <iostream>" << endl;
    cout << "#include <vector>" << endl;
    cout << "using namespace std;" << endl;
    declare(root, "");
    cout << "int main()" << endl;
    cout << "{" << endl;
    parser(root, "", 'i', "    ");
    cout << "}" << endl;
}
