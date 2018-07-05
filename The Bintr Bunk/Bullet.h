#pragma once
#include "GameObject.h"
#include "LoadResources.h"
#include "BintrsTools.h"

class Bullet : public GameObject
{
public:
    bool despawn = false; // leave tracer around before despawning bullet
    vec vel;
    int samples;
    vector<vec> tracerPositions;
    Bullet(vec pos, float angle, float speed);
    vec posInterp;
    void setInterpolation();
    void move();
    void render();

};

