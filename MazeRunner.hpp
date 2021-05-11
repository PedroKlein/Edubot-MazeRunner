#ifndef __MAZE_RUNNER__
#define __MAZE_RUNNER__

#include <climits>
#include <list>
#include "Node.hpp"
#include "Robot.hpp"
#include "Globals.hpp"

using std::list;

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

    void setCurrentNode(Node *node)
    {
        currentNode = node;
    }

    void setTargetNode(Node *node)
    {
        targetNode = node;
    }

    Node *addNode()
    {
        Node node;
        nodes.push_back(node);
        return &nodes.back();
    }

    Node *updateCurrentNode()
    {
        bool avaliableDir[DIRECTION_QTY] = {runner->getAvaliableDir()};
        Direction currentNodeDir;

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

        return currentNode;
    }

    float calculateDistanceFromCurrentNode()
    {
        Coordinate currentPos = runner->getPos();
        Coordinate currentNodePos = currentNode->getCoordinate();
        return ABS(currentPos.x) > ABS(currentNodePos.x) + runner->getSafeDistance() ? ABS(currentPos.x) + ABS(currentNodePos.x) : ABS(currentPos.y) + ABS(currentNodePos.y);
    }
};

#endif