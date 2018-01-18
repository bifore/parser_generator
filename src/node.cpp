#include "node.h"

using namespace std;

Node::Node() { }

Node::Node(string name, string type, Node* parent)
{
    this->name = name;
    this->type = type;
    this->parent = parent;
}
