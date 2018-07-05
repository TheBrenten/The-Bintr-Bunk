#include "Player.h"
#include "Particle.h"


extern float physRate;
int showHealthBar = 0;
float headJumpPitch = 1;

extern bool moveLeft;
extern bool moveRight;
extern bool moveUp;
extern bool moveDown;
extern bool noclip;
extern bool jumping;
extern bool justJumped;
extern bool sprinting;
bool canDoubleJump = true;
float extraJumpAngle = 0;

extern vec cam;
extern vec camLast;

float gravity = 1.4;
float friction_air = 0.95;
float friction_ground = 0.82;
float friction_noclip = 0.92;
float friction_climb = 0.88;
float speed_ground = 2;
float speed_crouch = 0.6;
float speed_air = 0.5;
float speed_noclip = 2;
float speed_climb = 1.75;
float sprint_mult = 1.5;
float friction;

bool crouched = false;
bool grounded = false;
bool sliding = false;
float jumpForceMax = 7;
float jumpForce = jumpForceMax;
float jumpForceDecay = .6;

int ghostJumpTracker = 0;
int ghostJumpSteps = 10;
int slidingTracker = 0;
int slidingSteps = 10;

extern bool leftGrab;
extern bool rightGrab;
extern bool leftAction;
extern bool rightAction;
extern bool leftActionReleased;
extern bool rightActionReleased;
extern bool leftActionPressed;
extern bool rightActionPressed;
extern bool reloading;
extern vec globalMouse;
int grabSteps = 12;
int leftGrabTracker;
int rightGrabTracker;
float handGravity = 0.5;
void springForce(vec& p1, vec& p2);
void stickForce(vec& p1, vec& p2);




bool goombaStomped = false;


#define randomChoice(vector) vector[randint(0,vector.size()-1)]
#define forV(vector) for (int i=0;i<vector.size();i++)

vector<Player*> players;

Player::Player()
{
    players.emplace_back(this);
    //body = ImageObj();
    //lArm = ImageObj();
    //rArm = ImageObj();
    health = 100;
    maxHealth = 100;
    // make our colors
    sf::Color skinColor = hsvtorgb(randnum(18, 23),
                                   randnum(.2, .5),
                                   randnum(.8, 1));
    sf::Color shirtColor = hsvtorgb(randint(0, 360), randnum(0, 1), randtriangular(0, 1, 1));
    sf::Color pantsColor = hsvtorgb(randint(0, 360), randnum(0, 1), randtriangular(0, 1, 1));
    // load players textures up
    {
        forV(playerCrouchWalk) {
            crouchWalk.emplace_back(ImageData(playerCrouchWalk[i], skinColor, shirtColor, pantsColor));
        }
        forV(playerJumpToFall) {
            jumpToFall.emplace_back(ImageData(playerJumpToFall[i], skinColor, shirtColor, pantsColor));
        }
        forV(playerRun) {
            run.emplace_back(ImageData(playerRun[i], skinColor, shirtColor, pantsColor));
        }
        forV(playerStandToCrouch) {
            standToCrouch.emplace_back(ImageData(playerStandToCrouch[i], skinColor, shirtColor, pantsColor));
        }
        stand  = ImageData(playerStand, skinColor, shirtColor, pantsColor);
        crouch = ImageData(playerCrouch, skinColor, shirtColor, pantsColor);
        jump  = ImageData(playerJump, skinColor, shirtColor, pantsColor);
        fall  = ImageData(playerFall, skinColor, shirtColor, pantsColor);
        for (auto armIt = playerArm.begin(); armIt != playerArm.end(); armIt++) {
            arms[armIt->first] = ArmData(armIt->second, skinColor, shirtColor, pantsColor);
        }
        this->imageData = &fall;
    }

    //texture.loadFromImage(imgPlayerStand);
    //sprite.setTexture(texture);
    //sprite.setOrigin(64, 64);
    //updateRegions();
}


Player::~Player()
{
    for (int i = 0; i < players.size(); i++) {
        if (players[i] == this) players.erase(players.begin() + i);
    }
    //say("Deleted Player");
}

// only needed clientside
Weapon* mainItem;
Weapon* lItem;
Weapon* rItem;
ImageObj* mainItemObj;

