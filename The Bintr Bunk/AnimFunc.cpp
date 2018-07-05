#include "AnimFunc.h"
#include "BintrsTools.h"
#include "Player.h"
#define say(x) std::cout<<x<<'\n'
#define randomChoice(vector) vector[randint(0,vector.size()-1)]

extern Player* p;

void animFunc() {
    // all anim states
    /*

    animStanding,
    animWalking,
    animSprinting,
    animCrouchWalking,
    animBackwardsWalking,
    animBackwardsCrouchWalking,
    animBackwardsSprinting,
    animFalling,
    animRising, //(smooth transition from falling)
    animJumping, //(sharp change to jump frame)
    animJumpBoosting, //like standing from a crouch but fast
    animCrouching,

    s - standing
    W - walking
    f - falling
    w - crouch walking
    B - backwards walking
    b - backwards crouch walking
    r - rising (smooth transition from falling)
    j - jumping (sharp change to jump frame)
    J - 'jump boosting', like standing from a crouch but fast
    c - crouching
    S - sprinting
    */
    for (auto p : players) {// && p->alive) {
        int s = p->animState;
        int s_last = p->animStateLast;
        if (s == animStanding              ) {
            p->fallFrame = 0;
            if (p->standToCrouchFrame == 0)
                p->imageData = &p->stand;
            else {
                p->standToCrouchFrame--;
                p->imageData = &p->standToCrouch[p->standToCrouchFrame];
            }
        }
        if (s == animCrouching             ) {
            p->fallFrame = 0;
            if (p->standToCrouchFrame == p->standToCrouch.size())
                p->imageData = &p->crouch;
            else {
                p->imageData = &p->standToCrouch[p->standToCrouchFrame];
                p->standToCrouchFrame++;
            }
        }
        if (s == animJumping               ) {
            p->fallFrame = 0;
            p->standToCrouchFrame = 0;
            p->imageData = &p->jump;
        }
        if (s == animJumpBoosting          ) {
            p->standToCrouchFrame -= 2;
            if (p->standToCrouchFrame < 0) p->standToCrouchFrame = 0;
            p->imageData = &p->standToCrouch[p->standToCrouchFrame];
        }
        if (s == animRising                ) {
            if (p->fallFrame == 0) {
                p->imageData = &p->jump;
            }
            else {
                p->fallFrame--;
                p->imageData = &p->jumpToFall[p->fallFrame];
            }
        }
        if (s == animFalling               ) {
            p->standToCrouchFrame = 0;
            if (p->fallFrame == p->jumpToFall.size()) {
                p->imageData = &p->fall;
            }
            else {
                p->imageData = &p->jumpToFall[p->fallFrame];
                p->fallFrame++;
            }
        }
        if (s == animWalking               ) {
            p->fallFrame = 0;
            if (p->standToCrouchFrame == 0) {
                if (s_last != animSprinting && 
                    s_last != animCrouchWalking && 
                    s_last != animWalking) p->runFrame = 23;
                if (s_last == animFalling) p->runFrame = 6;
                else if (p->runFrame == 6 ||
                         p->runFrame == 19)
                    playSound(randomChoice(footsteps), p->pos, 20);
                p->imageData = &p->run[p->runFrame];
                p->runFrame++;
                if (p->runFrame >= p->run.size()) p->runFrame = 0;
            }
            else {
                p->standToCrouchFrame--;
                p->imageData = &p->standToCrouch[p->standToCrouchFrame];
            }

        }
        if (s == animCrouchWalking         ) {
            p->fallFrame = 0;
            if (s_last != animCrouchWalking) p->crouchWalkFrame = 23;
            if (p->crouchWalkFrame == 29 ||
                p->crouchWalkFrame == 10)
                playSound(randomChoice(footsteps), p->pos, 2);
            if (p->standToCrouchFrame == p->standToCrouch.size()) {
                p->imageData = &p->crouchWalk[p->crouchWalkFrame];
                p->crouchWalkFrame++;
                if (p->crouchWalkFrame >= p->crouchWalk.size()) p->crouchWalkFrame = 0;
            }
            else {
                p->imageData = &p->standToCrouch[p->standToCrouchFrame];
                p->standToCrouchFrame++;
            }

        }
        if (s == animBackwardsCrouchWalking) {
            p->fallFrame = 0;
            if (s_last != animBackwardsCrouchWalking) p->crouchWalkFrame = 23;
            if (p->crouchWalkFrame == 29 ||
                p->crouchWalkFrame == 10)
                playSound(randomChoice(footsteps), p->pos, 2);
            if (p->standToCrouchFrame == p->standToCrouch.size()) {
                p->imageData = &p->crouchWalk[p->crouchWalkFrame];
                p->crouchWalkFrame--;
                if (p->crouchWalkFrame < 0) p->crouchWalkFrame = p->crouchWalk.size() - 1;
            }
            else {
                p->imageData = &p->standToCrouch[p->standToCrouchFrame];
                p->standToCrouchFrame++;
            }
        }
        if (s == animBackwardsWalking      ) {
            p->fallFrame = 0;
            if (p->standToCrouchFrame == 0) {
                if (s_last != animSprinting && 
                    s_last != animCrouchWalking && 
                    s_last != animBackwardsWalking &&
                    s_last != animBackwardsSprinting) p->runFrame = 23;
                if (s_last == animFalling) p->runFrame = 6;
                else if (p->runFrame == 6 ||
                         p->runFrame == 19)
                    playSound(randomChoice(footsteps), p->pos, 20);
                p->imageData = &p->run[p->runFrame];
                p->runFrame--;
                if (p->runFrame < 0) p->runFrame = p->run.size() - 1;
            }
            else {
                p->standToCrouchFrame--;
                p->imageData = &p->standToCrouch[p->standToCrouchFrame];
            }
        }
        if (s == animSprinting) {
            p->fallFrame = 0;
            if (p->standToCrouchFrame == 0) {
                if (s_last != animSprinting && 
                    s_last != animCrouchWalking && 
                    s_last != animWalking) p->runFrame = 23;
                if (s_last == animFalling) p->runFrame = 6;
                else if (p->runFrame == 6 ||
                         p->runFrame == 7 ||
                         p->runFrame == 19 ||
                         p->runFrame == 20)
                    playSound(randomChoice(footsteps), p->pos, 40);
                p->imageData = &p->run[p->runFrame];
                p->runFrame += 2;
                if (p->runFrame >= p->run.size()) p->runFrame -= (p->run.size() - 1);
            }
            else {
                p->standToCrouchFrame -= 2;
                if (p->standToCrouchFrame < 0) p->standToCrouchFrame = 0;
                p->imageData = &p->standToCrouch[p->standToCrouchFrame];
            }

        }
        if (s == animBackwardsSprinting) {
            p->fallFrame = 0;
            if (p->standToCrouchFrame == 0) {
                if (s_last != animSprinting &&
                    s_last != animCrouchWalking &&
                    s_last != animBackwardsWalking &&
                    s_last != animBackwardsSprinting) p->runFrame = 23;
                if (s_last == animFalling) p->runFrame = 6;
                else if (p->runFrame == 6 ||
                         p->runFrame == 7 ||
                         p->runFrame == 19 ||
                         p->runFrame == 20)
                    playSound(randomChoice(footsteps), p->pos, 20);
                p->imageData = &p->run[p->runFrame];
                p->runFrame -= 2;
                if (p->runFrame < 0) p->runFrame += (p->run.size() - 1);
            }
            else {
                p->standToCrouchFrame-=2;
                if (p->standToCrouchFrame < 0) p->standToCrouchFrame = 0;
                p->imageData = &p->standToCrouch[p->standToCrouchFrame];
            }
        }
        p->body.sprite.setTexture(p->imageData->texture);
        p->animStateLast = p->animState;
        //say(p->animState);
    }
}
