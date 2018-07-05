#pragma once
#include "GameObject.h"

#include "LoadResources.h"
#include "BintrImage.h"
#include "PhysicsStuff.h"
#include "ImageObj.h"
extern int showHealthBar;
class Player;
extern vector<Player*> players;


/*
multiplayer process:
-player creates 3 imageobjs (body + 2 arms)
-packet updates pos/angle/dir/hand pos
-player.setImages() runs and sets imageobjs layer/pos/angle/dir accordingly
*/

extern Weapon* mainItem;
extern Weapon* lItem;
extern Weapon* rItem;
extern ImageObj* mainItemObj;

class Player
{
public:
    

    // possibly remove
    //vec   lItemPos;
    //float lItemAng;
    //vec   rItemPos;
    //float rItemAng;
    //vec   mainItemPos;
    //float mainItemAng;
    //vec lHandPos;
    //vec rHandPos;
    //vec lShoulderPos, rShoulderPos;

    // definitely needed
    float health, maxHealth;
    vec pos;
    vec posLast;
    // angle is smoothed
    float angle;
    float idealAngle = 0;
    vector<ImageData> crouchWalk;
    vector<ImageData> jumpToFall;
    vector<ImageData> run;
    vector<ImageData> standToCrouch;
    ImageObj body;
    ImageObj lArm;
    ImageObj rArm;
    bool leftHanded = false;
    // animation
    bool dir = true;
    bool dirLast = true;
    int animState = animStanding;
    int animStateLast = animStanding;
    int runFrame = 0;
    int fallFrame = 0;
    int crouchWalkFrame = 0;
    int standToCrouchFrame = 0;
    map<int, ArmData> arms;
    ArmData* rArmData;
    ArmData* lArmData;
    int invincibilitySteps = 0;
    int regenerate = 0;
    sf::Shader* shader = nullptr;
    ImageData stand;
    ImageData crouch;
    ImageData jump;
    ImageData fall;
    ImageData* imageData; // active image

    Player();
    ~Player();
    void move();
    void setImages();
    void setLeftItem(Weapon* item);
    void setRightItem(Weapon* item);
    void hurt(float damage);
};