// only the controlled player will call this
void Player::move()
{
    // manage trackers
    if (ghostJumpTracker > 0) ghostJumpTracker--;
    if (slidingTracker   > 0) slidingTracker--;
    if (leftGrabTracker > 0) leftGrabTracker--;
    if (rightGrabTracker > 0) rightGrabTracker--;
    if (leftAction && !checkTerrain(lArm.pos)) leftGrabTracker = 0;
    if (rightAction && !checkTerrain(rArm.pos)) rightGrabTracker = 0;
    //if (lastJumpTracker > 0) lastJumpTracker--;
    if (slidingTracker == 0) sliding = false;

    if (mainItem) {
        if (mainItem->shootTracker > 0) mainItem->shootTracker--;
        if (mainItem->reloadTracker > 0) {
            mainItem->reloadTracker--;
            if (mainItem->reloadTracker == 0) mainItem->magBullets = mainItem->magSize;
        }
    }
    if (lItem) {
        if (lItem->shootTracker > 0) lItem->shootTracker--;
        if (lItem->reloadTracker > 0) {
            lItem->reloadTracker--;
            if (lItem->reloadTracker == 0) lItem->magBullets = lItem->magSize;
        }
    }
    if (rItem) {
        if (rItem->shootTracker > 0) rItem->shootTracker--;
        if (rItem->reloadTracker > 0) {
            rItem->reloadTracker--;
            if (rItem->reloadTracker == 0) rItem->magBullets = rItem->magSize;
        }
    }
    // prevent float grabbing
    if (leftGrab && !checkTerrain(lArm.pos)) leftGrab = false;
    if (rightGrab && !checkTerrain(rArm.pos)) rightGrab = false;


    // play sliding sound
    float slideVolume;
    slideLoop->setPosition(pos.x, pos.y, -500);
    slideLoop->setPitch(physRate / 60);
    if (sliding) {
        slideVolume = 10;
    }
    else {
        slideVolume = 0;
    }
    float currentVolume = slideLoop->getVolume();
    if (currentVolume < slideVolume) currentVolume += 1;
    if (currentVolume > slideVolume) currentVolume -= 2;
    if (abs(slideVolume - currentVolume) < 2) currentVolume = slideVolume;
    slideLoop->setVolume(currentVolume);


    // infer velocity from previous position
    vec vel = pos - posLast;
    posLast = pos;


    vec addedFriction = vel - (vel * friction);
    vel -= addedFriction;
    //vel *= friction;

    vec addForce;
    if (moveLeft) addForce.x--;
    if (moveRight) addForce.x++;
    // noclip allows vertical movement
    if (noclip || (leftGrab || rightGrab)) {
        headJumpPitch = 1;
        if (moveUp) addForce.y--;
        if (moveDown) addForce.y++;
    }
    if (noclip) grounded = false;
    if (!grounded && !sliding) idealAngle = 0;

    // determine speed/friction
    float speed;
    if (grounded) {
        if (crouched) speed = speed_crouch;
        else          speed = speed_ground;
        friction = friction_ground;
    }
    else {
        speed = speed_air;
        friction = friction_air;
    }
    if (noclip) {
        speed = speed_noclip;
        friction = friction_noclip;
    }
    if (leftGrab || rightGrab) {
        ghostJumpTracker = ghostJumpSteps;
        speed = speed_climb;
        friction = friction_climb;
    }
    // add sprint force to speed
    if (sprinting) speed *= sprint_mult;

    if (grounded) addForce.rotate(angle);

    addForce.norm(speed);

    

    // jump code hopefully all here
    jumpForce -= jumpForceDecay;
    if (jumpForce < 0) jumpForce = 0;
    if (jumping || goombaStomped) {
        if (justJumped) {
            goombaStomped = false;
            // either single or double jump...
            if (ghostJumpTracker) {
                headJumpPitch = 1; // wall jumps will reset the increasing boing sound
                                   // for climbing wall jumps
                if (leftGrab) leftGrabTracker = grabSteps;
                if (rightGrab) rightGrabTracker = grabSteps;
                leftGrab = false;
                rightGrab = false;

                fallFrame = 0;
                playSound(randomChoice(jumpSounds), pos, 20);
                jumpForce = jumpForceMax;
                grounded = false;
                ghostJumpTracker = 0;
                if (sliding) {
                    vel.y = 0;
                    slidingTracker = 0;
                    extraJumpAngle = getTerrainNormal(pos, 32) - 270;
                    while (extraJumpAngle < -180) extraJumpAngle += 360;

                    //say("EJA: " << extraJumpAngle);
                    //extraJumpAngle *= .6; // maybe to bring it closer to 0
                }
                else extraJumpAngle = 0;
            }
            else if (canDoubleJump) {
                for (int i = 0; i < 4; i++) {
                    Color L = hsvtorgb(randnum(0, 360), 1, 1);
                    Color L2 = Color(L.r, L.g, L.b, 0);
                    Color R = hsvtorgb(randnum(0, 360), 1, 1);
                    Color R2 = Color(R.r, R.g, R.b, 0);

                    vec pVel = vel*randnum(0, -.4);
                    pVel.y = randnum(-1, 2);
                    vec Lv, Rv;
                    if (dir) {
                        Lv = pos + vec(20, 32).rotated(angle);
                        Rv = pos + vec(-18, 52).rotated(angle);
                    }
                    else {
                        Lv = pos + vec(20, 32).flipx().rotated(angle);
                        Rv = pos + vec(-18, 52).flipx().rotated(angle);
                    }
                    new Particle(Lv, pVel + randVec(0, 2), randint(20, 50), randnum(2, 5), randnum(5, 8), L, L2);
                    new Particle(Rv, pVel + randVec(0, 2), randint(20, 50), randnum(2, 5), randnum(5, 8), R, R2);
                }
                fallFrame = 0;
                if (vel.y > 0) vel.y = 0;
                playSound(randomChoice(doubleJumpSounds), pos, 20);
                canDoubleJump = false;

                jumpForce = jumpForceMax;
                grounded = false;
                ghostJumpTracker = 0;
                extraJumpAngle = 0;
            }
        }
        // we want wall jumps to send us away from the wall...
        pos += vec(0, -jumpForce).rotated(angle + extraJumpAngle);
        extraJumpAngle *= .6;
    }

    // terrain collision
    vector<vec> simpleCorrections;
    if (moveDown) crouched = true;
    if (crouched)
        simpleCorrections = {
        vec(0,  5).rotated(angle),  // toHead 
        vec(10, 34).rotated(angle),  // toRight 
        vec(-10, 34).rotated(angle),  // toLeft 
        vec(5,  20).rotated(angle),
        vec(-5,  20).rotated(angle),
        vec(5,  49).rotated(angle),
        vec(-5,  49).rotated(angle)
    };
    else
        simpleCorrections = {
        vec(0, -50).rotated(angle),  // toHead 
        vec(10,   7).rotated(angle),  // toRight 
        vec(-10,   7).rotated(angle),  // toLeft 
        vec(5, -21).rotated(angle),
        vec(-5, -21).rotated(angle),
        vec(5,  35).rotated(angle),
        vec(-5,  35).rotated(angle)
    };
    //new Dot(p->worldNozzle+toHead);
    //new Dot(p->worldNozzle + toLeft);
    //new Dot(p->worldNozzle + toRight);

    vec toFeet = vec(0, 64).rotated(angle);

    // goomba stopm
    if (!noclip && !grounded && vel.y > 4) {
        for (int i = 0; i < activeObjects.size(); i++) {
            GameObject* obj = activeObjects[i];
            if (obj->isEnemy && obj->health > 0 && (obj->pos - (pos + toFeet)).getLength() < 70 && pos.y > obj->pos.y - 80) {
                obj->hurt(obj->health + 1);
                obj->posLast.y = obj->pos.y;
                obj->pos.y += randnum(2, 5); // send down a bit
                                             //posLast.y = worldNozzle.y;
                vel.y = -10; // send player up a bit
                jumpForce = jumpForceMax;
                canDoubleJump = true;
                goombaStomped = true;
                fallFrame = 0; // makes player snap to jump frame
                playSound(headJump, pos + toFeet, 20, headJumpPitch);
                headJumpPitch *= 1.0594627424; // chromatic scale
                break;
            }
        }
    }

    pos += addForce;
    pos += vel;



    // gravity
    if (!noclip) {
        pos.y += gravity;

        int fixes = 0;
        // head/sides collision
        for (int i = 0; i < simpleCorrections.size(); i++) {
            vec bodyPoint = simpleCorrections[i];
            if (checkTerrain(pos + bodyPoint)) {
                // push outside terrain
                float slopeAngle = getTerrainNormal(pos + bodyPoint, 32);
                vec toOutside = vec(1, 0).rotated(slopeAngle);
                while (checkTerrain(pos + bodyPoint)) {
                    pos += toOutside;
                    //fixes++;
                }
                // define angle steepness for "sliding"
                // added 20 degrees for better wall jumping
                if (slopeAngle > 150 && slopeAngle < 270) {
                    idealAngle = slopeAngle + 180;
                    if (!grounded) {
                        sliding = true;
                        slidingTracker = slidingSteps;
                        canDoubleJump = true;
                        ghostJumpTracker = ghostJumpSteps;
                    }
                }
                if (slopeAngle < 30 || slopeAngle > 270) {
                    idealAngle = slopeAngle;
                    if (!grounded) {
                        sliding = true;
                        slidingTracker = slidingSteps;
                        canDoubleJump = true;
                        ghostJumpTracker = ghostJumpSteps;
                    }
                }
                // experiment
                // stick player to wall
                if (sliding) {
                    pos -= toOutside * 2;
                    //posLast -= toOutside * 4;
                }
            }
        }
        // feet collision
        vec checkPos = pos + toFeet;
        // check this many pixels under feet for ground
        int groundChecks = 8;
        if (justJumped) groundChecks = 1;
        //if (leftGrab || rightGrab) groundChecks = 2;
        //if ((!ghostJumpTracker || grounded || leftGrab || rightGrab)) sliding = false;
        while (groundChecks > 0) {
            groundChecks--;
            //new Dot(checkPos);
            // if we find ground under feet...
            if (checkTerrain(checkPos)) {
                vec tempDiff = pos - posLast;
                pos = checkPos - toFeet;
                posLast = pos - tempDiff;
                // determine slope idealAngle
                float groundAngle = getTerrainNormal(checkPos, 32) + 90;
                while (groundAngle > 360) groundAngle -= 360;
                // determine if flat
                bool flatGround = false;
                if (groundAngle <= 45 || groundAngle >= 360 - 45)
                    flatGround = true;
                // counter gravity for standing/running through special slopes
                if ((moveLeft  &&  groundAngle > 0 && groundAngle < 180) ||
                    (moveRight &&  groundAngle > 180 && groundAngle < 360) ||
                    (grounded && (groundAngle > 360 - 45 || groundAngle < 0 + 45))) {
                    pos.y -= gravity;
                }
                //
                // now push player outside slope:
                while (checkTerrain(pos + toFeet)) {
                    // slope is decent, push straight up
                    if (flatGround) {
                        fixes++;
                        pos.y--;

                    }
                    // slope is steep, push outside terrain
                    else {
                        pos += vec(0, -1).rotated(groundAngle);
                        float steepSlopeAngle = getTerrainNormal(pos + toFeet, 32);
                        if (!grounded) {
                            // sliding is set true when player touches steep walls
                            // sliding is set false when player goes X frames without touching steep walls
                            // slidingTracker is updated
                            sliding = true;
                            slidingTracker = slidingSteps;
                            canDoubleJump = true;
                            ghostJumpTracker = ghostJumpSteps;
                            // maybe enclose following 2 lines in this block
                        }
                        if (steepSlopeAngle < 270) idealAngle = steepSlopeAngle + 180;
                        else idealAngle = steepSlopeAngle;
                    }
                }
                // prevent shooting through space
                // set the player grounded if
                // -standing on flat ground or
                // -not grabbing
                // correct last position to prevent bouncing out of terrain
                // noite: this doesnt seem necessary
                if (!grounded) {
                    while (checkTerrain(posLast + toFeet)) {
                        posLast += vec(1, 0).rotated(groundAngle + 270);
                        //say("Shazam!");
                    }
                }
                if (flatGround || grounded) {// || abs(angle - groundAngle) < 45) {//|| (!leftGrab && !rightGrab)) { // && !leftAction && !rightAction)) {
                                             // set ideal player idealAngle perpendicular to slope
                                             //idealAngle = groundAngle;
                                             //if (!grounded) playSound(randomChoice(footsteps), &worldNozzle, 20);
                                             // player just hit ground
                    if (!grounded) {
                        //say("Grounded: " << abs(angle - groundAngle));
                        playSound(randomChoice(footsteps), pos, 20);
                    }
                    grounded = true;
                    goombaStomped = false;
                    headJumpPitch = 1;
                    canDoubleJump = true;
                    ghostJumpTracker = ghostJumpSteps;
                    //if (!jumping) 
                    idealAngle = groundAngle;
                }
                break;
            }
            // no ground detected, set player ungrounded
            if (groundChecks == 0) {
                grounded = false;
            }
            checkPos += vec(0, 1).rotated(idealAngle); //uh
        }

        if (fixes > 10) posLast = pos;

    }
    if (leftGrab || rightGrab) sliding = false;

    // determine if player is crouched or not
    if (grounded && moveDown) {
        crouched = true;
    }
    // try to uncrouch
    else {
        if (!checkTerrain(pos + vec(0, -50).rotated(angle))) crouched = false;
        // auto crouch
        else if (grounded) crouched = true;
    }


    constrainAngles(&angle, &idealAngle);
    angle += (idealAngle - angle)*.15;

    // arm physics

    // determine shoulder positions
    vec lShoulder, rShoulder, avgShoulder;
    {
        if (dir) {
            lShoulder = pos + (vec(-64, -64) + imageData->armBack).rotated(angle);
            rShoulder = pos + (vec(-64, -64) + imageData->armFront).rotated(angle);
        }
        else {
            lShoulder = pos + (vec(64, -64) + imageData->armFront.flipx()).rotated(angle);
            rShoulder = pos + (vec(64, -64) + imageData->armBack.flipx()).rotated(angle);
        }
        avgShoulder = lShoulder + (rShoulder - lShoulder)*.5;
    }

    //vec lShoulder, rShoulder, avgShoulder;
    // Move player towards grabbed hands
    if (leftGrab) {
        if (grounded && (lShoulder - lArm.pos).getLength() > 62) {
            leftGrab = false;
            leftGrabTracker = grabSteps;
        }
        vec lShoulderLast = lShoulder;
        stickForce(lShoulder, lArm.pos, 0, 62, 0, 1);
        springForce(lShoulder, lArm.pos, 0, 30, 0, 1, 0.02);
        pos += (lShoulder - lShoulderLast);
        // move left hand close to shoulder
        vec toShoulder = (lShoulder - lArm.pos).normalized();
        while (checkTerrain(lArm.pos + toShoulder) && (lArm.pos - lShoulder).getLength() > 62) {
            lArm.pos += toShoulder;
        }
    }
    if (rightGrab) {
        if (grounded && (rShoulder - rArm.pos).getLength() > 62) {
            rightGrab = false;
            rightGrabTracker = grabSteps;
        }
        vec rShoulderLast = rShoulder;
        stickForce(rShoulder, rArm.pos, 0, 62, 0, 1);
        springForce(rShoulder, rArm.pos, 0, 30, 0, 1, 0.02);
        pos += (rShoulder - rShoulderLast);
        // move left hand close to shoulder
        vec toShoulder = (rShoulder - rArm.pos).normalized();
        while (checkTerrain(rArm.pos + toShoulder) && (rArm.pos - rShoulder).getLength() > 62) {
            rArm.pos += toShoulder;
        }
    }

    // pull arms close if animation state is walking/crouch walking
    if (animState == animWalking ||
        animState == animCrouchWalking) {
        // dont apply if arms are grabbing - we do that elsewhere
        if (!leftAction && !leftGrab) {
            lArm.pos     += vel;
            lArm.posLast += vel;
        }
        if (!rightAction && !rightGrab) {
            rArm.pos += vel;
            rArm.posLast += vel;
        }
    }

    // move hands physically
    {
        float armNudge = .1;
        float lhNudge = 0;
        float rhNudge = 0;
        float maxReachDist = 52;
        if (!leftGrab) {
            // reaching
            if (leftAction) {
                // for stability
                lArm.pos += pos - posLast;
                lArm.posLast += pos - posLast;
                ///lArm.gravity = 0;
                // determine ideal left hand position
                vec toMouse = globalMouse - avgShoulder;
                if (toMouse.getLength() > maxReachDist) toMouse.norm(maxReachDist);
                vec idealPos = avgShoulder + toMouse;
                if (rightAction) idealPos += (lShoulder - avgShoulder);
                lArm.pos += (idealPos - lArm.pos)*.04;
                // if hand touches terrain - stick to it
                if (!lItem && !mainItem && !noclip && leftGrabTracker == 0 && checkTerrain(lArm.pos)) {
                    leftGrab = true;
                    if (rightGrab && rightAction && (moveLeft || moveRight || moveUp || moveDown)) {
                        rightGrab = false;
                        if (sprinting) rightGrabTracker = 10;
                        else rightGrabTracker = 20;
                    }
                    playSound(randomChoice(grabSounds), lArm.pos, 40, randnum(0.9, 1.1));
                    canDoubleJump = true;
                    lArm.posLast = lArm.pos;
                }
            }
            // not reaching
            else {
                lhNudge = armNudge;
                springForce(lArm.pos, lShoulder, 15, 30, 0, 1, 0.05);

            }
            // move arm as long as we didnt just grab
            if (!leftGrab) {
                lArm.pos += vec(lhNudge, 0).rotated(angle);
                //addFriction(friction); // posLast = pos + (pos - posLast)*f;
                vec vel = (lArm.pos - lArm.posLast);
                vel *= 0.9; // friction
                lArm.posLast = lArm.pos;
                lArm.pos = lArm.pos + vel;
                if (!noclip && !leftAction) lArm.pos.y += handGravity;
                stickForce(lArm.pos, lShoulder, 0, 62, 0, 1);
            }
        }
        
        if (!rightGrab) {
            if (rightAction) {
                // for stability
                rArm.pos += pos - posLast;
                rArm.posLast += pos - posLast;
                ///rArm.gravity = 0;
                // determine ideal right hand position
                vec toMouse = globalMouse - avgShoulder;
                if (toMouse.getLength() > maxReachDist) toMouse.norm(maxReachDist);
                vec idealPos = avgShoulder + toMouse;
                if (leftAction) idealPos += (rShoulder - avgShoulder);
                rArm.pos += (idealPos - rArm.pos)*.04;
                //new Dot(globalMouse);
                //new Dot(rArm.pos);
                //new Dot(idealPos);
                //cout << idealPos.x << " " << idealPos.y << endl;
                //idealPos.print();
                if (!rItem && !mainItem && !noclip && rightGrabTracker == 0 && checkTerrain(rArm.pos)) {
                    rightGrab = true;
                    if (leftGrab && leftAction && (moveLeft || moveRight || moveUp || moveDown)) {
                        leftGrab = false;
                        if (sprinting) leftGrabTracker = 10;
                        else leftGrabTracker = 20;
                    }
                    playSound(randomChoice(grabSounds), rArm.pos, 40, randnum(0.9, 1.1));
                    canDoubleJump = true;

                    rArm.posLast = rArm.pos;
                }
            }
            else {
                rhNudge = -armNudge;
                springForce(rArm.pos, rShoulder, 15, 30, 0, 1, 0.05);

            }
            if (!rightGrab) {
                rArm.pos += vec(rhNudge, 0).rotated(angle);
                //addFriction(friction); // posLast = pos + (pos - posLast)*f;
                vec vel = (rArm.pos - rArm.posLast);
                vel *= 0.9; // friction
                rArm.posLast = rArm.pos;
                rArm.pos += vel;
                if (!noclip && !rightAction) rArm.pos.y += handGravity; 
                stickForce(rArm.pos, rShoulder, 0, 62, 0, 1);
            }
        }

        // start auto climbing
        if (leftGrab && rightGrab &&
            leftAction && rightAction &&
            (moveUp || moveDown || moveLeft || moveRight)) {
            leftGrab = false;
            if (sprinting) leftGrabTracker = 10;
            else leftGrabTracker = 20;
        }
    }



    // all player physics is done
    body.pos = pos;
    body.angle = angle;


    // update camera
    camLast = cam;
    cam = pos;
    // update "ears"
    sf::Listener::setPosition(pos.x, pos.y, 0);

    // determine direction
    if (moveLeft) dir = false;
    if (moveRight) dir = true;
    bool movementDir = dir;
    // change direction if holding aimed item
    if (lItem || rItem || mainItem) {
        float ang1 = angle;
        float ang2 = (globalMouse - avgShoulder).getAngle();
        normalizeAngle(&ang1);
        normalizeAngle(&ang2);
        while (ang1 < ang2 - 180) ang1 += 360;
        while (ang1 > ang2 + 180) ang1 -= 360;
        float angleDiff = ang1 - ang2;
        if (angleDiff < 90 && angleDiff > -90) dir = true;
        else dir = false;
    }
    float aimAngle = (globalMouse - avgShoulder).getAngle();
    //if (!dir) aimAngle += 180;
    if (lItem) {
        vec handPos = avgShoulder + vec(35, 0).rotated(aimAngle);
        //lItemAng = aimAngle;
        lArm.pos = handPos;
        if (dir) {
            lItem->worldNozzle = handPos + (lItem->nozzle - lItem->grip1).rotated(aimAngle);
            lItem->angle = aimAngle;
        }
        else {
            lItem->worldNozzle = handPos - (lItem->nozzle - lItem->grip1).flipx().rotated(aimAngle);
            lItem->angle = aimAngle;
        }
        ///lItemNozzle = handPos;
    }
    if (rItem) {
        vec handPos;
        new Dot(avgShoulder);
        if (dir) handPos = avgShoulder + vec(35, 0).rotated(aimAngle);
        else        handPos = avgShoulder + vec(35, 0).flipx().rotated(aimAngle);
        new Dot(handPos);
        //rItemSprite.setPosition(handPos.x, handPos.y);
        //rItemSprite.setRotation(aimAngle);
        rArm.pos = handPos;
        ///rItemNozzle = handPos;
    }
    // mainItem - place/rotate weapon and hands
    if (mainItem) {
        vec itemPos;
        if (dir) itemPos = avgShoulder + mainItem->offset.rotated(angle);
        else     itemPos = avgShoulder + mainItem->offset.flipx().rotated(angle);
        vec itemPos2 = itemPos + vec(0, mainItem->nozzle.y - mainItem->origin.y);
        aimAngle = 0;
        int attempts = 0;
        while (abs(aimAngle - (globalMouse - itemPos2).getAngle()) > 1) {
            aimAngle--;
            while (aimAngle < 0) aimAngle += 360;
            while (aimAngle > 360) aimAngle -= 360;
            if (dir) itemPos2 = itemPos + vec(0, mainItem->nozzle.y - mainItem->origin.y).rotated(aimAngle);
            else     itemPos2 = itemPos + vec(0, mainItem->nozzle.y - mainItem->origin.y).flipy().rotated(aimAngle);
            attempts++;
            if (attempts == 360) {
                if (!dir) aimAngle = 180;
                break;
            }
        }
        mainItem->angle = aimAngle;
        vec LH, RH;
        vec lGrip, rGrip;
        if (leftHanded) {
            lGrip = mainItem->grip1;
            rGrip = mainItem->grip2;
        }
        else {
            rGrip = mainItem->grip1;
            lGrip = mainItem->grip2;
        }
        if (dir) {
            mainItem->worldNozzle = itemPos + (mainItem->nozzle - mainItem->origin).rotated(aimAngle);
            LH = itemPos - mainItem->origin.rotated(aimAngle) + lGrip.rotated(aimAngle);
            RH = itemPos - mainItem->origin.rotated(aimAngle) + rGrip.rotated(aimAngle);
        }
        else {
            mainItem->worldNozzle = itemPos + (mainItem->nozzle - mainItem->origin).flipy().rotated(aimAngle);
            LH = itemPos - mainItem->origin.flipy().rotated(aimAngle) + lGrip.flipy().rotated(aimAngle);
            RH = itemPos - mainItem->origin.flipy().rotated(aimAngle) + rGrip.flipy().rotated(aimAngle);
        }

        // shouldnt need to set attatched item positions?
        //bMainItem->set(itemPos, aimAngle, dir);


        lArm.pos = LH; rArm.pos = RH;
        mainItemObj->pos = itemPos;
        mainItemObj->angle = aimAngle;
        if (!dir) mainItemObj->angle += 180;
        mainItemObj->dir = dir;
        lArm.pos = mainItem->grip2;
        // animation
        KeyFrame* kf = mainItem->activeFrame;
        if (kf) {
            float blendRatio = (float)kf->elapsed / kf->duration;
            for (int i = 0; i < kf->displacements.size(); i++) {
                if (kf->displacements[i].objP) {
                    ImageObj* obj = *kf->displacements[i].objP;
                    vec posLast = vec(0,0);
                    vec posNext = vec(0,0) + kf->displacements[i].pos;
                    float angLast = 0;
                    float angNext = 0 + kf->displacements[i].angle;
                    //if (kf->lastFrame) 
                    vec   addedPos = posLast + (posNext - posLast) * blendRatio;
                    float addedAng = angLast + (angNext - angLast) * blendRatio;
                    if (obj->dir) {
                        addedPos.rotate(obj->angle);
                        obj->angle += addedAng;
                    }
                    else {
                        addedPos = addedPos.flipx();
                        addedPos.rotate(obj->angle);
                        obj->angle -= addedAng;
                    }
                    obj->pos += addedPos;
                }
            }
            if (kf->funcP && kf->elapsed == 0) kf->funcP();
            //vec gunPosLast = vec(0, 0);
            //float gunAngLast = 0;
            //vec secondHandPosLast = vec(0, 0);
            //if (kf->lastFrame) {
            //    gunPosLast = kf->lastFrame->gunPos;
            //    gunAngLast = kf->lastFrame->gunAng;
            //    secondHandPosLast = kf->lastFrame->secondHandPos;
            //}
            //vec gunPosNext = kf->gunPos;
            //float gunAngNext = kf->gunAng;
            //vec secondHandPosNext = kf->secondHandPos;
            //float blendRatio = (float)kf->elapsed / kf->duration;
            //vec   addedPos = gunPosLast + (gunPosNext - gunPosLast) * blendRatio;
            //float addedAng = gunAngLast + (gunAngNext - gunAngLast) * blendRatio;
            //vec addedSecondHandPos = secondHandPosLast + (secondHandPosNext - secondHandPosLast) * blendRatio;
            //if (dir) {
            //    //aimAngle += addedAng;
            //    addedPos.rotate(aimAngle);
            //    aimAngle += addedAng;
            //    addedSecondHandPos.rotate(aimAngle);
            //    LH.rotateAround(itemPos, addedAng);
            //    RH.rotateAround(itemPos, addedAng);
            //}
            //else {
            //    //aimAngle -= addedAng;
            //    addedPos = addedPos.flipx();
            //    addedPos.rotate(aimAngle + 180);
            //    aimAngle -= addedAng;
            //    addedSecondHandPos = addedSecondHandPos.flipx();
            //    addedSecondHandPos.rotate(aimAngle + 180);
            //    LH.rotateAround(itemPos, -addedAng);
            //    RH.rotateAround(itemPos, -addedAng);
            //
            //}
            //itemPos += addedPos;
            //
            ////new Dot(itemPos); 
            //LH += addedPos;
            //LH += addedSecondHandPos;
            //RH += addedPos;
            //
            kf->elapsed++;
            if (kf->elapsed == kf->duration) {
                kf->elapsed = 0;
                mainItem->activeFrame = kf->nextFrame;
            }
        }
        // end animation
    }
    //say(aimAngle);
    // item actions/animations - after item positions are set
    if (mainItem) {
        if (leftActionPressed) {
            mainItem->pressEvent();
        }
        if (leftAction && mainItem->automatic) mainItem->shoot();
        if (leftActionReleased) {
            mainItem->releaseEvent();
        }
        if (reloading) {
            mainItem->reload();
        }

    }
    if (lItem) {
        if (leftActionPressed) {
            lItem->pressEvent();
        }
        if (leftAction && lItem->automatic) lItem->shoot();
        if (leftActionReleased) {
            lItem->releaseEvent();
        }
        if (reloading) {
            lItem->reload();
        }

    }
    if (rItem) {
        if (rightActionPressed) {
            rItem->pressEvent();
        }
        if (rightAction && rItem->automatic) rItem->shoot();
        if (rightActionReleased) {
            rItem->releaseEvent();
        }
        if (reloading) {
            rItem->reload();
        }

    }

    // determine animation state
    if (grounded) {
        if (moveLeft != moveRight) {
            if (crouched) animState = animCrouchWalking;
            else {
                if (sprinting) animState = animSprinting;
                else animState = animWalking;
            }
        }
        else {
            if (crouched) animState = animCrouching;
            else animState = animStanding;
        }
    }
    // in air
    else {
        float DP = vel.rotated(-angle)*vec(0, 1);
        if (DP < -2 && animState != animJumping) animState = animRising;
        if (DP > -2) animState = animFalling;
    }

    if (movementDir != dir) {
        if (animState == animCrouchWalking) animState = animBackwardsCrouchWalking;
        if (animState == animWalking) animState = animBackwardsWalking;
        if (animState == animSprinting) animState = animBackwardsSprinting;

    }

    //if (justJumped) animState = animJumping;
    // reset per-tick input variables
    {
        justJumped = false;
        leftActionReleased = false;
        rightActionReleased = false;
        leftActionPressed = false;
        rightActionPressed = false;
    }
    if (invincibilitySteps>0) invincibilitySteps--;
    if (invincibilitySteps && invincibilitySteps % 4 == 0) shader = shaderFlashWhite;
    else shader = nullptr;
    if (regenerate>0) regenerate--; // can only regenerate health when "regenerate == 0"
    if (regenerate == 0) {
        health += maxHealth*.004;
        if (health > maxHealth) health = maxHealth;
    }
    if (health < maxHealth) showHealthBar = 60;
    if (showHealthBar > 0) showHealthBar--; // stop drawing health bar when "showHealthBar == 0"

                                            //updateRegions();
}

