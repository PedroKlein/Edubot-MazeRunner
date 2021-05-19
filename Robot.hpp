#ifndef __ROBOT__
#define __ROBOT__

#include "libs/EdubotLib.hpp"
#include "Coordinate.hpp"

// delay de 1.5 segundos para a rotacao.
#define DELAY_ROTATION 1500
// delay de loops para o programa nao rodar muito diferente em outros computadores.
#define DELAY_LOOP 1
// delay para executar uma leitura importante e garantir que esta pode ser diferente da anterior.
#define DELAY_SENSOR_MEASURE 100

#define ROBOT_SPEED .3
#define ROBOT_CAUTION_SPEED ROBOT_SPEED / 2.

#define ROBOT_SIZE 0.25
#define HALF_ROBOT_SIZE ROBOT_SIZE / 2.

// Menor distancia que o sensor pode detectar.
#define DELTA_SAFE_DISTANCE 0.01
#define MAX_SONAR_DISTANCE 2.0

// enum de indices dos bumpers.
enum Bumper
{
    FRONT_LEFT,
    FORNT_RIGHT,
    BACK_LEFT,
    BACK_RIGHT,
    BUMPER_QTY,
    NONE
};

// enum de indices dos sonares.
enum Sonar
{
    LEFT_SONAR,
    LEFT_60_SONAR,
    LEFT_30_SONAR,
    FRONT_SONAR,
    RIGHT_30_SONAR,
    RIGHT_60_SONAR,
    RIGHT_SONAR,
    SONAR_QTY
};

class Robot : public EdubotLib
{
private:
    float safeDistance = 0;
    float minAvaliableDistance = 0;

public:
    Robot(){};

    float getSafeDistance() const
    {
        return safeDistance;
    };

    float getMinAvaliableDistance() const
    {
        return minAvaliableDistance;
    };

    Coordinate getPos()
    {
        return Coordinate(this->getX(), this->getY());
    }

    // retorna direcao absoluta a qual o robo esta apontando
    Direction getDirection()
    {
        float theta = this->getTheta();
        theta = theta == 360 ? 0 : theta;
        return (Direction)(theta / 90);
    }

    // verifica se ha caminho viavel nas laterais
    bool hasPathSideways()
    {
        return (this->getSonar(LEFT_SONAR) >= minAvaliableDistance) || (this->getSonar(RIGHT_SONAR) >= minAvaliableDistance);
    }

    // verifica se o robo esta em um espaco vazio
    bool isInEmptySpace()
    {
        bool result = true;
        for (int i = 0; i < SONAR_QTY; i++)
        {
            result &= (this->getSonar(i) >= 2 * minAvaliableDistance);
        }
        return result;
    }

    // verifica se o robo esta em um beco sem saida
    bool isInDeadEnd()
    {
        bool result = false;
        result |= (this->getSonar(FRONT_SONAR) >= minAvaliableDistance);
        result |= (this->getSonar(LEFT_SONAR) >= minAvaliableDistance);
        result |= (this->getSonar(RIGHT_SONAR) >= minAvaliableDistance);
        return !result;
    }

    // a distancia segura eh calculada com base na media das distancias laterais iniciais do robo, ou seja,
    // ela fara com que o robo ande sempre com aproximadamente a mesma distancia entre as paredes.
    float calculateSafeDistance();

    // rotina simples de rotacao.
    void rotateRobot(float theta);

    // retorna o bumper que estiver ativo.
    Bumper getBumperActive();

    void getAvaliableDir(bool outDir[DIRECTION_QTY]);

    // retorna o theta "absoluto" (0 a 360 com base no plano do ambiente) desejado com uma rotacao.
    float getDesiredTheta(float thetaRotation, float thetaBeforeRotation);

    // rotina que garante que o robo fara uma curva dentro da distancia segura.
    void safetyBeforeRotation();

    // rotina de movimentacao segura por uma certa distanca.
    void safeMoveDistance(float distance);

    // rotina que rotaciona o robo para um theta "absoluto" (0 a 360 com base no plano do ambiente).
    void faceTheta(float theta);

    // versao segura da faceTheta (corrigi colisoes)
    void safeFaceTheta(float theta);

    // rotina que corrigi possiveis batidas, colocando o robo na direcao inicialmente desejada.
    void correctRotation(float desiredTheta);

    // rotina de rotacao segura, corrigindo erros e mantendo distancia segura.
    void safeRotate(float thetaRotation);

    // rotina que fragmaneta a rotacao de um determiando angulo
    void fragmentRotation(float thetaRotation, float times);
};

float Robot::calculateSafeDistance()
{
    this->sleepMilliseconds(DELAY_SENSOR_MEASURE);
    this->safeDistance = (this->getSonar(RIGHT_SONAR) + this->getSonar(LEFT_SONAR)) / 2.;
    this->minAvaliableDistance = this->safeDistance * 2 + HALF_ROBOT_SIZE;
    return this->safeDistance;
}

void Robot::rotateRobot(float theta)
{
    this->rotate(theta);
    this->sleepMilliseconds(DELAY_ROTATION);
}

Bumper Robot::getBumperActive()
{
    for (int i = 0; i < BUMPER_QTY; i++)
    {
        if (this->getBumper(i))
        {
            return (Bumper)i;
        }
    }
    return NONE;
}

