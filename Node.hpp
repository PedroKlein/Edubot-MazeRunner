#ifndef __NODE__
#define __NODE__

#include <climits>
#include <array>
#include "Globals.hpp"

using std::array;

class Node
{
public:
    class Path
    {

    private:
        float len = INT_MAX;
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

        Node *getNode()
        {
            return node;
        }

        void set(Node *n, int length)
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
    Node() : pos(Coordinate(-2., -2.)){};
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

    void setDirection(Node *n, Direction dir, int len = 1)
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

    Path &connectingEdge(Node *to)
    {
        for (int i = 0; i < DIRECTION_QTY; ++i)
        {
            if (getDirections()[i].getNode() == to)
            {
                return getDirections()[i];
            }
        }
        throw;
    }
};

#endif