void Player::setImages()
{
    body.layer = 0;
    body.dir = lArm.dir = rArm.dir = dir;
    //lArm->pos = lArmPos;
    //rArm->pos = rArmPos;
    if (dir) { // facing right
        lArm.layer = -1;
        rArm.layer = 1;
    }
    else { // facing left
        rArm.layer = -1;
        lArm.layer = 1;
    }
    // set arm images too
    // based on interpolated positions




    if (dirLast != dir) {
        //lItemAngInterp = lItemAng;
        //rItemAngInterp = rItemAng;
        //mainItemAngInterp = mainItemAng;
    }
    dirLast = dir;
    //if (dir) body->sprite.setScale(1, 1);
    //else     body->sprite.setScale(-1, 1);
    //body->sprite.setPosition(posInterp.x, posInterp.y);
    body.sprite.setOrigin(64, 64);





    // interpolation goodies
    //body
    vec   posRender = interpVec(body.posInterp, body.pos);
    float angRender = interpAng(body.angInterp, body.angle);
    //body->sprite.setPosition(posRender.x, posRender.y);
    //body->sprite.setRotation(angRender);
    //body->pos = posRender;
    ////lArm
    vec   lHandPosRender = interpVec(lArm.posInterp, lArm.pos);
    //lArm->sprite.setPosition(lHandPosRender.x, lHandPosRender.y);
    ////rArm
    vec   rHandPosRender = interpVec(rArm.posInterp, rArm.pos);



    //rArm->sprite.setPosition(rHandPosRender.x, rHandPosRender.y);
    ////lItem
    //vec   lItemPosRender = interpVec(lItemPosInterp, lItemPos);
    //float lItemAngRender = interpAng(lItemAngInterp, lItemAng);
    //lItemSprite.setPosition(lHandPosRender.x, lHandPosRender.y);
    //lItemSprite.setRotation(lItemAngRender);
    //if (lItem) {
    //    for (int i = 0; i < lItem->sprites.size(); i++) {
    //        lItem->sprites[i].setPosition(lItemPosRender.x, lItemPosRender.y);
    //        lItem->sprites[i].setRotation(lItemAngRender);
    //    }
    //}
    //rItem
    //vec   rItemPosRender = interpVec(rItemPosInterp, rItemPos);
    //float rItemAngRender = interpAng(rItemAngInterp, rItemAng);
    //rItemSprite.setPosition(rItemPosRender.x, rItemPosRender.y);
    //rItemSprite.setRotation(rItemAngRender);
    //if (rItem) {
    //    for (int i = 0; i < rItem->sprites.size(); i++) {
    //        rItem->sprites[i].setPosition(rItemPosRender.x, rItemPosRender.y);
    //        rItem->sprites[i].setRotation(rItemAngRender);
    //    }
    //}
    //mainItem
    //vec   mainItemPosRender = interpVec(mainItemPosInterp, mainItemPos);
    //float mainItemAngRender = interpAng(mainItemAngInterp, mainItemAng);
    //mainItemSprite.setPosition(mainItemPosRender.x, mainItemPosRender.y);
    //mainItemSprite.setRotation(mainItemAngRender);
    //if (mainItem) {
    //    for (int i = 0; i < mainItem->sprites.size(); i++) {
    //        mainItem->sprites[i].setPosition(mainItemPosRender.x, mainItemPosRender.y);
    //        mainItem->sprites[i].setRotation(mainItemAngRender);
    //    }
    //}
    vec lShoulderPos, rShoulderPos;
    if (dir) {
        lShoulderPos = posRender + (vec(-64, -64) + imageData->armBack).rotated(angRender);
        rShoulderPos = posRender + (vec(-64, -64) + imageData->armFront).rotated(angRender);
        //lArm->sprite.setScale(1, 1);
        //rArm->sprite.setScale(1, 1);
    }
    else {
        lShoulderPos = posRender + (vec(64, -64) + imageData->armFront.flipx()).rotated(angRender);
        rShoulderPos = posRender + (vec(64, -64) + imageData->armBack.flipx()).rotated(angRender);
        //lArm->sprite.setScale(1, -1);
        //rArm->sprite.setScale(1, -1);
    }
    float lAng = (lHandPosRender - lShoulderPos).getAngle();
    float rAng = (rHandPosRender - rShoulderPos).getAngle();
    if (!dir) { lAng += 180; rAng += 180; }
    lArm.angInterp = lArm.angle = lAng;
    rArm.angInterp = rArm.angle = rAng;


    float lHandDist = (lHandPosRender - lShoulderPos).getLength();
    float rHandDist = (rHandPosRender - rShoulderPos).getLength();
    float lHandAngle = (lHandPosRender - lShoulderPos).getAngle();
    float rHandAngle = (rHandPosRender - rShoulderPos).getAngle();
    lArm.sprite.setRotation(lHandAngle);
    rArm.sprite.setRotation(rHandAngle);
    int lIndex = 2;
    int rIndex = 2;
    while (lIndex < lHandDist - 0 && lIndex < 62) lIndex += 2;
    while (rIndex < rHandDist - 0 && rIndex < 62) rIndex += 2;
    lArmData = &arms[int(lIndex)];
    rArmData = &arms[int(rIndex)];
    lArm.sprite.setTexture(lArmData->texture, true);
    rArm.sprite.setTexture(rArmData->texture, true);
    lArm.sprite.setOrigin(lArmData->hand.x, lArmData->hand.y);
    rArm.sprite.setOrigin(rArmData->hand.x, rArmData->hand.y);



}