void Robot::getAvaliableDir(bool outDir[DIRECTION_QTY])
{
    Direction currentDir = getDirection();
    Direction leftSonarDir, rightSonarDir;

    switch (currentDir)
    {
    case RIGHT_DIR:
        leftSonarDir = TOP_DIR;
        rightSonarDir = DOWN_DIR;
        break;
    case DOWN_DIR:
        leftSonarDir = RIGHT_DIR;
        rightSonarDir = LEFT_DIR;
        break;
    case LEFT_DIR:
        leftSonarDir = DOWN_DIR;
        rightSonarDir = TOP_DIR;
        break;
    default:
        leftSonarDir = LEFT_DIR;
        rightSonarDir = RIGHT_DIR;
        break;
    }

    outDir[(int)currentDir] = this->getSonar(FRONT_SONAR) >= minAvaliableDistance + ROBOT_SIZE;
    outDir[(int)rightSonarDir] = this->getSonar(RIGHT_SONAR) >= minAvaliableDistance;
    outDir[(int)leftSonarDir] = this->getSonar(LEFT_SONAR) >= minAvaliableDistance;
}

float Robot::getDesiredTheta(float thetaRotation, float thetaBeforeRotation)
{
    float deltaTheta = thetaBeforeRotation - thetaRotation;
    if (deltaTheta < 0)
    {
        return 360 + deltaTheta;
    }
    return deltaTheta > 360 ? deltaTheta - 360 : deltaTheta;
}

void Robot::faceTheta(float theta)
{
    float currentTheta = this->getTheta();
    float deltaTheta = currentTheta - theta;

    if (currentTheta > theta)
    {
        deltaTheta > 180 ? rotateRobot(-(360 - deltaTheta)) : rotateRobot(deltaTheta);
    }
    else
    {
        ABS(deltaTheta) > 180 ? rotateRobot(360 - ABS(deltaTheta)) : rotateRobot(deltaTheta);
    }
}

void Robot::safeFaceTheta(float theta)
{
    float currentTheta = this->getTheta();
    float deltaTheta = currentTheta - theta;

    if (currentTheta > theta)
    {
        deltaTheta > 180 ? safeRotate(-(360 - deltaTheta)) : safeRotate(deltaTheta);
    }
    else
    {
        ABS(deltaTheta) > 180 ? safeRotate(360 - ABS(deltaTheta)) : safeRotate(deltaTheta);
    }
}

void Robot::correctRotation(float desiredTheta)
{
    Bumper bumper = getBumperActive();
    (int)bumper < (int)BACK_LEFT ? this->move(-ROBOT_CAUTION_SPEED) : this->move(ROBOT_CAUTION_SPEED);
    this->sleepMilliseconds(50);
    faceTheta(desiredTheta);
}

void Robot::safeMoveDistance(float distance)
{
    Coordinate finalPos = this->getPos();
    Direction movementDir = this->getDirection();

    switch (movementDir)
    {
    case RIGHT_DIR:
        finalPos.x += distance;
        break;
    case DOWN_DIR:
        finalPos.y -= distance;
        break;
    case LEFT_DIR:
        finalPos.x -= distance;
        break;
    default:
        finalPos.y += distance;
        break;
    }

    this->move(ROBOT_CAUTION_SPEED);
    while (!this->getPos().hasReachedCoordinate(finalPos, movementDir))
    {
        this->sleepMilliseconds(DELAY_LOOP);
    }
    this->stop();
}

void Robot::safetyBeforeRotation()
{
    this->sleepMilliseconds(DELAY_SENSOR_MEASURE);
    if (this->getSonar(FRONT_SONAR) <= this->getSafeDistance() - DELTA_SAFE_DISTANCE)
    {
        this->move(-ROBOT_CAUTION_SPEED);
        while (this->getSonar(FRONT_SONAR) <= this->getSafeDistance() - DELTA_SAFE_DISTANCE)
        {
            //this->sleepMilliseconds(DELAY_LOOP);
        }
        this->stop();
    }
}

void Robot::fragmentRotation(float thetaRotation, float times)
{
    for (int i = 0; i < times; i++)
    {
        rotateRobot(thetaRotation / times);
        this->move(ROBOT_CAUTION_SPEED);
        this->sleepMilliseconds(50 / times);
        this->stop();
    }
}

void Robot::safeRotate(float thetaRotation)
{
    this->stop();
    this->sleepMilliseconds(DELAY_SENSOR_MEASURE);

    float desiredTheta = getDesiredTheta(thetaRotation, this->getTheta());
    safetyBeforeRotation();

    // if (thetaRotation > 0)
    // {
    //     if (this->getSonar(LEFT_SONAR) < safeDistance)
    //     {
    //         fragmentRotation(thetaRotation, 2);
    //     }
    //     else
    //     {
    //         rotateRobot(thetaRotation);
    //     }
    // }
    // else
    // {
    //     if (this->getSonar(RIGHT_SONAR) < safeDistance)
    //     {
    //         fragmentRotation(thetaRotation, 2);
    //     }
    //     else
    //     {
    //         rotateRobot(thetaRotation);
    //     }
    // }
    rotateRobot(thetaRotation);
    if (getBumperActive() != NONE)
    {
        // TODO: infinite loop when rotation = -0;
        while (this->getTheta() != ABS(desiredTheta))
        {
            if (this->getTheta() == 360 && ABS(desiredTheta) == 0)
            {
                break;
            }
            correctRotation(desiredTheta);
        }
    }

    this->move(ROBOT_SPEED);
}

#endif