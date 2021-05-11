#ifndef __MAZE__
#define __MAZE__

#include <climits>
#include <list>
#include "Node.hpp"
#include "Globals.hpp"

using std::list;

class Maze
{

private:
    list<Node> nodes;
    Node *currentNode;

public:
    ~Maze() {}

    Maze(Coordinate initPos)
    {
        Node *initNode = addNode(initPos);
        Node *rightDir = addEmptyNode();
        initNode->setVisited(true);
        initNode->setRight(rightDir, INT_MAX);
        currentNode = initNode;
    }

    list<Node> getNodes()
    {
        return nodes;
    }

    Node *addNode(Coordinate pos)
    {
        Node node;
        node.setCoordinate(pos);
        nodes.push_back(node);
        return &nodes.back();
    }

    Node *addEmptyNode()
    {
        Node node;
        nodes.push_back(node);
        return &nodes.back();
    }
};

#endif