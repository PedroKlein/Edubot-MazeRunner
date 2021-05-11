#include <iostream>
#include <list>
#include "Robot.hpp"
#include "Maze.hpp"

using namespace std;

void printNodes(list<Node> nodes)
{
	for (auto it : nodes)
	{
		Coordinate pos = it.getCoordinate();
		cout << "X: " << pos.x << " Y: " << pos.y << " isVisited: " << it.isVisited() << endl;
	}
}

int main()
{
	Robot *robot = new Robot();
	Maze *maze = new Maze(Coordinate(0., 0.));
	printNodes(maze->getNodes());
	if (!robot->connect())
	{
		cout << "Could not connect on robot!" << endl;
		return 0;
	}

	cout << "DISTANCIA SEGURA: " << robot->calculateSafeDistance() << endl;

	robot->move(ROBOT_SPEED);

	while (robot->isConnected())
	{
		static bool isCloseToWall = false;
		// quando o robo esta a uma distancia igual ou menor da segura de uma parede ele realiza uma rotacao segura para o lado com mais espaco.
		if (robot->getSonar(FRONT_SONAR) <= robot->getSafeDistance())
		{
			isCloseToWall = false;
			robot->getSonar(LEFT_SONAR) <= robot->getSonar(RIGHT_SONAR) ? robot->safeRotate(90) : robot->safeRotate(-90);
		}
		// quando o robo esta perto de uma parede ele desacelera.
		else if (!isCloseToWall && robot->getSonar(FRONT_SONAR) <= robot->getSafeDistance() * 4.)
		{
			isCloseToWall = true;
			robot->move(ROBOT_SPEED / 4.);
		}
		robot->sleepMilliseconds(DELAY_LOOP);
	}

	robot->stop();

	robot->disconnect();

	return 0;
}