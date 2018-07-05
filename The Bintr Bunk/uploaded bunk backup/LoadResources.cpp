#include "loadResources.h"
#include "Item.h"
#define say(x) std::cout<<x<<'\n'

/* Note: 
Declaring texture objects outside main() is BAD
I don't really know why.
Declare texture pointers instead
*/
extern vector<Item*> hotbarItems;
sf::Image imgPlayerStand;
sf::Image flyingBomb;

Texture* smokeCloud;
Texture* blob;
Weapon* grenade1;
Weapon* grenade2;
Weapon* ak;
Weapon* ar10;
Weapon* Delisle;
Weapon* fal;
Weapon* famas;
Weapon* g3a1;
Weapon* mac10;
Weapon* mp5;
Weapon* pistol;
Weapon* revolver;
Weapon* rifle;
Weapon* rocket_launcher;
Weapon* shotgun;
Weapon* sword;
Weapon* tec9;
Weapon* teslagun;
Weapon* vintorez;
Hat* santa_beard               ;
Hat* mouth_pickle              ;
Hat* apple                     ;
Hat* arrow                     ;
Hat* angry_yellow              ;
Hat* angry_pig                 ;
Hat* angry_pig2                ;
Hat* egg                       ;
Hat* sunflower                 ;
Hat* SP_eric                   ;
Hat* SP_eyes                   ;
Hat* SP_kyle                   ;
Hat* SP_stan                   ;
Hat* SP_frown                  ;
Hat* skimask_black             ;
Hat* skimask_green             ;
Hat* deal_with_it_sunglasses   ;
Hat* santa_hat                 ;
Hat* badpresident              ;
Hat* mason_special             ;
Hat* backwards_cap             ;
Hat* baseball_cap              ;
Hat* cigar                     ;
Hat* cigarette                 ;
Hat* pipe                      ;
Hat* crazy_eyes                ;
Hat* umbrellahat               ;
Hat* propellercap              ;
Hat* hatchethat                ;
Hat* animeface                 ;
Hat* axehat                    ;
Hat* headband                  ;
Hat* jelboy                    ;
Hat* wizard_hat                ;
Hat* turd                      ;
Hat* plumbus                   ;
Hat* tophat                    ;
Hat* angry_hat                 ;
Hat* toilet_plunger            ;
Hat* bintrs_hat                ;
Hat* banana_peel               ;
vector<PlayerImage*> playerCrouchWalk;
vector<PlayerImage*> playerJumpToFall;
vector<PlayerImage*> playerRun;
vector<PlayerImage*> playerStandToCrouch;
PlayerImage* playerStand;
PlayerImage* playerCrouch;
PlayerImage* playerJump;
PlayerImage* playerFall;
map<int, ArmImage*> playerArm;
vector<SoundBuffer*> grabSounds;
vector<SoundBuffer*> footsteps;
vector<SoundBuffer*> jumpSounds;
vector<SoundBuffer*> doubleJumpSounds;
vector<SoundBuffer*> digSounds;
SoundBuffer* grenadePin;
SoundBuffer* grenadeTick;
SoundBuffer* triggerPull;
SoundBuffer* triggerRelease;
SoundBuffer* throwItem;
SoundBuffer* ding;
SoundBuffer* fart;
SoundBuffer* explosion;
SoundBuffer* shoot_pistol;
SoundBuffer* reload_pistol;
SoundBuffer* shoot_smg;
SoundBuffer* reload_smg;
SoundBuffer* shoot_shotgun;
SoundBuffer* reload_shotgun;
SoundBuffer* shoot_ak47;
SoundBuffer* reload_ak47;
SoundBuffer* shoot_fal;
SoundBuffer* reload_fal;
SoundBuffer* charge_tesla;
SoundBuffer* shoot_tesla;
SoundBuffer* shoot_rifle;
SoundBuffer* reload_rifle;
SoundBuffer* shoot_vintorez;
SoundBuffer* reload_vintorez;
SoundBuffer* GSH;
SoundBuffer* headJump;
SoundBuffer* planeLoop;
vector<SoundBuffer*> hits;


SoundBuffer* slideSound;
Sound* slideLoop;

