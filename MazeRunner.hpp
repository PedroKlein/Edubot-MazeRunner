#ifndef __MAZE_RUNNER__
#define __MAZE_RUNNER__

#include <iostream>
#include <climits>
#include <list>
#include "Node.hpp"
#include "Robot.hpp"
#include "Coordinate.hpp"

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

    //inicia labirinto com o primeirto node na posicao do robo e com um caminho para a direita.
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

    Robot *getRunner() const
    {
        return runner;
    }

    list<Node> getNodes() const
    {
        return nodes;
    }

    Node *getCurrentNode() const
    {
        return currentNode;
    }

    Node *getTargetNode() const
    {
        return targetNode;
    }

    void setCurrentNode(Node *node)
    {
        currentNode = node;
    }

    // atualiza target node para o node ao qual o robo esta apontando
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

    // procura um node dentro da posicao do runner, se tiver,
    // conecta com o current node e entao o current node vira ele
    Node *getNodeWithin(Coordinate pos)
    {
        for (auto it = nodes.begin(); it != nodes.end(); ++it)
        {
            if (pos.isWithinCircle(runner->getSafeDistance() + HALF_ROBOT_SIZE, it->getCoordinate()))
            {
                currentNode->linkToNode(&*it, runner->getDirection());
                currentNode = &*it;
                return &*it;
            }
        }
        return nullptr;
    }

    // offset para compensar a coordenada atual do runner e colocar o node no centro da interseccao
    Coordinate getOffsetNodePos() const
    {
        Coordinate updatedPos = runner->getPos();

        switch (runner->getDirection())
        {
        case RIGHT_DIR:
            updatedPos.x += (runner->getSafeDistance() + HALF_ROBOT_SIZE);
            break;
        case DOWN_DIR:
            updatedPos.y -= (runner->getSafeDistance() + HALF_ROBOT_SIZE);
            break;
        case LEFT_DIR:
            updatedPos.x -= (runner->getSafeDistance() + HALF_ROBOT_SIZE);
            break;
        default:
            updatedPos.y += (runner->getSafeDistance() + HALF_ROBOT_SIZE);
            break;
        }

        return updatedPos;
    }

    // atualiza o target node com a coordenada passada, seta ele como ja visitado,
    // preeche o array de direcoes possiveis, conecta ele com o current node
    // e entao o current node vira ele
    void updateCurrentNode(Coordinate pos)
    {
        bool avaliableDir[DIRECTION_QTY] = {false};

        targetNode->setCoordinate(pos);

        runner->getAvaliableDir(avaliableDir);

        for (int i = 0; i < DIRECTION_QTY; i++)
        {
            if (avaliableDir[i])
            {
                targetNode->setDirection(addNode(), (Direction)i, INT_MAX);
            }
        }
        targetNode->setVisited(true);

        currentNode->linkToNode(targetNode, runner->getDirection());
        currentNode = targetNode;
        printNode(currentNode);
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