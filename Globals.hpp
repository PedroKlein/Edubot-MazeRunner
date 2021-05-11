#ifndef __GLOBALS__
#define __GLOBALS__

struct Coordinate
{
    float x = INT_MAX;
    float y = INT_MAX;
    Coordinate(float x, float y) : x(x), y(y) {}
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