extern sf::RenderWindow* window;

#include <iostream>
#include "PhysicsStuff.h"

// for flashing white
const std::string fragmentFlashWhite = \
"uniform sampler2D texture;                              \n" \
"                                                        \n" \
"void main()                                             \n" \
"{                                                       \n" \
"    // lookup the pixel in the texture                  \n" \
"    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy); \n" \
"                                                        \n" \
"    // multiply it by the color                         \n" \
"    if (pixel != vec4(0,0,0,1))                         \n" \
"       gl_FragColor = gl_Color * pixel + vec4(.2,-.4,-.4,0);                    \n" \
"}                                                       \n";
// was .7,.7,.7,0
// nmow its red
sf::Shader shaderFlashWhite;
extern float physRate;
void loadResources()
{

    shaderFlashWhite.loadFromMemory(fragmentFlashWhite, sf::Shader::Fragment);

    smokeCloud = new Texture;
    blob = new Texture;
    smokeCloud->loadFromFile("resources/pictures/items/smoke_cloud.png");
    blob->loadFromFile("resources/pictures/blob.png");
    slideLoop = new Sound();
    slideSound = new SoundBuffer();slideSound->loadFromFile("resources/sounds/sliding.wav");
    slideLoop->setPitch(physRate / 60);
    slideLoop->setMinDistance(2000);
    slideLoop->setAttenuation(60);
    //slideLoop->setAttenuation();
    slideLoop->setBuffer(*slideSound);
    slideLoop->setLoop(true);
    slideLoop->setVolume(0);
    slideLoop->play();
    say("Window Active: " << window->isOpen());
    sf::SoundBuffer* sb;
    hits.emplace_back       (sb = new sf::SoundBuffer()); sb->loadFromFile("resources/sounds/hit1.wav");
    hits.emplace_back       (sb = new sf::SoundBuffer()); sb->loadFromFile("resources/sounds/hit2.wav");
    hits.emplace_back       (sb = new sf::SoundBuffer()); sb->loadFromFile("resources/sounds/hit3.wav");
    hits.emplace_back       (sb = new sf::SoundBuffer()); sb->loadFromFile("resources/sounds/hit4.wav");
    footsteps.emplace_back       (sb = new sf::SoundBuffer()); sb->loadFromFile("resources/sounds/step1.wav");
    footsteps.emplace_back       (sb = new sf::SoundBuffer()); sb->loadFromFile("resources/sounds/step2.wav");
    footsteps.emplace_back       (sb = new sf::SoundBuffer()); sb->loadFromFile("resources/sounds/step3.wav");
    footsteps.emplace_back       (sb = new sf::SoundBuffer()); sb->loadFromFile("resources/sounds/step4.wav");
    footsteps.emplace_back       (sb = new sf::SoundBuffer()); sb->loadFromFile("resources/sounds/step5.wav");
    jumpSounds.emplace_back      (sb = new sf::SoundBuffer()); sb->loadFromFile("resources/sounds/jump1.wav");
    jumpSounds.emplace_back      (sb = new sf::SoundBuffer()); sb->loadFromFile("resources/sounds/jump2.wav");
    jumpSounds.emplace_back      (sb = new sf::SoundBuffer()); sb->loadFromFile("resources/sounds/jump3.wav");
    doubleJumpSounds.emplace_back(sb = new sf::SoundBuffer()); sb->loadFromFile("resources/sounds/doublejump1.wav");
    doubleJumpSounds.emplace_back(sb = new sf::SoundBuffer()); sb->loadFromFile("resources/sounds/doublejump2.wav");
    doubleJumpSounds.emplace_back(sb = new sf::SoundBuffer()); sb->loadFromFile("resources/sounds/doublejump3.wav");
    doubleJumpSounds.emplace_back(sb = new sf::SoundBuffer()); sb->loadFromFile("resources/sounds/doublejump4.wav");
    doubleJumpSounds.emplace_back(sb = new sf::SoundBuffer()); sb->loadFromFile("resources/sounds/doublejump5.wav");
    grabSounds.emplace_back(sb = new SoundBuffer()); sb->loadFromFile("resources/sounds/grab1.wav");
    grabSounds.emplace_back(sb = new SoundBuffer()); sb->loadFromFile("resources/sounds/grab2.wav");
    grabSounds.emplace_back(sb = new SoundBuffer()); sb->loadFromFile("resources/sounds/grab3.wav");
    grabSounds.emplace_back(sb = new SoundBuffer()); sb->loadFromFile("resources/sounds/grab4.wav");
    grabSounds.emplace_back(sb = new SoundBuffer()); sb->loadFromFile("resources/sounds/grab5.wav");
    grabSounds.emplace_back(sb = new SoundBuffer()); sb->loadFromFile("resources/sounds/grab6.wav");
    grabSounds.emplace_back(sb = new SoundBuffer()); sb->loadFromFile("resources/sounds/grab7.wav");
    grabSounds.emplace_back(sb = new SoundBuffer()); sb->loadFromFile("resources/sounds/grab8.wav");
    grabSounds.emplace_back(sb = new SoundBuffer()); sb->loadFromFile("resources/sounds/grab9.wav");
    grabSounds.emplace_back(sb = new SoundBuffer()); sb->loadFromFile("resources/sounds/grab10.wav");
    grabSounds.emplace_back(sb = new SoundBuffer()); sb->loadFromFile("resources/sounds/grab11.wav");

    digSounds.emplace_back(sb = new SoundBuffer()); sb->loadFromFile("resources/sounds/dig1.wav");
    digSounds.emplace_back(sb = new SoundBuffer()); sb->loadFromFile("resources/sounds/dig2.wav");
    digSounds.emplace_back(sb = new SoundBuffer()); sb->loadFromFile("resources/sounds/dig3.wav");
    digSounds.emplace_back(sb = new SoundBuffer()); sb->loadFromFile("resources/sounds/dig4.wav");

    planeLoop       = new SoundBuffer; planeLoop->loadFromFile("resources/sounds/plane_loop.wav");
    headJump        = new SoundBuffer; headJump       ->loadFromFile("resources/sounds/headjump.wav" );
    grenadePin      = new SoundBuffer; grenadePin     ->loadFromFile("resources/sounds/grenade_pin.wav" );
    grenadeTick     = new SoundBuffer; grenadeTick    ->loadFromFile("resources/sounds/grenade_tick.wav" );
    triggerPull     = new SoundBuffer; triggerPull    ->loadFromFile("resources/sounds/trigger_pull.wav" );
    triggerRelease  = new SoundBuffer; triggerRelease ->loadFromFile("resources/sounds/trigger_release.wav" );
    throwItem       = new SoundBuffer; throwItem      ->loadFromFile("resources/sounds/throw.wav" );
    ding            = new SoundBuffer; ding           ->loadFromFile("resources/sounds/ding.wav" );
    fart            = new SoundBuffer; fart           ->loadFromFile("resources/sounds/fart.wav" );
    explosion       = new SoundBuffer; explosion      ->loadFromFile("resources/sounds/explosion.wav" );
    shoot_pistol    = new SoundBuffer; shoot_pistol   ->loadFromFile("resources/sounds/shoot_pistol.wav" );
    reload_pistol   = new SoundBuffer; reload_pistol  ->loadFromFile("resources/sounds/reload_pistol.wav" );
    shoot_smg       = new SoundBuffer; shoot_smg      ->loadFromFile("resources/sounds/shoot_smg.wav" );
    reload_smg      = new SoundBuffer; reload_smg     ->loadFromFile("resources/sounds/reload_smg.wav" );
    shoot_shotgun   = new SoundBuffer; shoot_shotgun  ->loadFromFile("resources/sounds/shoot_shotgun.wav" );
    reload_shotgun  = new SoundBuffer; reload_shotgun ->loadFromFile("resources/sounds/reload_shotgun.wav" );
    shoot_ak47      = new SoundBuffer; shoot_ak47     ->loadFromFile("resources/sounds/shoot_ak47.wav" );
    reload_ak47     = new SoundBuffer; reload_ak47    ->loadFromFile("resources/sounds/reload_ak47.wav" );
    shoot_fal       = new SoundBuffer; shoot_fal      ->loadFromFile("resources/sounds/shoot_fal.wav" );
    reload_fal      = new SoundBuffer; reload_fal     ->loadFromFile("resources/sounds/reload_fal.wav" );
    shoot_rifle     = new SoundBuffer; shoot_rifle    ->loadFromFile("resources/sounds/shoot_rifle.wav" );
    reload_rifle    = new SoundBuffer; reload_rifle   ->loadFromFile("resources/sounds/reload_rifle.wav" );
    shoot_vintorez  = new SoundBuffer; shoot_vintorez ->loadFromFile("resources/sounds/shoot_vintorez.wav" );
    reload_vintorez = new SoundBuffer; reload_vintorez->loadFromFile("resources/sounds/reload_vintorez.wav" );
    GSH             = new SoundBuffer; GSH            ->loadFromFile("resources/sounds/GSH.wav" );
    charge_tesla    = new SoundBuffer; charge_tesla   ->loadFromFile("resources/sounds/charge_tesla.wav" );
    shoot_tesla     = new SoundBuffer; shoot_tesla    ->loadFromFile("resources/sounds/shoot_tesla.wav" );

    imgPlayerStand.loadFromFile("resources/pictures/player/standing_side.png");
    flyingBomb.loadFromFile("resources/pictures/flying_bomb.png");
    //smokeCloud = new Texture;
    //blob = new Texture;
    //smokeCloud->loadFromFile("resources/pictures/items/smoke_cloud.png");
    //blob->loadFromFile("resources/pictures/blob.png");
    //// animations
    //rifleShoot1.linkTo(&rifleShoot2);
    //rifleShoot2.linkTo(&rifleShoot3);
    //
    //// sounds 
    ////genContext = new Context();
    //{
    //    slideLoop = new Sound();
    //    slide_sound = new SoundBuffer();
    //    slide_sound->loadFromFile("resources/sounds/sliding.wav");
    //    slideLoop->setBuffer(*slide_sound);
    //    slideLoop->setLoop(true);
    //    slideLoop->setVolume(0);
    //    slideLoop->play();
    //
    
    //    footsteps.emplace_back(new SoundBuffer()); footsteps[footsteps.size() - 1]->loadFromFile("resources/sounds/step1.wav");
    //    footsteps.emplace_back(new SoundBuffer()); footsteps[footsteps.size() - 1]->loadFromFile("resources/sounds/step2.wav");
    //    footsteps.emplace_back(new SoundBuffer()); footsteps[footsteps.size() - 1]->loadFromFile("resources/sounds/step3.wav");
    //    footsteps.emplace_back(new SoundBuffer()); footsteps[footsteps.size() - 1]->loadFromFile("resources/sounds/step4.wav");
    //    footsteps.emplace_back(new SoundBuffer()); footsteps[footsteps.size() - 1]->loadFromFile("resources/sounds/step5.wav");
    //    jumpSounds.emplace_back(new SoundBuffer()); jumpSounds[jumpSounds.size() - 1]->loadFromFile("resources/sounds/jump1.wav");
    //    jumpSounds.emplace_back(new SoundBuffer()); jumpSounds[jumpSounds.size() - 1]->loadFromFile("resources/sounds/jump2.wav");
    //    jumpSounds.emplace_back(new SoundBuffer()); jumpSounds[jumpSounds.size() - 1]->loadFromFile("resources/sounds/jump3.wav");
    //    doubleJumpSounds.emplace_back(new SoundBuffer()); doubleJumpSounds[doubleJumpSounds.size() - 1]->loadFromFile("resources/sounds/doublejump1.wav");
    //    doubleJumpSounds.emplace_back(new SoundBuffer()); doubleJumpSounds[doubleJumpSounds.size() - 1]->loadFromFile("resources/sounds/doublejump2.wav");
    //    doubleJumpSounds.emplace_back(new SoundBuffer()); doubleJumpSounds[doubleJumpSounds.size() - 1]->loadFromFile("resources/sounds/doublejump3.wav");
    //    doubleJumpSounds.emplace_back(new SoundBuffer()); doubleJumpSounds[doubleJumpSounds.size() - 1]->loadFromFile("resources/sounds/doublejump4.wav");
    //    doubleJumpSounds.emplace_back(new SoundBuffer()); doubleJumpSounds[doubleJumpSounds.size() - 1]->loadFromFile("resources/sounds/doublejump5.wav");
    //    digSounds.emplace_back(new SoundBuffer()); digSounds[digSounds.size() - 1]->loadFromFile("resources/sounds/dig1.wav");
    //    digSounds.emplace_back(new SoundBuffer()); digSounds[digSounds.size() - 1]->loadFromFile("resources/sounds/dig2.wav");
    //    digSounds.emplace_back(new SoundBuffer()); digSounds[digSounds.size() - 1]->loadFromFile("resources/sounds/dig3.wav");
    //    digSounds.emplace_back(new SoundBuffer()); digSounds[digSounds.size() - 1]->loadFromFile("resources/sounds/dig4.wav");
    //
    //    grenadePin     = new SoundBuffer; grenadePin    ->loadFromFile("resources/sounds/grenade_pin.wav");
    //    grenadeTick    = new SoundBuffer; grenadeTick   ->loadFromFile("resources/sounds/grenade_tick.wav");
    //    triggerPull    = new SoundBuffer; triggerPull   ->loadFromFile("resources/sounds/trigger_pull.wav");
    //    triggerRelease = new SoundBuffer; triggerRelease->loadFromFile("resources/sounds/trigger_release.wav");
    //    throwItem      = new SoundBuffer; throwItem     ->loadFromFile("resources/sounds/throw.wav");
    //    ding = new SoundBuffer; ding->loadFromFile("resources/sounds/ding.wav");
    //    fart = new SoundBuffer; fart->loadFromFile("resources/sounds/fart.wav");
    //    explosion = new SoundBuffer; explosion->loadFromFile("resources/sounds/explosion.wav");
    //    shoot_pistol = new SoundBuffer; shoot_pistol->loadFromFile("resources/sounds/shoot_pistol.wav");
    //    reload_pistol = new SoundBuffer; reload_pistol->loadFromFile("resources/sounds/reload_pistol.wav");
    //    shoot_smg = new SoundBuffer; shoot_smg->loadFromFile("resources/sounds/shoot_smg.wav");
    //    reload_smg = new SoundBuffer; reload_smg->loadFromFile("resources/sounds/reload_smg.wav");
    //    shoot_shotgun = new SoundBuffer; shoot_shotgun->loadFromFile("resources/sounds/shoot_shotgun.wav");
    //    reload_shotgun = new SoundBuffer; reload_shotgun->loadFromFile("resources/sounds/reload_shotgun.wav");
    //    shoot_ak47 = new SoundBuffer; shoot_ak47->loadFromFile("resources/sounds/shoot_ak47.wav");
    //    reload_ak47 = new SoundBuffer; reload_ak47->loadFromFile("resources/sounds/reload_ak47.wav");
    //    shoot_fal = new SoundBuffer; shoot_fal->loadFromFile("resources/sounds/shoot_fal.wav");
    //    reload_fal = new SoundBuffer; reload_fal->loadFromFile("resources/sounds/reload_fal.wav");
    //    shoot_rifle = new SoundBuffer; shoot_rifle->loadFromFile("resources/sounds/shoot_rifle.wav");
    //    reload_rifle = new SoundBuffer; reload_rifle->loadFromFile("resources/sounds/reload_rifle.wav");
    //    shoot_vintorez = new SoundBuffer; shoot_vintorez->loadFromFile("resources/sounds/shoot_vintorez.wav");
    //    reload_vintorez = new SoundBuffer; reload_vintorez->loadFromFile("resources/sounds/reload_vintorez.wav");
    //    GSH = new SoundBuffer; GSH->loadFromFile("resources/sounds/GSH.wav");
    //    charge_tesla = new SoundBuffer; charge_tesla->loadFromFile("resources/sounds/charge_tesla.wav");
    //    shoot_tesla = new SoundBuffer; shoot_tesla->loadFromFile("resources/sounds/shoot_tesla.wav");
    //}
    //// a lot of image stuff
    //{
    // item images
    {
        int num = -7;
        ak              = new Weapon("resources/pictures/items/ak.png"             ,vec(5 ,4 ),vec(num+6,7));//,vec(5,4)  ,vec(6,7));
        //ak->shootFrame = &rifleShoot1;
        //ak->reloadFrame = &akReload1;
        ak->shootSound = shoot_ak47;
        ak->reloadSound = reload_ak47;
        ak->magSize = 3000;
        ak->magBullets = 3000;
        ak->shootSteps = 60 / 10; // 10 per second
        ak->reloadSteps = 40;
        ak->projectile = bullet;
        ak->projectilesPerShot = 1;
        ak->inaccuracy = 2.5;
        grenade1            = new Weapon("resources/pictures/items/grenade.png" );
        grenade2            = new Weapon("resources/pictures/items/grenade2.png");
        grenade2->shootSound = throwItem;
        grenade2->reloadSound = reload_fal;
        grenade2->magSize = 10000000;
        grenade2->magBullets = 10000000;
        grenade2->shootSteps = 60 / 5; // 10 per second
        grenade2->reloadSteps = 60;
        grenade2->projectile = grenade;
        grenade2->projectilesPerShot = 1;
        grenade2->hasTrigger = false;
        grenade2->hasPin = true;
        grenade2->isTwoHanded = false;
        grenade2->automatic = false;
        grenade2->shootOnPress = false;
        grenade2->shootOnRelease = true;
        ar10            = new Weapon("resources/pictures/items/ar10.png"           ,vec(-3,11),vec(num+1,-2));//,vec(-3,11),vec());
        Delisle         = new Weapon("resources/pictures/items/Delisle.png"        ,vec(19,2 ),vec(num+4,12));//,vec(15,5) ,vec());
        fal             = new Weapon("resources/pictures/items/fal.png"            ,vec(21,7 ),vec(num+14,18));//,vec(17,10),vec());
        fal->shootSound = shoot_fal;
        fal->reloadSound = reload_fal;
        fal->magSize = 3000;
        fal->magBullets = 3000;
        fal->shootSteps = 60 / 20; // 10 per second
        fal->reloadSteps = 30;
        fal->projectile = bullet;
        fal->projectilesPerShot = 14;
        fal->inaccuracy = 4; 
        famas           = new Weapon("resources/pictures/items/famas.png"          ,vec(6 ,7 ),vec(num+11,4));//,vec(9,2)  ,vec());
        g3a1            = new Weapon("resources/pictures/items/g3a1.png"           ,vec(12,7 ),vec(num+16,4));//,vec(3,15) ,vec());
        mac10           = new Weapon("resources/pictures/items/mac10.png"          );//);          num+
        mp5             = new Weapon("resources/pictures/items/mp5.png"            );//);          num+
        pistol          = new Weapon("resources/pictures/items/pistol.png"         );//);          num+
        revolver        = new Weapon("resources/pictures/items/revolver.png"       );//);          num+
        rifle           = new Weapon("resources/pictures/items/rifle.png"          ,vec(-1,12),vec(num+7,0));//,vec(-3,11), vec(-1,0));
        rocket_launcher = new Weapon("resources/pictures/items/rocket_launcher.png",vec(18,18),vec(num+2,-3));//,vec(8,10) ,vec());
        rocket_launcher->shootSound = shoot_pistol;
        rocket_launcher->reloadSound = reload_fal;
        rocket_launcher->magSize = 10000000;
        rocket_launcher->magBullets = 10000000;
        rocket_launcher->shootSteps = 60 / 5; // 10 per second
        rocket_launcher->reloadSteps = 60;
        rocket_launcher->projectile = grenade;
        rocket_launcher->projectilesPerShot = 1;
        rocket_launcher->inaccuracy = 4;
        shotgun         = new Weapon("resources/pictures/items/shotgun.png"        ,vec(11,3 ),vec(num+20,7));//,vec(12,5) ,vec(8,10));
        shotgun->shootSound = shoot_shotgun;
        shotgun->reloadSound = reload_shotgun;
        shotgun->magSize = 800;
        shotgun->magBullets = 800;
        shotgun->shootSteps = 60 / 2; // 10 per second
        shotgun->reloadSteps = 45;
        shotgun->projectile = bullet;
        shotgun->projectilesPerShot = 19;
        shotgun->inaccuracy = 5;
        sword           = new Weapon("resources/pictures/items/sword.png"          );//);          num+
        tec9            = new Weapon("resources/pictures/items/tec9.png"           );//);          num+
        teslagun        = new Weapon("resources/pictures/items/teslagun.png"       ,vec(18,8 ),vec(num+14,18));//,vec(11,8) ,vec(2,6));
        vintorez        = new Weapon("resources/pictures/items/vintorez.png"       ,vec(0 ,10),vec(num+7,2));//,vec(-4,6) ,vec());
    }
    hotbarItems.emplace_back(new Item(new Weapon(*grenade1       )));
    hotbarItems.emplace_back(new Item(new Weapon(*grenade2       )));
    hotbarItems.emplace_back(new Item(new Weapon(*ak             )));
    hotbarItems.emplace_back(new Item(new Weapon(*ar10           )));
    hotbarItems.emplace_back(new Item(new Weapon(*Delisle        )));
    hotbarItems.emplace_back(new Item(new Weapon(*fal            )));
    hotbarItems.emplace_back(new Item(new Weapon(*famas          )));
    hotbarItems.emplace_back(new Item(new Weapon(*g3a1           )));
    hotbarItems.emplace_back(new Item(new Weapon(*rifle          )));
    hotbarItems.emplace_back(new Item(new Weapon(*rocket_launcher)));
    hotbarItems.emplace_back(new Item(new Weapon(*shotgun        )));
    hotbarItems.emplace_back(new Item(new Weapon(*teslagun       )));
    hotbarItems.emplace_back(new Item(new Weapon(*vintorez       )));
    hotbarItems.emplace_back(new Item(new Weapon(*pistol         )));
    hotbarItems.emplace_back(new Item(new Weapon(*tec9           )));
    //
    //
        // player images
        {
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk1.png"    ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk2.png"    ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk3.png"    ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk4.png"    ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk5.png"    ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk6.png"    ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk7.png"    ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk8.png"    ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk9.png"    ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk10.png"   ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk11.png"   ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk12.png"   ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk13.png"   ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk14.png"   ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk15.png"   ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk16.png"   ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk17.png"   ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk18.png"   ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk19.png"   ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk20.png"   ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk21.png"   ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk22.png"   ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk23.png"   ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk24.png"   ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk25.png"   ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk26.png"   ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk27.png"   ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk28.png"   ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk29.png"   ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk30.png"   ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk31.png"   ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk32.png"   ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk33.png"   ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk34.png"   ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk35.png"   ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk36.png"   ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk37.png"   ));
            playerCrouchWalk.emplace_back   (new PlayerImage("resources/pictures/player/crouch_walk38.png"   ));
            playerJumpToFall.emplace_back   (new PlayerImage("resources/pictures/player/jump_to_fall1.png"   ));
            playerJumpToFall.emplace_back   (new PlayerImage("resources/pictures/player/jump_to_fall2.png"   ));
            playerJumpToFall.emplace_back   (new PlayerImage("resources/pictures/player/jump_to_fall3.png"   ));
            playerJumpToFall.emplace_back   (new PlayerImage("resources/pictures/player/jump_to_fall4.png"   ));
            playerJumpToFall.emplace_back   (new PlayerImage("resources/pictures/player/jump_to_fall5.png"   ));
            playerJumpToFall.emplace_back   (new PlayerImage("resources/pictures/player/jump_to_fall6.png"   ));
            playerJumpToFall.emplace_back   (new PlayerImage("resources/pictures/player/jump_to_fall7.png"   ));
            playerRun.emplace_back          (new PlayerImage("resources/pictures/player/run1.png"            ));
            playerRun.emplace_back          (new PlayerImage("resources/pictures/player/run2.png"            ));
            playerRun.emplace_back          (new PlayerImage("resources/pictures/player/run3.png"            ));
            playerRun.emplace_back          (new PlayerImage("resources/pictures/player/run4.png"            ));
            playerRun.emplace_back          (new PlayerImage("resources/pictures/player/run5.png"            ));
            playerRun.emplace_back          (new PlayerImage("resources/pictures/player/run6.png"            ));
            playerRun.emplace_back          (new PlayerImage("resources/pictures/player/run7.png"            ));
            playerRun.emplace_back          (new PlayerImage("resources/pictures/player/run8.png"            ));
            playerRun.emplace_back          (new PlayerImage("resources/pictures/player/run9.png"            ));
            playerRun.emplace_back          (new PlayerImage("resources/pictures/player/run10.png"           ));
            playerRun.emplace_back          (new PlayerImage("resources/pictures/player/run11.png"           ));
            playerRun.emplace_back          (new PlayerImage("resources/pictures/player/run12.png"           ));
            playerRun.emplace_back          (new PlayerImage("resources/pictures/player/run13.png"           ));
            playerRun.emplace_back          (new PlayerImage("resources/pictures/player/run14.png"           ));
            playerRun.emplace_back          (new PlayerImage("resources/pictures/player/run15.png"           ));
            playerRun.emplace_back          (new PlayerImage("resources/pictures/player/run16.png"           ));
            playerRun.emplace_back          (new PlayerImage("resources/pictures/player/run17.png"           ));
            playerRun.emplace_back          (new PlayerImage("resources/pictures/player/run18.png"           ));
            playerRun.emplace_back          (new PlayerImage("resources/pictures/player/run19.png"           ));
            playerRun.emplace_back          (new PlayerImage("resources/pictures/player/run20.png"           ));
            playerRun.emplace_back          (new PlayerImage("resources/pictures/player/run21.png"           ));
            playerRun.emplace_back          (new PlayerImage("resources/pictures/player/run22.png"           ));
            playerRun.emplace_back          (new PlayerImage("resources/pictures/player/run23.png"           ));
            playerRun.emplace_back          (new PlayerImage("resources/pictures/player/run24.png"           ));
            playerRun.emplace_back          (new PlayerImage("resources/pictures/player/run25.png"           ));
            playerRun.emplace_back          (new PlayerImage("resources/pictures/player/run26.png"           ));
            playerStandToCrouch.emplace_back(new PlayerImage("resources/pictures/player/stand_to_crouch1.png"));
            playerStandToCrouch.emplace_back(new PlayerImage("resources/pictures/player/stand_to_crouch2.png"));
            playerStandToCrouch.emplace_back(new PlayerImage("resources/pictures/player/stand_to_crouch3.png"));
            playerStandToCrouch.emplace_back(new PlayerImage("resources/pictures/player/stand_to_crouch4.png"));
            playerStandToCrouch.emplace_back(new PlayerImage("resources/pictures/player/stand_to_crouch5.png"));
            playerStandToCrouch.emplace_back(new PlayerImage("resources/pictures/player/stand_to_crouch6.png"));
            playerStandToCrouch.emplace_back(new PlayerImage("resources/pictures/player/stand_to_crouch7.png"));
            playerStandToCrouch.emplace_back(new PlayerImage("resources/pictures/player/stand_to_crouch8.png"));
            playerStandToCrouch.emplace_back(new PlayerImage("resources/pictures/player/stand_to_crouch9.png"));
    
            playerStand  = new PlayerImage("resources/pictures/player/standing_side.png");
            playerCrouch = new PlayerImage("resources/pictures/player/crouch.png");
            playerJump   = new PlayerImage("resources/pictures/player/jump.png");
            playerFall   = new PlayerImage("resources/pictures/player/fall.png");
    
            for (int num=2;num<=62;num+=2){
                playerArm[num] = new ArmImage("resources/pictures/player/arms/arm_grip"+to_string(num)+".png");
            }
        }
    //}
    //
    //// make our colors
    //sf::Color skinColor = hsvtorgb(randnum(18, 23),
    //                               randnum(.2, .5),
    //                               randnum(.8, 1));
    //sf::Color shirtColor = hsvtorgb(randint(0, 360), randnum(0, 1), randtriangular(0, 1, 1));
    //sf::Color pantsColor = hsvtorgb(randint(0, 360), randnum(0, 1), randtriangular(0, 1, 1));
    

}
