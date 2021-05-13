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

    Node *getNodeWithin(Coordinate pos)
    {
        for (auto it = nodes.begin(); it != nodes.end(); ++it)
        {
            if (pos.isWithinCircle(runner->getSafeDistance() * 2, it->getCoordinate()))
            {
                runner->stop();
                return &*it;
            }
        }
        return nullptr;
    }

    void updateCurrentNode()
    {
        bool avaliableDir[DIRECTION_QTY] = {false};
        Direction currentNodeDir;
        Coordinate updatedPos = runner->getPos();

        switch (runner->getDirection())
        {
        case RIGHT_DIR:
            currentNodeDir = LEFT_DIR;
            updatedPos.x += (runner->getSafeDistance() + HALF_ROBOT_SIZE);
            break;
        case DOWN_DIR:
            currentNodeDir = TOP_DIR;
            updatedPos.y -= (runner->getSafeDistance() + HALF_ROBOT_SIZE);
            break;
        case LEFT_DIR:
            currentNodeDir = RIGHT_DIR;
            updatedPos.x -= (runner->getSafeDistance() + HALF_ROBOT_SIZE);
            break;
        default:
            currentNodeDir = DOWN_DIR;
            updatedPos.y += (runner->getSafeDistance() + HALF_ROBOT_SIZE);
            break;
        }

        printNode(getNodeWithin(updatedPos));

        targetNode->setCoordinate(updatedPos);

        runner->getAvaliableDir(avaliableDir);

        for (int i = 0; i < DIRECTION_QTY; i++)
        {
            if (avaliableDir[i])
            {
                targetNode->setDirection(addNode(), (Direction)i, INT_MAX);
            }
        }
        targetNode->setVisited(true);
        targetNode->setDirection(currentNode, currentNodeDir, currentNode->getDistanceToNode(targetNode));
        currentNode = targetNode;
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

    void printNode(Node *node)
    {
        if (!node)
            return;

        Coordinate pos = node->getCoordinate();
        cout << "X: " << pos.x << " Y: " << pos.y << " isVisited: " << node->isVisited() << endl
             << endl;
        for (auto it : node->getDirections())
        {
            if (it.isReachable())
            {
                Coordinate pos = it.getNode()->getCoordinate();
                cout << "X: " << pos.x << " Y: " << pos.y << " Distance: " << it.getLength() << endl;
            }
        }
        cout << endl;
    }
};

#endif