//void Player::renderBack()
//{
//    if (dirLast != dir) {
//        lItemAngInterp = lItemAng;
//        rItemAngInterp = rItemAng;
//        mainItemAngInterp = mainItemAng;
//    }
//    dirLast = dir;
//    if (dir) sprite.setScale(1, 1);
//    else     sprite.setScale(-1, 1);
//    sprite.setPosition(posInterp.x, posInterp.y);
//    sprite.setOrigin(64, 64);
//
//    
//    
//
//
//    // interpolation goodies
//    //body
//    vec   posRender = interpVec(posInterp, pos);
//    float angRender = interpAng(angInterp, angle);
//    sprite.setPosition(posRender.x, posRender.y);
//    sprite.setRotation(angRender);
//    //lArm
//    vec   lHandPosRender = interpVec(lHandPosInterp, lHandPos);
//    lArmSprite.setPosition(lHandPosRender.x, lHandPosRender.y);
//    //rArm
//    vec   rHandPosRender = interpVec(rHandPosInterp, rHandPos);
//    rArmSprite.setPosition(rHandPosRender.x, rHandPosRender.y);
//    //lItem
//    vec   lItemPosRender = interpVec(lItemPosInterp, lItemPos);
//    float lItemAngRender = interpAng(lItemAngInterp, lItemAng);
//    //lItemSprite.setPosition(lHandPosRender.x, lHandPosRender.y);
//    //lItemSprite.setRotation(lItemAngRender);
//    if (lItem) {
//        for (int i = 0; i < lItem->sprites.size(); i++) {
//            lItem->sprites[i].setPosition(lItemPosRender.x, lItemPosRender.y);
//            lItem->sprites[i].setRotation(lItemAngRender);
//        }
//    }
//    //rItem
//    vec   rItemPosRender = interpVec(rItemPosInterp, rItemPos);
//    float rItemAngRender = interpAng(rItemAngInterp, rItemAng);
//    //rItemSprite.setPosition(rItemPosRender.x, rItemPosRender.y);
//    //rItemSprite.setRotation(rItemAngRender);
//    if (rItem) {
//        for (int i = 0; i < rItem->sprites.size(); i++) {
//            rItem->sprites[i].setPosition(rItemPosRender.x, rItemPosRender.y);
//            rItem->sprites[i].setRotation(rItemAngRender);
//        }
//    }
//    //mainItem
//    vec   mainItemPosRender = interpVec(mainItemPosInterp, mainItemPos);
//    float mainItemAngRender = interpAng(mainItemAngInterp, mainItemAng);
//    //mainItemSprite.setPosition(mainItemPosRender.x, mainItemPosRender.y);
//    //mainItemSprite.setRotation(mainItemAngRender);
//    if (mainItem) {
//        for (int i = 0; i < mainItem->sprites.size(); i++) {
//            mainItem->sprites[i].setPosition(mainItemPosRender.x, mainItemPosRender.y);
//            mainItem->sprites[i].setRotation(mainItemAngRender);
//        }
//    }
//    if (dir) {
//        lShoulderPos = posRender + (vec(-64, -64) + imageData->armBack ).rotated(angRender);
//        rShoulderPos = posRender + (vec(-64, -64) + imageData->armFront).rotated(angRender);
//        lArmSprite.setScale(1, 1);
//        rArmSprite.setScale(1, 1);
//    }
//    else {
//        lShoulderPos = posRender + (vec(64, -64) + imageData->armFront.flipx()).rotated(angRender);
//        rShoulderPos = posRender + (vec(64, -64) + imageData->armBack .flipx()).rotated(angRender);
//        lArmSprite.setScale(1, -1);
//        rArmSprite.setScale(1, -1);
//    }
//    // beep
//    // boop
//    float lHandDist  = (lHandPosRender - lShoulderPos).getLength();
//    float rHandDist  = (rHandPosRender - rShoulderPos).getLength();
//    float lHandAngle = (lHandPosRender - lShoulderPos).getAngle();
//    float rHandAngle = (rHandPosRender - rShoulderPos).getAngle();
//    lArmSprite.setRotation(lHandAngle);
//    rArmSprite.setRotation(rHandAngle);
//    int lIndex = 2;
//    int rIndex = 2;
//    while (lIndex < lHandDist - 0 && lIndex < 62) lIndex += 2;
//    while (rIndex < rHandDist - 0 && rIndex < 62) rIndex += 2;
//    lArmData = arms[int(lIndex)];
//    rArmData = arms[int(rIndex)];
//    lArmSprite.setTexture(lArmData->texture, true);
//    rArmSprite.setTexture(rArmData->texture, true);
//    lArmSprite.setOrigin(lArmData->hand.x, lArmData->hand.y);
//    rArmSprite.setOrigin(rArmData->hand.x, rArmData->hand.y);
//
//
//
//    //// start drawing
//    //if (dir) {
//    //    window->draw(lArmSprite,shader);
//    //    if (lItem) {
//    //        //lItemSprite.setScale(1, 1);
//    //        for (int i = 0; i < lItem->sprites.size(); i++) {
//    //            lItem->sprites[i].setScale(1, 1);
//    //        }
//    //        //window->draw(lItemSprite);
//    //        lItem->render();
//    //    }
//    //    else if (mainItem) {
//    //        //mainItemSprite.setScale(1, 1);
//    //        for (int i = 0; i < mainItem->sprites.size(); i++) {
//    //            mainItem->sprites[i].setScale(1, 1);
//    //        }
//    //        //window->draw(mainItemSprite);
//    //        mainItem->render();
//    //    }
//    //}
//    //else {
//    //    window->draw(rArmSprite,shader);
//    //    if (rItem) {
//    //        //rItemSprite.setScale(1, -1);
//    //        for (int i = 0; i < rItem->sprites.size(); i++) {
//    //            rItem->sprites[i].setScale(1, -1);
//    //        }
//    //        //window->draw(rItemSprite);
//    //        rItem->render();
//    //    }
//    //    else if (mainItem) {
//    //        //mainItemSprite.setScale(1, -1);
//    //        for (int i = 0; i < mainItem->sprites.size(); i++) {
//    //            mainItem->sprites[i].setScale(1, -1);
//    //        }
//    //        //window->draw(mainItemSprite);
//    //        mainItem->render();
//    //    }
//    //}
//
//
//
//    
//
//}


