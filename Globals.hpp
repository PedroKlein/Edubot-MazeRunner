#ifndef __GLOBALS__
#define __GLOBALS__

#include <math.h>

#define SQUARE(x) (x * x)
// macro para valor absoluto.
#define ABS(x) ((x) >= 0 ? (x) : -(x))

struct Coordinate
{
    float x = INT_MAX;
    float y = INT_MAX;

    Coordinate(float x, float y) : x(x), y(y) {}

    bool isWithinCircle(const float radius, const Coordinate center) const
    {
        return SQUARE((x - center.x)) + SQUARE((y - center.y)) <= SQUARE(radius);
    }

    float distanceBetween(const Coordinate dest) const
    {
        return sqrt(SQUARE((x - dest.x)) + SQUARE((y - dest.y)));
    }
};

enum Direction
{
    RIGHT_DIR,
    TOP_DIR,
    LEFT_DIR,
    DOWN_DIR,
    DIRECTION_QTY
};

#endif