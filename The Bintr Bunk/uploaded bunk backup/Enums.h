#pragma once

enum TerrainTypes {
    air,
    stone,
    dirt
};

enum AnimationStates {
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
};