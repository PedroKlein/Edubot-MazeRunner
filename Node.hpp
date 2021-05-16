#ifndef __NODE__
#define __NODE__

#include <climits>
#include <iostream>
#include <array>
#include "Globals.hpp"

using std::array;
using namespace std;

class Node
{
public:
    class Path
    {

    private:
        float len = (float)INT_MAX;
        Node *node = nullptr;
        friend class Node;

    public:
        Path() {}

        bool isReachable() const
        {
            return node;
        }

        float getLength() const
        {
            return len;
        }

        Node *getNode() const
        {
            return node;
        }

        void set(Node *const n, const float length)
        {
            node = n;
            len = length;
        }

        ~Path()
        {
            node = nullptr;
        }
    };

private:
    bool visited = false;
    array<Path, DIRECTION_QTY> directions;
    Coordinate pos;

public:
    Node() : pos(Coordinate(INT_MAX, INT_MAX)){};
    Node(Coordinate pos) : pos(pos){};
    ~Node(){};

    array<Path, DIRECTION_QTY> &getDirections()
    {
        return directions;
    }

    Coordinate getCoordinate() const
    {
        return pos;
    }

    void setCoordinate(Coordinate coordinate)
    {
        pos.x = coordinate.x;
        pos.y = coordinate.y;
    }

    void setDirection(Node *n, Direction dir, float len = 1)
    {
        directions[dir].set(n, len);
    }

    void setVisited(bool b)
    {
        visited = b;
    }

    bool isVisited()
    {
        return visited;
    }

    float getDistanceToNode(Node *destNode) const
    {
        return pos.distanceBetween(destNode->getCoordinate());
    }

    void linkToNode(Node *node, Direction dir)
    {
        Direction oppositDir = (Direction)(dir + 2 >= DIRECTION_QTY ? dir - 2 : dir + 2);
        float distanceBetween = getDistanceToNode(node);
        if (distanceBetween == 0)
        {
            return;
        }
        getDirections()[dir].set(node, distanceBetween);
        node->getDirections()[oppositDir].set(this, distanceBetween);
    }

    Direction getNewPathDir()
    {
        for (int i = 0; i < DIRECTION_QTY; ++i)
        {
            Path currentPath = getDirections()[i];
            if (currentPath.isReachable() && !currentPath.getNode()->isVisited())
            {
                return (Direction)i;
            }
        }

        return DIRECTION_QTY;
    }

    Path &connectingEdge(Node *n)
    {
        for (int i = 0; i < DIRECTION_QTY; ++i)
        {
            if (getDirections()[i].getNode() == n)
            {
                return getDirections()[i];
            }
        }
        throw;
    }
};

#endif