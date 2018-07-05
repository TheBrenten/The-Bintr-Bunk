#pragma once

#include <vector>
#include "GameObject.h"
#include "Region.h"

void makeNoise(float radius, vec pos);

class Enemy : public GameObject
{
public:
    // sound
    Sound sound;
    float soundVolume;
    float soundPitch;
    // pathfinding / AI
    vec playerSpottedAt;
    bool wandering = true;

    int canPlaySound = 0;
    float spinOutSine = 0;
    int justHurt = 0;
    float spinOutAngle;
    vec posLast;
    Texture texture;
    Sprite sprite;
    CircleShape disp_circ;
    Enemy(vec pos = vec(0, 0)); 
    ~Enemy() {
        //say("Enemy Deleted!");
    }
    void move();
    void renderFront();
    void renderDebug();
    void hurt(float damage);
};