//void Player::renderFront()
//{
//
//    // body
//    window->draw(sprite,shader);
//
//
//
//
//    // front
//    if (dir) {
//        if (rItem) {
//            //rItemSprite.setScale(1, 1);
//            for (int i = 0; i < rItem->sprites.size(); i++) {
//                rItem->sprites[i].setScale(1, 1);
//            }
//            //window->draw(rItemSprite);
//            rItem->render();
//        }
//        else if (!leftHanded && mainItem) {
//            //mainItemSprite.setScale(1, 1);
//            for (int i = 0; i < mainItem->sprites.size(); i++) {
//                mainItem->sprites[i].setScale(1, 1);
//            }
//            //window->draw(mainItemSprite);
//            mainItem->render();
//        }
//        window->draw(rArmSprite,shader);
//    }
//    else {
//        if (lItem) {
//            //lItemSprite.setScale(1, -1);
//            for (int i = 0; i < lItem->sprites.size(); i++) {
//                lItem->sprites[i].setScale(1, -1);
//            }
//            //window->draw(lItemSprite);
//            lItem->render();
//        }
//        else if (leftHanded && mainItem) {
//            //mainItemSprite.setScale(1, -1);
//            for (int i = 0; i < mainItem->sprites.size(); i++) {
//                mainItem->sprites[i].setScale(1, -1);
//            }
//            //window->draw(mainItemSprite);
//            mainItem->render();
//        }
//        window->draw(lArmSprite,shader);
//    }
//}


