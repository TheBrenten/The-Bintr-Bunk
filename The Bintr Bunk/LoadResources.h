#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Weapon.h"
#include "Hat.h"
#include "BintrImage.h"
using namespace std;
using namespace sf;

extern Texture* smokeCloud;
extern Texture* blob;
extern sf::Image imgPlayerStand;
extern sf::Image flyingBomb;
extern Texture* ak_magazine;
class Weapon;
extern Weapon* grenade1;
extern Weapon* grenade2;
extern Weapon* ak;
extern Weapon* ar10;
extern Weapon* Delisle;
extern Weapon* fal;
extern Weapon* famas;
extern Weapon* g3a1;
extern Weapon* mac10;
extern Weapon* mp5;
extern Weapon* pistol;
extern Weapon* revolver;
extern Weapon* rifle;
extern Weapon* rocket_launcher;
extern Weapon* shotgun;
extern Weapon* sword;
extern Weapon* tec9;
extern Weapon* teslagun;
extern Weapon* vintorez;
extern Hat* santa_beard               ;
extern Hat* mouth_pickle              ;
extern Hat* apple                     ;
extern Hat* arrow                     ;
extern Hat* angry_yellow              ;
extern Hat* angry_pig                 ;
extern Hat* angry_pig2                ;
extern Hat* egg                       ;
extern Hat* sunflower                 ;
extern Hat* SP_eric                   ;
extern Hat* SP_eyes                   ;
extern Hat* SP_kyle                   ;
extern Hat* SP_stan                   ;
extern Hat* SP_frown                  ;
extern Hat* skimask_black             ;
extern Hat* skimask_green             ;
extern Hat* deal_with_it_sunglasses   ;
extern Hat* santa_hat                 ;
extern Hat* badpresident              ;
extern Hat* mason_special             ;
extern Hat* backwards_cap             ;
extern Hat* baseball_cap              ;
extern Hat* cigar                     ;
extern Hat* cigarette                 ;
extern Hat* pipe                      ;
extern Hat* crazy_eyes                ;
extern Hat* umbrellahat               ;
extern Hat* propellercap              ;
extern Hat* hatchethat                ;
extern Hat* animeface                 ;
extern Hat* axehat                    ;
extern Hat* headband                  ;
extern Hat* jelboy                    ;
extern Hat* wizard_hat                ;
extern Hat* turd                      ;
extern Hat* plumbus                   ;
extern Hat* tophat                    ;
extern Hat* angry_hat                 ;
extern Hat* toilet_plunger            ;
extern Hat* bintrs_hat                ;
extern Hat* banana_peel               ;
extern vector<PlayerImage*> playerCrouchWalk;
extern vector<PlayerImage*> playerJumpToFall;
extern vector<PlayerImage*> playerRun;
extern vector<PlayerImage*> playerStandToCrouch;
extern PlayerImage* playerStand;
extern PlayerImage* playerCrouch;
extern PlayerImage* playerJump;
extern PlayerImage* playerFall;
extern map<int, ArmImage*> playerArm;
extern vector<SoundBuffer*> grabSounds;
extern vector<SoundBuffer*> footsteps;
extern vector<SoundBuffer*> jumpSounds;
extern vector<SoundBuffer*> doubleJumpSounds;
extern vector<SoundBuffer*> digSounds;
extern SoundBuffer* grenadePin;
extern SoundBuffer* grenadeTick;
extern SoundBuffer* triggerPull;
extern SoundBuffer* triggerRelease;
extern SoundBuffer* throwItem;
extern SoundBuffer* ding;
extern SoundBuffer* fart;
extern SoundBuffer* explosion;
extern SoundBuffer* shoot_pistol;
extern SoundBuffer* reload_pistol;
extern SoundBuffer* shoot_smg;
extern SoundBuffer* reload_smg;
extern SoundBuffer* shoot_shotgun;
extern SoundBuffer* reload_shotgun;
extern SoundBuffer* shoot_ak47;
extern SoundBuffer* reload_ak47;
extern SoundBuffer* shoot_fal;
extern SoundBuffer* reload_fal;
extern SoundBuffer* charge_tesla;
extern SoundBuffer* shoot_tesla;
extern SoundBuffer* shoot_rifle;
extern SoundBuffer* reload_rifle;
extern SoundBuffer* shoot_vintorez;
extern SoundBuffer* reload_vintorez;
extern SoundBuffer* GSH;
extern SoundBuffer* headJump;
extern SoundBuffer* planeLoop;
extern vector<SoundBuffer*> hits;


extern sf::SoundBuffer* slideSound;
extern Sound* slideLoop;

extern sf::Shader* shaderFlashWhite;

void loadResources();