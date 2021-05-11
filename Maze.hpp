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
    Node *initNode;

public:
    ~Maze() {};

    Maze(Coordinate initPos)
    {
        *initNode = new Node(initPos);
        Node *rightDir = new Node();

        initNode->setVisited(true);
        initNode->setRight(rightDir, INT_MAX);
    };

    *Node getInitNode()
    {
        return initNode;
    }
};

#endif