#ifndef __NODE__
#define __NODE__

#include <climits>
#include <list>
#include <array>
#include <utility>
#include "Globals.hpp"

using std::array;
using std::list;

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

    bool isEqual(Node *node)
    {
        return node->pos.isEqual(this->pos);
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

    pair<list<Direction>, float> shortestPathToNode(Node *node)
    {
        
    }

    list<Direction> getDirectionsToNewPath()
    {
        list<Direction> directionsToNewPath;
        Node *currentNode = this, *initialNode = this;

        while (currentNode->getNewPathDir() == DIRECTION_QTY)
        {
            float minDistance = INT_MAX;
            Direction selectedDir = DIRECTION_QTY;

            for (int i = 0; i < DIRECTION_QTY; ++i)
            {
                Path currentPath = getDirections()[i];

                if (currentPath.isReachable() && !currentPath.getNode()->isEqual(currentNode) && minDistance > currentPath.getLength())
                {
                    minDistance = currentPath.getLength();
                    selectedDir = (Direction)i;
                    currentNode = currentPath.getNode();
                }
            }

            if (currentNode->isEqual(initialNode))
            {
            }

            directionsToNewPath.push_back(selectedDir);
        }

        directionsToNewPath.push_back(currentNode->getNewPathDir());

        return directionsToNewPath;
    }
};

#endif