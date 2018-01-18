#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>

struct Node
{
    std::string name, type, array_length;
    int depth;
    std::vector<Node*> children;
    Node* parent;

    Node();
    Node(std::string name, std::string type, Node* parent);
};

#endif /* NODE_H */