void Player::setLeftItem(Weapon* item)
{
    lArm.parent = nullptr;
    rArm.parent = nullptr;
    mainItem = nullptr;
    lItem = item;
    // could be NULL
    if (item) {
        if (item->isTwoHanded) {
            //mainItemSprite.setTexture(item->textures[0], true);
            //mainItemSprite.setOrigin(item->origin.x, item->origin.y);
            mainItem = item;
            mainItemObj = new ImageObj;
            if (item->sprites.size() > 0) mainItemObj->sprite = item->sprites[0];
            // add later for special guns like shotgun
            // if (item->equipFunc) item->equipFunc();
            lItem = nullptr;
            rItem = nullptr;
            lArm.parent = mainItemObj;
            rArm.parent = mainItemObj;
            //mainItemPos = pos;
            //mainItemPosInterp = pos;

        }
        else {
            //lItemSprite.setTexture(item->textures[0], true);
            //lItemSprite.setOrigin(item->grip1.x, item->grip1.y);
            delete mainItemObj;
        }
    }
    //else lItemSprite.setTexture(Texture(), true);
}

void Player::setRightItem(Weapon* item)
{
    mainItem = nullptr;
    rItem = item;
    if (item) {
        if (item->isTwoHanded) {
            //mainItemSprite.setTexture(item->textures[0], true);
            //mainItemSprite.setOrigin(item->origin.x, item->origin.y);
            mainItem = item;
            lItem = nullptr;
            rItem = nullptr;
            //mainItemPos = pos;
            //mainItemPosInterp = pos;
        }
        else {
            //rItemSprite.setTexture(item->textures[0], true);
            //rItemSprite.setOrigin(item->grip1.x, item->grip1.y);
        }
    }
    //else rItemSprite.setTexture(Texture(), true);
}

void Player::hurt(float damage) {
    if (invincibilitySteps == 0) {
        playSound(randomChoice(hits), pos, 15, randnum(.9, 1.1));
        health -= damage;
        invincibilitySteps = 30;
        regenerate = 240;
    }

}