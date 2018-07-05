#include "Player.h"
#include "Particle.h"


extern float physRate;
int showHealthBar = 0;
float headJumpPitch = 1;
/* 


TODO: 

-Player sliding sounds
-Animations (mostly copy paste)
-Fruit Smoothie
-Barack Obama
-Ducking
    -Change hitbox checks when ducked
        -Keep diamond shape?
    -Duck walk quiet footsteps, try around volume 5
-Fix wall jumping to allow >90 degree slopes
-ARMS!!!!
    -ARMS THAT STICK TO WALLS
-GUNS!!!!
-RANDOMLY GENERATED GUNS!!!
-TERRAIN GRASS!!!!!
-MULTIPLAYER!!!
    -FUCKING GRAB OTHER PLAYERS WITH YOUR ARMS
    -HATS!!!! OF FUCKING COURSE IT HAS HATS!!!!
-PATH FINDING ENEMIES!!!!
    -FUCKIN NODES THAT CONNECT PATHS
        -PATHS FOR GROUNDED ENEMIES
        -PATHS FOR AIRBORNE ENEMIES
        -PATHS FOR ENEMIES THAT MOVE THROUGH THE GROUND????????
            -that sounds fuckin stupid just use direction
            -but really maybe if theres a hard material in the way
            -we could swerve around it
            -we need a hard material first :)
-rolling donut daggers
    -randomly generated
    -sprinkles
    -destroy terrain
    -crush enemies
-Inventory
-Hotbar
*/

#define randomChoice(vector) vector[randint(0,vector.size()-1)]
#define forV(vector) for (int i=0;i<vector.size();i++)

Player::Player()
{
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
            crouchWalk.emplace_back(new ImageData(playerCrouchWalk[i], skinColor, shirtColor, pantsColor));
        }
        forV(playerJumpToFall) {
            jumpToFall.emplace_back(new ImageData(playerJumpToFall[i], skinColor, shirtColor, pantsColor));
        }
        forV(playerRun) {
            run.emplace_back(new ImageData(playerRun[i], skinColor, shirtColor, pantsColor));
        }
        forV(playerStandToCrouch) {
            standToCrouch.emplace_back(new ImageData(playerStandToCrouch[i], skinColor, shirtColor, pantsColor));
        }
        stand = new ImageData(playerStand, skinColor, shirtColor, pantsColor);
        crouch = new ImageData(playerCrouch, skinColor, shirtColor, pantsColor);
        jump = new ImageData(playerJump, skinColor, shirtColor, pantsColor);
        fall = new ImageData(playerFall, skinColor, shirtColor, pantsColor);
        for (auto armIt = playerArm.begin(); armIt != playerArm.end(); armIt++) {
            arms[armIt->first] = new ArmData(armIt->second, skinColor, shirtColor, pantsColor);
        }
        this->imageData = fall;
    }

    //texture.loadFromImage(imgPlayerStand);
    //sprite.setTexture(texture);
    sprite.setOrigin(64, 64);
    updateRegions();
}


Player::~Player()
{
    say("Deleted Player");
}

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

extern bool leftGrab       ;
extern bool rightGrab      ;
extern bool leftAction     ;
extern bool rightAction    ;
extern bool leftActionReleased ;
extern bool rightActionReleased;
extern bool leftActionPressed  ;
extern bool rightActionPressed ;
extern bool reloading;
extern vec globalMouse     ;
int grabSteps = 12;
int leftGrabTracker ;
int rightGrabTracker;
float handGravity = 0.5;
void springForce(vec& p1, vec& p2);
void stickForce(vec& p1, vec& p2);



PhysPoint* ppLShoulder;
PhysPoint* ppLHand;
Spring*    lArmSpring;
Stick*     lArmStick;
PhysPoint* ppRShoulder;
PhysPoint* ppRHand;
Spring*    rArmSpring;
Stick*     rArmStick;

bool goombaStomped = false;

