#include <iostream>
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
	const float SAFE_DISTANCE = robot->calculateSafeDistance();
	cout << "DISTANCIA SEGURA: " << SAFE_DISTANCE << endl;
	MazeRunner *maze = new MazeRunner(robot);
	maze->updateTargetNodeOnDirection();
	//maze->printNode(maze->getCurrentNode());

	robot->move(ROBOT_SPEED);

	while (robot->isConnected())
	{
		static bool isCloseToWall = false, hasPathSideways = false, isBackTracking = false;

		if (!hasPathSideways && robot->hasPathSideways())
		{

			if (robot->isInEmptySpace())
			{
				cout << "LABIRINTO CONCLUIDO!" << endl;
				break;
			}

			hasPathSideways = true;

			Coordinate offsetNodePos = maze->getOffsetNodePos();

			if (auto node = maze->getNodeWithin(offsetNodePos))
			{
				Direction newPathDir = node->getNewPathDir();
				if (newPathDir < DIRECTION_QTY)
				{
					robot->safeMoveDistance(HALF_ROBOT_SIZE + SAFE_DISTANCE / 2.);
					robot->safeFaceTheta(newPathDir * 90);
					robot->move(ROBOT_SPEED);
					isBackTracking = false;
				}
				else if (!isBackTracking)
				{
					robot->safeRotate(180);
					isBackTracking = true;
				}
			}
			else
			{
				maze->updateCurrentNode(offsetNodePos);
			}
			maze->updateTargetNodeOnDirection();
			maze->printNode(maze->getCurrentNode());
		}
		// quando o robo esta a uma distancia igual ou menor da segura de uma parede ele realiza uma rotacao segura para o lado com mais espaco.
		if (robot->getSonar(FRONT_SONAR) <= SAFE_DISTANCE)
		{
			isCloseToWall = false;
			robot->getSonar(LEFT_SONAR) <= robot->getSonar(RIGHT_SONAR) ? robot->safeRotate(90) : robot->safeRotate(-90);
			maze->updateTargetNodeOnDirection();
		}
		// quando o robo esta perto de uma parede ele desacelera.
		else if (!isCloseToWall && robot->getSonar(FRONT_SONAR) <= SAFE_DISTANCE * 2.)
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