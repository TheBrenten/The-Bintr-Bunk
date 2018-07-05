#pragma once
#include "GameObject.h"

#include "LoadResources.h"
#include "BintrImage.h"
#include "PhysicsStuff.h"
extern int showHealthBar;

extern PhysPoint* ppLShoulder;
extern PhysPoint* ppLHand;
extern Spring*    lArmSpring;
extern Stick*     lArmStick;
extern PhysPoint* ppRShoulder;
extern PhysPoint* ppRHand;
extern Spring*    rArmSpring;
extern Stick*     rArmStick;


class Player : public GameObject
{
public:
    bool leftHanded = false;
    sf::Sprite sprite;
    sf::Sprite lArmSprite;
    sf::Sprite rArmSprite;
    sf::Sprite mainItemSprite;
    sf::Sprite lItemSprite;
    sf::Sprite rItemSprite;

    // animation
    bool dir = true;
    int animState = animStanding;
    int animStateLast = animStanding;
    int runFrame = 0;
    int fallFrame = 0;
    int crouchWalkFrame = 0;
    int standToCrouchFrame = 0;

    vec lHandPos, rHandPos;
    vec lShoulderPos, rShoulderPos;

    // angle is smoothed
    float angle = 0;
    float idealAngle = 0;
    Weapon* mainItem;
    Weapon* lItem;
    Weapon* rItem;
    vector<ImageData*> crouchWalk;
    vector<ImageData*> jumpToFall;
    vector<ImageData*> run;
    vector<ImageData*> standToCrouch;
    ImageData* stand;
    ImageData* crouch;
    ImageData* jump;
    ImageData* fall;
    ImageData* imageData; // active image
    map<int, ArmData*> arms;
    ArmData* rArmData;
    ArmData* lArmData;
    
    Player();
    ~Player();
    void move();
    void renderBack();
    void renderFront();
    //void renderFront();
    //
    void setLeftItem(Weapon* item);
    void setRightItem(Weapon* item);
    int invincibilitySteps = 0;
    int regenerate = 0;
    sf::Shader* shader = nullptr;
    void hurt(float damage);
};

