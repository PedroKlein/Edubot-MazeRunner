#include <iostream>
#include <list>
#include "MazeRunner.hpp"
#include "Robot.hpp"

using namespace std;

int main()
{
	Robot *robot = new Robot();

	if (!robot->connect())
	{
		cout << "Could not connect on robot!" << endl;
		return 0;
	}

	cout << "DISTANCIA SEGURA: " << robot->calculateSafeDistance() << endl;
	MazeRunner *maze = new MazeRunner(robot);
	maze->updateTargetNodeOnDirection();
	maze->printCurrentNode();

	robot->move(ROBOT_SPEED);

	while (robot->isConnected())
	{
		static bool isCloseToWall = false, hasPathSideways = false;

		if (!hasPathSideways && robot->hasPathSideways())
		{
			hasPathSideways = true;
			maze->updateCurrentNode();
			maze->updateTargetNodeOnDirection();
			maze->printCurrentNode();
		}
		// quando o robo esta a uma distancia igual ou menor da segura de uma parede ele realiza uma rotacao segura para o lado com mais espaco.
		if (robot->getSonar(FRONT_SONAR) <= robot->getSafeDistance())
		{
			isCloseToWall = false;
			robot->getSonar(LEFT_SONAR) <= robot->getSonar(RIGHT_SONAR) ? robot->safeRotate(90) : robot->safeRotate(-90);
			maze->updateTargetNodeOnDirection();
		}
		// quando o robo esta perto de uma parede ele desacelera.
		else if (!isCloseToWall && robot->getSonar(FRONT_SONAR) <= robot->getSafeDistance() * 2.)
		{
			isCloseToWall = true;
			robot->move(ROBOT_CAUTION_SPEED);
		}

		if (!robot->hasPathSideways())
		{
			hasPathSideways = false;
		}
		robot->sleepMilliseconds(DELAY_LOOP);
	}

	robot->stop();

	robot->disconnect();

	return 0;
}