// only the controlled player will call this
void Player::move()
{

    // manage trackers
    if (ghostJumpTracker > 0) ghostJumpTracker--;
    if (slidingTracker   > 0) slidingTracker--;
    if (leftGrabTracker > 0) leftGrabTracker--;
    if (rightGrabTracker > 0) rightGrabTracker--;
    if (leftAction && !checkTerrain(ppLHand->pos)) leftGrabTracker = 0;
    if (rightAction && !checkTerrain(ppRHand->pos)) rightGrabTracker = 0;
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
    // item actions/animations
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
    if (sprinting) { // && !crouched) {
        speed *= sprint_mult;
    }

    if (grounded) addForce.rotate(angle);
    addForce.norm(speed);
    
    // Move player towards grabbed hands
    if (leftGrab) {
        PhysPoint p1; p1.setPosition(ppLShoulder->pos);
        PhysPoint p2; p2.setPosition(ppLHand->pos);
        if (grounded && (p1.pos - p2.pos).getLength() > 62) {
            leftGrab = false;
            leftGrabTracker = grabSteps;
        }
        stickForce(&p1, &p2, 0, 62, 0, 1);
        springForce(&p1, &p2, 0, 30, 0, 1, 0.02);
        pos += (p1.pos - p1.posLast);
        // move left hand close to shoulder
        vec toShoulder = (ppLShoulder->pos - ppLHand->pos).normalized();
        while (checkTerrain(ppLHand->pos + toShoulder) && (ppLHand->pos - ppLShoulder->pos).getLength() > 62) {
            ppLHand->pos += toShoulder;
        }
        ppLHand->posLast = ppLHand->pos;
    }
    if (rightGrab) {
        PhysPoint p1; p1.setPosition(ppRShoulder->pos);
        PhysPoint p2; p2.setPosition(ppRHand->pos);
        if (grounded && (p1.pos - p2.pos).getLength() > 62) {
            rightGrab = false;
            rightGrabTracker = grabSteps;
        }
        stickForce(&p1, &p2, 0, 62, 0, 1);
        springForce(&p1, &p2, 0, 30, 0, 1, 0.08);
        pos += (p1.pos - p1.posLast);
        // move left hand close to shoulder
        vec toShoulder = (ppRShoulder->pos - ppRHand->pos).normalized();
        while (checkTerrain(ppRHand->pos + toShoulder) && (ppRHand->pos - ppRShoulder->pos).getLength() > 62) {
            ppRHand->pos += toShoulder;
        }
        ppRHand->posLast = ppRHand->pos;
    }

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
                    pVel.y = randnum(-1,2);
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
        pos += vec(0, -jumpForce).rotated(angle+extraJumpAngle);
        extraJumpAngle *= .6;
    }

    // terrain collision
    vector<vec> simpleCorrections;
    if (moveDown) crouched = true;
    if (crouched) 
        simpleCorrections = {
        vec(  0,  5).rotated(angle),  // toHead 
        vec( 10, 34).rotated(angle),  // toRight 
        vec(-10, 34).rotated(angle),  // toLeft 
        vec( 5,  20).rotated(angle),
        vec(-5,  20).rotated(angle),
        vec( 5,  49).rotated(angle),
        vec(-5,  49).rotated(angle)
        };
    else
        simpleCorrections = {
        vec(  0, -50).rotated(angle),  // toHead 
        vec( 10,   7).rotated(angle),  // toRight 
        vec(-10,   7).rotated(angle),  // toLeft 
        vec(  5, -21).rotated(angle),
        vec( -5, -21).rotated(angle),
        vec(  5,  35).rotated(angle),
        vec( -5,  35).rotated(angle) 
        };
    //new Dot(p->pos+toHead);
    //new Dot(p->pos + toLeft);
    //new Dot(p->pos + toRight);

    vec toFeet = vec(0, 64).rotated(angle);

    // goomba stopm
    if (!noclip && !grounded && vel.y > 4) {
        for (int i = 0; i < activeObjects.size(); i++) {
            GameObject* obj = activeObjects[i];
            if (obj->isEnemy && obj->health > 0 && (obj->pos - (pos + toFeet)).getLength() < 70 && pos.y > obj->pos.y-80) {
                obj->hurt(obj->health + 1);
                obj->posLast.y = obj->pos.y;
                obj->pos.y += randnum(2, 5); // send down a bit
                //posLast.y = pos.y;
                vel.y = -10; // send player up a bit
                jumpForce = jumpForceMax;
                canDoubleJump = true;
                goombaStomped = true;
                fallFrame = 0; // makes player snap to jump frame
                playSound(headJump, pos + toFeet, 20, headJumpPitch);
                headJumpPitch*=1.0594627424; // chromatic scale
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
                        float steepSlopeAngle = getTerrainNormal(pos + toFeet,32);
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
                        if (steepSlopeAngle < 270) idealAngle = steepSlopeAngle+180;
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
                    //if (!grounded) playSound(randomChoice(footsteps), &pos, 20);
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
        ppLShoulder->pos = lShoulder;
        ppRShoulder->pos = rShoulder;
    }
    // pull arms close if animation state is walking/crouch walking
    if (animState == animWalking || 
        animState == animCrouchWalking) { 
        // dont apply if arms are grabbing - we do that elsewhere
        if (!leftAction && !leftGrab) {
            ppLHand->pos += vel;
            ppLHand->posLast += vel;
        }
        if (!rightAction && !rightGrab) {
            ppRHand->pos += vel;
            ppRHand->posLast += vel;
        }
    }

    // move hands, change behavior if hand is reaching or grabbing
    {
        float armNudge = .1;
        float lhMinDist = 0;
        float rhMinDist = 0;
        float lhNudge = 0;
        float rhNudge = 0;
        float maxReachDist = 52;
        if (!leftGrab) {
            if (leftAction) {
                // for stability
                ppLHand->pos += pos - posLast;
                ppLHand->posLast += pos - posLast;
                ppLHand->gravity = 0;
                // determine ideal left hand position
                vec toMouse = globalMouse - avgShoulder;
                if (toMouse.getLength() > maxReachDist) toMouse.norm(maxReachDist);
                vec idealPos = avgShoulder + toMouse;
                if (rightAction) idealPos += (lShoulder - avgShoulder);
                ppLHand->pos += (idealPos - ppLHand->pos)*.04;
                if (!lItem && !mainItem && !noclip && leftGrabTracker == 0 && checkTerrain(ppLHand->pos)) {
                    leftGrab = true;
                    if (rightGrab && rightAction && (moveLeft || moveRight || moveUp || moveDown)) {
                        rightGrab = false;
                        if (sprinting) rightGrabTracker = 10;
                        else rightGrabTracker = 20;
                    }
                    playSound(randomChoice(grabSounds), ppLHand->pos, 40, randnum(0.9, 1.1));
                    canDoubleJump = true;

                    ppLHand->posLast = ppLHand->pos;
                }
            }
            else {
                if (noclip) ppLHand->gravity = 0;
                else ppLHand->gravity = handGravity;
                lhMinDist = 15;
                lhNudge = armNudge;
                springForce(ppLHand, ppLShoulder, lhMinDist, 30, 0, 1, 0.05);

            }
            if (!leftGrab) {
                ppLHand->pos += vec(lhNudge, 0).rotated(angle);
                ppLHand->move();
                stickForce(ppLHand, ppLShoulder, 0, 62, 0, 1);
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
        if (!rightGrab) {
            if (rightAction) {
                // for stability
                ppRHand->pos += pos - posLast;
                ppRHand->posLast += pos - posLast;
                ppRHand->gravity = 0;
                // determine ideal right hand position
                vec toMouse = globalMouse - avgShoulder;
                if (toMouse.getLength() > maxReachDist) toMouse.norm(maxReachDist);
                vec idealPos = avgShoulder + toMouse;
                if (leftAction) idealPos += (rShoulder - avgShoulder);
                ppRHand->pos += (idealPos - ppRHand->pos)*.04;
                if (!rItem && !mainItem && !noclip && rightGrabTracker == 0 && checkTerrain(ppRHand->pos)) {
                    rightGrab = true;
                    if (leftGrab && leftAction && (moveLeft || moveRight || moveUp || moveDown)) {
                        leftGrab = false;
                        if (sprinting) leftGrabTracker = 10;
                        else leftGrabTracker = 20;
                    }
                    playSound(randomChoice(grabSounds), ppRHand->pos, 40, randnum(0.9, 1.1));
                    canDoubleJump = true;

                    ppRHand->posLast = ppRHand->pos;
                }
            }
            else {
                if (noclip) ppRHand->gravity = 0;
                else ppRHand->gravity = handGravity;
                rhMinDist = 15;
                rhNudge = -armNudge;
                springForce(ppRHand, ppRShoulder, rhMinDist, 30, 0, 1, 0.05);

            }
            if (!rightGrab) {
                ppRHand->pos += vec(rhNudge, 0).rotated(angle);
                ppRHand->move();
                stickForce(ppRHand, ppRShoulder, 0, 62, 0, 1);
            }
        }
        lHandPos = ppLHand->pos;
        rHandPos = ppRHand->pos;
    }

    if (leftGrab || rightGrab) sliding = false;


    // all player physics is done

    // update camera
    camLast = cam;
    cam = pos;
    // update "ears"
    sf::Listener::setPosition(pos.x, pos.y, 0);

    // determine direction
    if (moveLeft) dir = false;
    if (moveRight) dir = true;


    float aimAngle = (globalMouse - avgShoulder).getAngle();
    if (!dir) aimAngle += 180;
    if (lItem) {
        //
        vec handPos;
        new Dot(avgShoulder);
        if (dir) handPos = avgShoulder + vec(35, 0).rotated(aimAngle);
        else        handPos = avgShoulder + vec(35, 0).flipx().rotated(aimAngle);
        new Dot(handPos);
        lItemSprite.setPosition(handPos.x, handPos.y);
        lItemSprite.setRotation(aimAngle);
        lHandPos = handPos;
        ///lItemNozzle = handPos;
    }
    if (rItem) {
        vec handPos;
        new Dot(avgShoulder);
        if (dir) handPos = avgShoulder + vec(35, 0).rotated(aimAngle);
        else        handPos = avgShoulder + vec(35, 0).flipx().rotated(aimAngle);
        new Dot(handPos);
        rItemSprite.setPosition(handPos.x, handPos.y);
        rItemSprite.setRotation(aimAngle);
        rHandPos = handPos;
        ///rItemNozzle = handPos;
    }
    if (mainItem) {
        vec itemPos;
        if (dir) {
            itemPos = avgShoulder + mainItem->offset.rotated(angle);
        }
        else {
            itemPos = avgShoulder + mainItem->offset.flipx().rotated(angle);
        }
        //new Dot(itemPos,Color::Red,2);
        vec itemPos2;
        if (dir) itemPos2 = itemPos + vec(0, mainItem->nozzle.y - mainItem->origin.y);
        else        itemPos2 = itemPos + vec(0, mainItem->nozzle.y - mainItem->origin.y).flipx();
        aimAngle = 0;
        int attempts = 0;
        while (abs(aimAngle - (globalMouse - itemPos2).getAngle()) > 1) {
            aimAngle--;
            while (aimAngle < 0) aimAngle += 360;
            while (aimAngle > 360) aimAngle -= 360;
            if (dir) itemPos2 = itemPos + vec(0, mainItem->nozzle.y - mainItem->origin.y).rotated(aimAngle);
            else        itemPos2 = itemPos + vec(0, mainItem->nozzle.y - mainItem->origin.y).flipx().rotated(aimAngle + 180);
            //new Dot(itemPos2, Color::Blue, 2);
            attempts++;
            if (attempts == 360) {
                if (!dir) aimAngle = 180;
                break;
            }
        }

        if (dir) mainItem->pos = itemPos + (mainItem->nozzle - mainItem->origin).rotated(aimAngle);
        else        mainItem->pos = itemPos + (mainItem->nozzle - mainItem->origin).flipx().rotated(aimAngle + 180);
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
            LH = itemPos - mainItem->origin.rotated(aimAngle);
            LH += lGrip.rotated(aimAngle);
            RH = itemPos - mainItem->origin.rotated(aimAngle);
            RH += rGrip.rotated(aimAngle);

        }
        else {
            LH = itemPos - mainItem->origin.flipx().rotated(aimAngle + 180);
            LH += lGrip.flipx().rotated(aimAngle + 180);
            RH = itemPos - mainItem->origin.flipx().rotated(aimAngle + 180);
            RH += rGrip.flipx().rotated(aimAngle + 180);
            aimAngle += 180;
        }

        mainItemSprite.setPosition(itemPos.x, itemPos.y);
        lHandPos = LH;
        rHandPos = RH;
        mainItemSprite.setRotation(aimAngle);

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
        if (DP >  2) animState = animFalling;
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
    if (invincibilitySteps && invincibilitySteps % 4 == 0) shader = &shaderFlashWhite;
    else shader = nullptr;
    if (regenerate>0) regenerate--; // can only regenerate health when "regenerate == 0"
    if (regenerate == 0) {
        health += maxHealth*.004;
        if (health > maxHealth) health = maxHealth;
    }
    if (health < maxHealth) showHealthBar = 60;
    if (showHealthBar > 0) showHealthBar--; // stop drawing health bar when "showHealthBar == 0"

    updateRegions();
}

void Player::renderBack()
{
    
    if (dir) sprite.setScale(1, 1);
    else     sprite.setScale(-1, 1);
    sprite.setPosition(posInterp.x, posInterp.y);
    sprite.setOrigin(64, 64);

    //new Dot(point.posLast);
    if (dir) {
        lShoulderPos = pos + (vec(-64, -64) + imageData->armBack).rotated(angle);
        rShoulderPos = pos + (vec(-64, -64) + imageData->armFront).rotated(angle);
        lArmSprite.setScale(1, 1);
        rArmSprite.setScale(1, 1);
    }
    else {
        lShoulderPos = pos + (vec(64, -64) + imageData->armFront.flipx()).rotated(angle);
        rShoulderPos = pos + (vec(64, -64) + imageData->armBack.flipx()).rotated(angle);
        lArmSprite.setScale(1, -1);
        rArmSprite.setScale(1, -1);
    }
    lArmSprite.setPosition(lHandPos.x, lHandPos.y);
    rArmSprite.setPosition(rHandPos.x, rHandPos.y);
    lArmSprite.move(posInterp.x - pos.x, posInterp.y - pos.y);
    rArmSprite.move(posInterp.x - pos.x, posInterp.y - pos.y);
    //lItemSprite.move(posInterp.x - pos.x, posInterp.y - pos.y);
    //rItemSprite.move(posInterp.x - pos.x, posInterp.y - pos.y);
    //mainItemSprite.move(posInterp.x - pos.x, posInterp.y - pos.y);

    float lHandDist = (lHandPos - lShoulderPos).getLength();
    float rHandDist = (rHandPos - rShoulderPos).getLength();
    float lHandAngle = (lHandPos - lShoulderPos).getAngle();
    float rHandAngle = (rHandPos - rShoulderPos).getAngle();
    lArmSprite.setRotation(lHandAngle);
    rArmSprite.setRotation(rHandAngle);
    int lIndex = 2;
    int rIndex = 2;
    while (lIndex < lHandDist - 0 && lIndex < 62) lIndex += 2;
    while (rIndex < rHandDist - 0 && rIndex < 62) rIndex += 2;
    lArmData = arms[int(lIndex)];
    rArmData = arms[int(rIndex)];
    lArmSprite.setTexture(lArmData->texture, true);
    rArmSprite.setTexture(rArmData->texture, true);
    lArmSprite.setOrigin(lArmData->hand.x, lArmData->hand.y);
    rArmSprite.setOrigin(rArmData->hand.x, rArmData->hand.y);

    if (dir) {
        window->draw(lArmSprite,shader);
        if (lItem) {
            lItemSprite.setScale(1, 1);
            window->draw(lItemSprite);
        }
        else if (mainItem) {
            mainItemSprite.setScale(1, 1);
            window->draw(mainItemSprite);
        }
    }
    else {
        window->draw(rArmSprite,shader);
        if (rItem) {
            rItemSprite.setScale(-1, 1);
            window->draw(rItemSprite);
        }
        else if (mainItem) {
            mainItemSprite.setScale(-1, 1);
            window->draw(mainItemSprite);
        }
    }
    sprite.setRotation(angle);



    

}


void Player::renderFront()
{
    // body
    window->draw(sprite,shader);




    // front
    if (dir) {
        if (rItem) {
            rItemSprite.setScale(1, 1);
            window->draw(rItemSprite);
        }
        else if (!leftHanded && mainItem) {
            mainItemSprite.setScale(1, 1);
            window->draw(mainItemSprite);
        }
        window->draw(rArmSprite,shader);
    }
    else {
        if (lItem) {
            lItemSprite.setScale(-1, 1);
            window->draw(lItemSprite);
        }
        else if (leftHanded && mainItem) {
            mainItemSprite.setScale(-1, 1);
            window->draw(mainItemSprite);
        }
        window->draw(lArmSprite,shader);
    }
}


void Player::setLeftItem(Weapon* item)
{
    mainItem = nullptr;
    lItem = item;
    // could be NULL
    if (item) {
        if (item->isTwoHanded) {
            mainItemSprite.setTexture(item->texture, true);
            mainItemSprite.setOrigin(item->origin.x, item->origin.y);
            mainItem = item;
            lItem = nullptr;
            rItem = nullptr;
        }
        else {
            lItemSprite.setTexture(item->texture, true);
            lItemSprite.setOrigin(item->grip1.x, item->grip1.y);
        }
    }
    else lItemSprite.setTexture(Texture(), true);
}

void Player::setRightItem(Weapon* item)
{
    mainItem = nullptr;
    rItem = item;
    if (item) {
        if (item->isTwoHanded) {
            mainItemSprite.setTexture(item->texture, true);
            mainItemSprite.setOrigin(item->origin.x, item->origin.y);
            mainItem = item;
            lItem = nullptr;
            rItem = nullptr;
        }
        else {
            rItemSprite.setTexture(item->texture, true);
            rItemSprite.setOrigin(item->grip1.x, item->grip1.y);
        }
    }
    else rItemSprite.setTexture(Texture(), true);
}

void Player::hurt(float damage) {
    if (invincibilitySteps == 0) {
        playSound(randomChoice(hits), pos, 15, randnum(.9, 1.1));
        health -= damage;
        invincibilitySteps = 30;
        regenerate = 240;
    }

}