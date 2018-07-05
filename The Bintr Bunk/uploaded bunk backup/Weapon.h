#pragma once
#include <SFML/Graphics.hpp>
#include "BintrsTools.h"
#include "Bullet.h"
#include "Grenade.h"
using namespace sf;
using namespace std;

extern SoundBuffer* grenadePin;
extern SoundBuffer* grenadeTick;
extern SoundBuffer* triggerPull;
extern SoundBuffer* triggerRelease;

enum projectileTypes {
    none,
    bullet,
    grenade
};

// store position/angle in weapons
// then use that to spawn projectiles

class Weapon {
public:
    vec pos;
    float angle;
    int projectile;
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
    Texture texture;

    int projectilesPerShot = 1;
    bool hasTrigger = true;
    bool hasPin = false;
    bool shootOnPress, shootOnRelease;
    bool automatic = true;

    Weapon(string filepath, vec origin = vec(), vec offset = vec()) {
        this->origin = origin;
        this->offset = offset;
        Image image;
        image.loadFromFile(filepath);
        for (int x = 0; x < image.getSize().x; x++) {
            for (int y = 0; y < image.getSize().y; y++) {
                if (image.getPixel(x, y) == Color(255, 0, 0)) {
                    replacePixel(&image, x, y);
                    nozzle = vec(x, y);
                }
                if (image.getPixel(x, y) == Color(0, 255, 0)) {
                    replacePixel(&image, x, y);
                    grip1 = vec(x, y);
                }
                // armBack top - 0,0,255
                // 2 handed
                if (image.getPixel(x, y) == Color(0, 0, 255)) {
                    replacePixel(&image, x, y);
                    grip2 = vec(x, y);
                    isTwoHanded = true;
                }
            }
        }
        texture.loadFromImage(image); // after image is processed

    }

    //void shoot();
    //void reload();
    // shoot may be called from press or release events
    void shoot();
    void reload() {
        if (reloadTracker == 0) {
            reloadTracker = reloadSteps;
            playSound(reloadSound, pos, 20);
        }
    }

    void pressEvent() {

        if (hasTrigger)
            playSound(triggerPull, pos, 20);
        else if (hasPin)
            playSound(grenadePin, pos, 20);

        if (shootOnPress) {
            shoot();
        }

    }
    void releaseEvent() {
        say("release event");
        if (hasTrigger)
            playSound(triggerRelease, pos, 20);
        if (shootOnRelease) {
            shoot();
        }
    }

};