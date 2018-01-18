#include "node.h"

using namespace std;

Node::Node()
{
    this->depth = -1;
}

Node::Node(string name, string type, Node* parent)
{
    this->name = name;
    this->type = type;
    this->parent = parent;
    this->depth = parent->depth + 1;
}
