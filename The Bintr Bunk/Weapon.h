#pragma once
#include <SFML/Graphics.hpp>
#include "BintrsTools.h"
#include "Bullet.h"
#include "Grenade.h"
#include "ImageObj.h"
using namespace sf;
using namespace std;

extern SoundBuffer* grenadePin;
extern SoundBuffer* grenadeTick;
extern SoundBuffer* triggerPull;
extern SoundBuffer* triggerRelease;
class KeyFrame;
extern vector<KeyFrame*> activeKeyFrames;

enum projectileTypes {
    none,
    bullet,
    grenade
};

class GameObj;
class Displacement {
public:
    ImageObj** objP = nullptr;
    vec pos;
    float angle;
    ImageObj** relativeObjP = nullptr;
    Displacement(ImageObj** objP, vec pos, float angle, ImageObj** relativeObjP = nullptr) {
        this->objP = objP;
        this->pos = pos;
        this->angle = angle;
        this->relativeObjP = relativeObjP;
    }
};

class KeyFrame {
public:
    int duration;
    int elapsed = 0;
    // key: vector pointer
    // value: ( position, angle )
    void(*funcP)() = nullptr;
    vector<Displacement> displacements;
    KeyFrame* nextFrame = nullptr;
    KeyFrame* lastFrame = nullptr;
    KeyFrame() {}
};

void linkFrames(KeyFrame* kf1, KeyFrame* kf2);

// store position/angle in weapons
// then use that to spawn projectiles

class Weapon {
public:
    KeyFrame* activeFrame = nullptr;
    KeyFrame* shootFrame = nullptr;
    KeyFrame* reloadFrame = nullptr;
    vec worldNozzle;
    float angle;
    int projectile;
    float projectileSpeed = 150;
    float inaccuracy = 1;

    int shootTracker, shootSteps;
    int reloadTracker, reloadSteps;
    int magBullets = 0;
    int magSize = 0;
    SoundBuffer* shootSound = nullptr;
    SoundBuffer* reloadSound = nullptr;

    vec grip1;
    vec grip2;
    vec nozzle;

    vec origin; // local point image will rotate around
    vec offset; // offset relative to player shoulder
    bool isTwoHanded = false;
    //Texture texture;
	vector<Texture> textures;
    vector<Sprite> sprites;
    vector<int> invisibleSprites;

    int projectilesPerShot = 1;
    bool hasTrigger = true;
    bool hasPin = false;
    bool shootOnPress, shootOnRelease;
    bool automatic = true;


	/*
	Special Weapon Image Pixels:
	-Green: Main hand Point
	-Blue: Secondary hand point (if appliciable)
	-Red: Nozzle point (if appliciable)
	Extra image notes:
	-Must have green pixel to orient around
	-On weapon creation, input a vector of filepath strings at least 1 in size
		-index 0 must 
		-be full body, used for inventory images
		-contain all points (others after just need 1 green)
	*/
    Weapon(vector<string> filepaths, vec origin = vec(), vec offset = vec());

    // shoot may be called from press or release events
    void shoot();
    void reload();

    void pressEvent();
    void releaseEvent();

    void render();

};