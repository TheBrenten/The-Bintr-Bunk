#include "PhysFunc.h"
#include "GameObject.h"
#include "GameFunction.h"
#include "Particle.h"
#include "MenuFunctions.h"
#include <vector>
using namespace std;
#define say(x) std::cout<<x<<'\n'

extern bool moveLeft ;
extern bool moveRight;
extern bool moveUp   ;
extern bool moveDown ;
extern vec cam;
extern vec camLast;

extern bool zoomIn;
extern bool zoomOut;
extern float zoomScale;

extern float physRate;
extern GameFunction gfPhysics;
extern GameFunction gfAnim;
extern vector<BintrSound*> activeSounds;


float screenShake = 0;
float screenShakeAngle = 0;
vec screenShakePos;

void physFunc()
{
    // screen shake
    {
        screenShakeAngle = randgauss(0, 5) * screenShake;
        screenShakePos = randVec(0, 20) * screenShake;
        screenShake *= 0.85;
        if (screenShake < 0.01) screenShake = 0;
    }
    // erase debug dots & lines

    while (dots.size() > 0) {
        delete dots[0];
        dots.erase(dots.begin());
    }
    // Debug Lines
    while (lines.size() > 0) {
        delete lines[0];
        lines.erase(lines.begin());
    }

    // animate menu things
    for (int i = menuObjects.size() - 1; i >= 0; i--) {
        MenuObject* m = menuObjects[i];
        m->animate();
    }

    //new Particle(cam, randVec(1, 15), 30, 50, 0, hsvtorgb(randnum(0, 360), 1, 1), sf::Color(0, 0, 0, 0));

    // update timescale in physics thread to prevent weird interpolation hitches
    {
        float desiredPhysRate = 1 / physRate;
        gfPhysics.rate += (desiredPhysRate-gfPhysics.rate)*.25;
        gfAnim.rate = gfPhysics.rate * 2;
    }
    // Zoom Code
    {
        if (zoomIn) zoomScale *= 0.98;
        if (zoomOut) {
            float maxAdd = zoomScale*1.02 - zoomScale;
            float add = (5 - zoomScale)*.1;
            zoomScale += min(add, maxAdd);
        }
        if (zoomScale < 0.1) zoomScale = 0.1;
        if (zoomScale > 5) zoomScale = 5;
    }
    // object physics
    // reverse iterate in case they despawn 
    for (int i = activeObjects.size()-1; i >= 0; i--) {
        GameObject* obj = activeObjects[i];
        obj->posInterpLast = obj->pos;
        // only move objects if theyre in valid regions
        if (obj->canMove) {
            obj->move();
        }
        else {
            obj->updateRegions();
        }
    }

    // decrease region life, despawn them after offscreen for 5 seconds
    auto it = regions.begin();
    while (it != regions.end()) {
        it->second->life--;
        if (it->second->life <= 0) {
            delete it->second;
            it = regions.erase(it);
        }
        else {
            ++it;
        }
    }
    // Calculate actively playing sounds
    for (int i = activeSounds.size() - 1; i >= 0; i--) {
        if (activeSounds[i]->sound.getStatus() != Sound::Playing) {
            delete activeSounds[i];
            activeSounds.erase(activeSounds.begin() + i);
        }
        else {
            //activeSounds[i].pos->print();
            float pitch = activeSounds[i]->sound.getPitch() * activeSounds[i]->pitchVel;
            activeSounds[i]->sound.setPitch(pitch);
        }
    }

    

}
