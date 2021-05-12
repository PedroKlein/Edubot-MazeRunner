#ifndef __MAZE_RUNNER__
#define __MAZE_RUNNER__

#include <iostream>
#include <climits>
#include <list>
#include "Node.hpp"
#include "Robot.hpp"
#include "Globals.hpp"

using namespace std;

class MazeRunner
{

private:
    list<Node> nodes;
    Node *currentNode;
    Node *targetNode;
    Robot *runner;

public:
    ~MazeRunner() {}

    MazeRunner(Robot *robot)
    {
        runner = robot;
        Node initNode;
        initNode.setCoordinate(runner->getPos());
        initNode.setVisited(true);
        initNode.setDirection(addNode(), RIGHT_DIR, INT_MAX);
        nodes.push_back(initNode);
        currentNode = &nodes.back();
    }

    Robot *getRunner()
    {
        return runner;
    }

    list<Node> getNodes()
    {
        return nodes;
    }

    Node *getCurrentNode()
    {
        return currentNode;
    }

    void setCurrentNode(Node *node)
    {
        currentNode = node;
    }

    void updateTargetNodeOnDirection()
    {
        targetNode = currentNode->getDirections()[(int)runner->getDirection()].getNode();
    }

    Node *addNode()
    {
        Node node;
        nodes.push_back(node);
        return &nodes.back();
    }

    void updateCurrentNode()
    {
        bool avaliableDir[DIRECTION_QTY] = {false};
        Direction currentNodeDir;

        runner->getAvaliableDir(avaliableDir);


        switch (runner->getDirection())
        {
        case RIGHT_DIR:
            currentNodeDir = LEFT_DIR;
            break;
        case DOWN_DIR:
            currentNodeDir = TOP_DIR;
            break;
        case LEFT_DIR:
            currentNodeDir = RIGHT_DIR;
            break;
        default:
            currentNodeDir = DOWN_DIR;
            break;
        }

        targetNode->setCoordinate(runner->getPos());

        for (int i = 0; i < DIRECTION_QTY; i++)
        {
            if (avaliableDir[i])
            {
                targetNode->setDirection(addNode(), (Direction)i, INT_MAX);
            }
        }
        targetNode->setDirection(currentNode, currentNodeDir, calculateDistanceFromCurrentNode());
        currentNode = targetNode;
    }

    float calculateDistanceFromCurrentNode()
    {
        const Coordinate currentPos = runner->getPos();
        const Coordinate currentNodePos = currentNode->getCoordinate();
        return currentPos.distanceBetween(currentNodePos);
    }

    //DEBUG
    void printNodes()
    {
        for (auto it : nodes)
        {
            Coordinate pos = it.getCoordinate();
            cout << "X: " << pos.x << " Y: " << pos.y << " isVisited: " << it.isVisited() << endl;
        }
    }

    void printCurrentNode()
    {
        Coordinate pos = currentNode->getCoordinate();
        cout << "X: " << pos.x << " Y: " << pos.y << " isVisited: " << currentNode->isVisited() << endl
             << endl;
        for (auto it : currentNode->getDirections())
        {
            if (it.isReachable())
            {
                Coordinate pos = it.getNode()->getCoordinate();
                cout << "X: " << pos.x << " Y: " << pos.y << " Distance: " << it.getLength() << endl;
            }
        }
    }
};

#endif