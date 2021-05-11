#ifndef __ROBOT__
#define __ROBOT__

#include "libs/EdubotLib.hpp"
#include "Globals.hpp"

// macro para valor absoluto.
#define ABS(x) ((x) >= 0 ? (x) : -(x))

// delay de 1.5 segundos para a rotacao.
#define DELAY_ROTATION 1500
// delay de loops para o programa nao rodar muito diferente em outros computadores.
#define DELAY_LOOP 1
// delay para executar uma leitura importante e garantir que esta pode ser diferente da anterior.
#define DELAY_SENSOR_MEASURE 50

#define ROBOT_SPEED 1.
// Menor distancia que o sensor pode detectar.
#define DELTA_SAFE_DISTANCE 0.01

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

public:
    Robot();

    float getSafeDistance()
    {
        return safeDistance;
    };

    // a distancia segura eh calculada com base na media das distancias laterais iniciais do robo, ou seja,
    // ela fara com que o robo ande sempre com aproximadamente a mesma distancia entre as paredes.
    float calculateSafeDistance();

    // rotina simples de rotacao.
    void rotateRobot(float theta);

    // retorna o bumper que estiver ativo.
    Bumper getBumperActive();

    // retorna o theta "absoluto" (0 a 360 com base no plano do ambiente) desejado com uma rotacao.
    float getDesiredTheta(float thetaRotation, float thetaBeforeRotation);

    // rotina que garante que o robo fara uma curva dentro da distancia segura.
    void safetyBeforeRotation();

    // rotina que rotaciona o robo para um theta "absoluto" (0 a 360 com base no plano do ambiente).
    void faceTheta(float theta);

    // rotina que corrigi possiveis batidas, colocando o robo na direcao inicialmente desejada.
    void correctRotation(float desiredTheta);

    // rotina de rotacao segura, corrigindo erros e mantendo distancia segura.
    void safeRotate(float thetaRotation);
};

float Robot::calculateSafeDistance()
{
    this->sleepMilliseconds(DELAY_SENSOR_MEASURE);
    this->safeDistance = (this->getSonar(RIGHT_SONAR) + this->getSonar(LEFT_SONAR)) / 2.;
    return this->safeDistance;
}

// rotina simples de rotacao.
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

float Robot::getDesiredTheta(float thetaRotation, float thetaBeforeRotation)
{
    float deltaTheta = thetaBeforeRotation - thetaRotation;
    if (deltaTheta < 0)
    {
        return 360 + deltaTheta;
    }
    return deltaTheta > 360 ? deltaTheta - 360 : deltaTheta;
}

void Robot::safetyBeforeRotation()
{
    this->sleepMilliseconds(DELAY_SENSOR_MEASURE);
    if (this->getSonar(FRONT_SONAR) <= this->getSafeDistance() - DELTA_SAFE_DISTANCE)
    {
        this->move(-ROBOT_SPEED / 4.);
        while (this->getSonar(FRONT_SONAR) <= this->getSafeDistance())
        {
            this->sleepMilliseconds(DELAY_LOOP);
        }
        this->stop();
    }
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

void Robot::correctRotation(float desiredTheta)
{
    Bumper bumper = getBumperActive();

    (int)bumper < (int)BACK_LEFT ? this->move(-ROBOT_SPEED / 6.) : this->move(ROBOT_SPEED / 6.);
    this->sleepMilliseconds(50);
    faceTheta(desiredTheta);
}

void Robot::safeRotate(float thetaRotation)
{
    this->stop();
    this->sleepMilliseconds(DELAY_SENSOR_MEASURE);

    float desiredTheta = getDesiredTheta(thetaRotation, this->getTheta());

    safetyBeforeRotation();
    rotateRobot(thetaRotation);

    if (getBumperActive() != NONE)
    {
        while (this->getTheta() != ABS(desiredTheta))
        {
            correctRotation(desiredTheta);
        }
    }

    this->move(ROBOT_SPEED);
}

#endif