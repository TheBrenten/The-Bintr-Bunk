#include "MenuFunctions.h"
void updateWindow()
{
    resx = disp_resx;
    resy = disp_resy;
    if (!window) window = new sf::RenderWindow;
    if (fullscreen)
        window->create(sf::VideoMode(resx, resy), "The Bintr Bunk", sf::Style::Fullscreen);
    else
        window->create(sf::VideoMode(resx, resy), "The Bintr Bunk", sf::Style::Titlebar | sf::Style::Close);
    x100 = window->getSize().x * .01;
    y100 = window->getSize().y * .01;
    window->setFramerateLimit(60);
    window->setKeyRepeatEnabled(false);
}



bool paused = false;
bool inMainMenu = false;
float ingameZoomScale = 1;
vector<MenuObject*> menuObjects;
extern int resx;
extern int resy;
extern vec cam;
extern void physFunc();

MenuObject* activeMenuObj;
MenuObject* activeMenuObjLast;
Button* circleButton;

Button* bBack;
Button* bResolution;
Button* bFullscreen;
Button* bOptions;
Button* bApply;
Button* bSingleplayer;

float skinH, skinS, skinV;
float shirtH, shirtS, shirtV;
float pantsH, pantsS, pantsV;

MenuSlider* sHue;
MenuSlider* sSat;
MenuSlider* sVal;

Button* bSkin;
Button* bShirt;
Button* bPants;

void updatePlayerPreview() {
    bSkin->color1 = hsvtorgb(skinH,
                             skinS   * .01,
                             skinV   * .01);
    bSkin->color2 = hsvtorgb(skinH,
                             skinS   * .01,
                             skinV   * .01);
    bShirt->color1 = hsvtorgb(shirtH,
                              shirtS  * .01,
                              shirtV  * .01);
    bShirt->color2 = hsvtorgb(shirtH,
                              shirtS  * .01,
                              shirtV  * .01);
    bPants->color1 = hsvtorgb(pantsH,
                              pantsS  * .01,
                              pantsV  * .01);
    bPants->color2 = hsvtorgb(pantsH,
                              pantsS  * .01,
                              pantsV  * .01);

}

int colorToSet = 0; // skin


void editSkinColor() {
    sHue->linkedValue = &skinH;
    sSat->linkedValue = &skinS;
    sVal->linkedValue = &skinV;
}

void editShirtColor() {
    sHue->linkedValue = &shirtH;
    sSat->linkedValue = &shirtS;
    sVal->linkedValue = &shirtV;
}

void editPantsColor() {
    sHue->linkedValue = &pantsH;
    sSat->linkedValue = &pantsS;
    sVal->linkedValue = &pantsV;
}

void randomizePlayerColors() {
    skinH = randnum(18, 23);
    skinS = randnum(20, 50);
    skinV = randnum(80, 100);
    shirtH = randnum(0, 360);
    shirtS = randnum(0, 100);
    shirtV = randtriangular(0, 100, 100);
    pantsH = randnum(0, 360);
    pantsS = randnum(0, 100);
    pantsV = randtriangular(0, 100, 100);
    updatePlayerPreview();
}

void singleplayerMenu() {

    clearMenu();



    menuObjects.emplace_back(new MenuBox(x100 * 40, y100 * 46,
                                         x100 * 40, y100 * 50, Color(60, 60, 60)));

    Button* bStart = new Button(
        "Start",
        goSingleplayer,
        4, 50, 30, 10,
        hsvtorgb(135, 0.75, 0.6),
        //hsvtorgb(135, 0.75, 0.8),
        hsvtorgb(135, 1, 1));
    bBack = new Button(
        "Back",
        mainMenuFromSingleplayerMenu,
        4, 86, 30, 10);

    sHue = new MenuSlider("Hue", "°",
                          x100 * 55,
                          y100 * 56,
                          x100 * 20,
                          69, &skinH,
                          0, 360, NULL, updatePlayerPreview);
    sSat = new MenuSlider("Saturation", "%",
                          x100 * 55,
                          y100 * 66,
                          x100 * 20,
                          69, &skinS,
                          0.1, 100, NULL, updatePlayerPreview);
    sVal = new MenuSlider("Value", "%",
                          x100 * 55,
                          y100 * 76,
                          x100 * 20,
                          69, &skinV,
                          0, 100, NULL, updatePlayerPreview);
    bSkin = new Button("Skin",
                       editSkinColor,
                       45, 51, 8, 8);
    bShirt = new Button("Shirt",
                        editShirtColor,
                        45, 61, 8, 8);
    bPants = new Button("Pants",
                        editPantsColor,
                        45, 71, 8, 8);
    Button* bRandom = new Button("Randomize All", randomizePlayerColors,
                                 45, 83, 30, 8);
    menuObjects.emplace_back(bRandom);
    menuObjects.emplace_back(bSkin);
    menuObjects.emplace_back(bShirt);
    menuObjects.emplace_back(bPants);
    menuObjects.emplace_back(sHue);
    menuObjects.emplace_back(sSat);
    menuObjects.emplace_back(sVal);

    menuObjects.emplace_back(bStart);
    menuObjects.emplace_back(bBack);
    activeMenuObj = bStart;
    randomizePlayerColors();

    updatePlayerPreview();

}
void clearGameObjects() {

    while (activeObjects.size() > 0) {
        delete activeObjects[0];
    }
}
extern Region* lastCreatedRegion;

#include "mutex"
extern mutex regionThreadLock;

void clearRegions() {
    // wait for creation thread to stop
    regionThreadLock.lock();
    regionThreadLock.unlock();
    regionsToMake.clear();
    if (lastCreatedRegion) {
        delete lastCreatedRegion;
        lastCreatedRegion = nullptr;
    }
    while (regions.size() > 0) {
        delete regions.begin()->second;
        regions.erase(regions.begin());
    }
}

void mainMenu()
{
    //say("in main menu!");
    clearMenu();
    if (p) {
        delete p;
        p = nullptr;
    }
    clearGameObjects();
    paused = false;
    clearRegions();
    if (!inMainMenu) {
        ingameZoomScale = zoomScale;
        zoomScale = 1;
        inMainMenu = true;
        for (int i = 0; i < 420; i++) physFunc();
        //clearGameObjs();
    }
    //clearMenu();
    //new MenuRect(0, 23, 100, 4,
    //    sf::Color(40, 10, 10));
    //new MenuRect(36, 19, 28, 12,
    //    sf::Color(40, 10, 10));

    //new MenuRect(0, 24, 100, 2,
    //    sf::Color(50, 50, 50));
    //
    ////border around window
    //new MenuRect(0, 0, 100, 1,
    //    sf::Color(0, 0, 0));
    //new MenuRect(0, 0, 1, 100,
    //    sf::Color(0, 0, 0));
    //new MenuRect(0, 99, 100, 1,
    //    sf::Color(0, 0, 0));
    //new MenuRect(99, 0, 1, 100,
    //    sf::Color(0, 0, 0));

    //new MenuText(
    //    "Welcome To The Bintr Bunk",
    //    50, 25,
    //    32,
    //    sf::Color(255, 255, 255),
    //    true,
    //    true,
    //    sf::Color(50, 50, 50),
    //    true,
    //    sf::Color(40, 10, 10));
    ///new Brush(x100 * 40- (res_x / 2),  y100 * 10 - (res_y / 2), x100 * 20, y100 * 20);
    //rect around bottons
    //new MenuRect(3, 49, 32, 48,
    //    sf::Color(50, 50, 50));

    //MenuSlider* testSlider = new MenuSlider("prompt","x",
    //                                        500,300,100,20,
    //                                        &camx,69,420);
    //menuObjects.emplace_back(testSlider);

    TextObject* testText = new TextObject("Welcome To The Bintr Bunk",
                                          x100 * 50, y100 * 20,
                                          Color::White,
                                          50);
    int textW = testText->getW();
    int textH = testText->getH();
    testText->text.setOrigin(textW / 2, testText->getH() / 2);

    menuObjects.emplace_back(new MenuBox(x100 * 50 - textW / 2 - 25,
                                         y100 * 20 - textH / 2 - 25,
                                         textW + 50, textH + 50,
                                         Color(80, 20, 20)));
    menuObjects.emplace_back(testText);
    bSingleplayer = new Button(
        "Singleplayer",
        singleplayerMenu,
        4, 50, 30, 10,
        hsvtorgb(135, 0.75, 0.6),
        //hsvtorgb(135, 0.75, 0.8),
        hsvtorgb(135, 1, 1));
    Button* bMultiplayer = new Button(
        "Multiplayer",
        NULL,
        4, 62, 30, 10,
        hsvtorgb(220, 0.75, 0.6),
        //hsvtorgb(220, 0.75, 0.8),
        hsvtorgb(220, 1, 1));
    bOptions = new Button(
        "Options",
        optionsMenu,
        4, 74, 30, 10,
        hsvtorgb(23, 0.75, 0.6),
        //hsvtorgb(23, 0.75, 0.8),
        hsvtorgb(23, 1, 1));
    Button* bQuit = new Button(
        "Quit",
        quitGame,
        4, 86, 30, 10,
        hsvtorgb(0, 0.75, 0.6),
        //hsvtorgb(0, 0.75, 0.8),
        hsvtorgb(0, 1, 1));
    bSingleplayer->neighborU = bQuit;
    bSingleplayer->neighborD = bMultiplayer;
    bMultiplayer->neighborU = bSingleplayer;
    bMultiplayer->neighborD = bOptions;
    bOptions->neighborU = bMultiplayer;
    bOptions->neighborD = bQuit;
    bQuit->neighborU = bOptions;
    bQuit->neighborD = bSingleplayer;
    activeMenuObj = bSingleplayer;

    menuObjects.emplace_back(bSingleplayer);
    menuObjects.emplace_back(bMultiplayer);
    menuObjects.emplace_back(bOptions);
    menuObjects.emplace_back(bQuit);
    //inMainMenu = true;
    //threadLock.unlock();
}

bool fullscreen = false;
int disp_resx = resx;
int disp_resy = resy;

void applyResolution() {
    updateWindow();
    optionsMenu();
    activeMenuObj = bApply;
}

void mainMenuFromOptions() {
    mainMenu();
    activeMenuObj = bOptions;
}

void mainMenuFromSingleplayerMenu() {
    mainMenu();
    activeMenuObj = bSingleplayer;
}

void pauseMenuFromOptions() {
    pauseMenu();
    activeMenuObj = bOptions;
}



void optionsMenu()
{
    clearMenu();
    if (paused) {
        // darken screen
        menuObjects.emplace_back(new MenuBox(0, 0,
                                             resx, resy,
                                             Color(0, 0, 0, 100)));
    }
    if (fullscreen)
    {
        bFullscreen = new Button(
            "Fullscreen: On",
            toggleFullscreen,
            4, 38, 30, 10,
            hsvtorgb(23, 0.75, 0.6),
            //hsvtorgb(23, 0.75, 0.8),
            hsvtorgb(23, 1, 1));
    }
    else
    {
        bFullscreen = new Button(
            "Fullscreen: Off",
            toggleFullscreen,
            4, 38, 30, 10,
            hsvtorgb(23, 0.75, 0.6),
            //hsvtorgb(23, 0.75, 0.8),
            hsvtorgb(23, 1, 1));
    }
    bResolution = new Button(
        "Resolution: " + to_string(disp_resx) + " x " + to_string(disp_resy),
        changeResolution, //ChangeResolution,
        4, 50, 30, 10,
        hsvtorgb(23, 0.75, 0.6),
        //hsvtorgb(23, 0.75, 0.8),
        hsvtorgb(23, 1, 1));
    Button* bControls = new Button(
        "Controls",
        NULL, //Controls,
        4, 62, 30, 10,
        hsvtorgb(23, 0.75, 0.6),
        //hsvtorgb(23, 0.75, 0.8),
        hsvtorgb(23, 1, 1));
    bApply = new Button(
        "Apply",
        applyResolution, //ApplyResolution,
        4, 74, 30, 10,
        hsvtorgb(135, 0.75, 0.6),
        //hsvtorgb(135, 0.75, 0.8),
        hsvtorgb(135, 1, 1));

    if (inMainMenu)
    {
        bBack = new Button(
            "Back",
            mainMenuFromOptions, //MainMenu,
            4, 86, 30, 10);
    }
    else //paused
    {
        bBack = new Button(
            "Back",
            pauseMenuFromOptions, //PauseMenu,
            4, 86, 30, 10);
    }
    bFullscreen->neighborU = bBack;
    bResolution->neighborU = bFullscreen;
    bControls->neighborU = bResolution;
    bApply->neighborU = bControls;
    bBack->neighborU = bApply;
    bFullscreen->neighborD = bResolution;
    bResolution->neighborD = bControls;
    bControls->neighborD = bApply;
    bApply->neighborD = bBack;
    bBack->neighborD = bFullscreen;
    menuObjects.emplace_back(bFullscreen);
    menuObjects.emplace_back(bResolution);
    menuObjects.emplace_back(bControls);
    menuObjects.emplace_back(bApply);
    menuObjects.emplace_back(bBack);
    activeMenuObj = bBack;
    circleButton = bBack;


}


extern float handGravity;
extern bool leftGrab;
extern bool rightGrab;
void respawnPlayer() {
    //p->alive = true;
    //p->point.setPosition(vec(0, 0));
    //pp = &p->point;
    // randomize spawn a bit
    vec spawnFound(0, 0);
    int radius = 80;
    while (spawnFound == vec(0, 0)) {
        vec rV = randVec(300, 500);
        if (!checkTerrainCircle(p->pos + rV, radius)) spawnFound = rV;
        radius--;
    }
    leftGrab = false;
    rightGrab = false;
    p->pos += spawnFound;
    p->posLast += spawnFound;
}


void goSingleplayer()
{

    clearGameObjects();
    clearRegions();

    // TODO: sort this region clearing business out

    inMainMenu = false;
    //while (regions.size() > 0) {
    //    delete regions.begin()->second;
    //    regions.erase(regions.begin());
    //}
    regions.clear();
    //int minRegionX = getRegion(cam.x - (resx / 2)*zoomScale) - 1;
    //int maxRegionX = getRegion(cam.x + (resx / 2)*zoomScale) + 1;
    //int minRegionY = getRegion(cam.y - (resy / 2)*zoomScale) - 1;
    //int maxRegionY = getRegion(cam.y + (resy / 2)*zoomScale) + 1;
    //for (int x = minRegionX; x <= maxRegionX; x++) {
    //    for (int y = minRegionY; y <= maxRegionY; y++) {
    //        if (regions.find(make_pair(x, y)) == regions.end()) {
    //            new Region(x, y);
    //        }
    //    }
    //}
    clearMenu();
    inMainMenu = false;
    // make our colors
    float h = randnum(18, 23);
    float s = randnum(.2, .5);
    float v = randnum(.8, 1);
    sf::Color skinColor = hsvtorgb(skinH, skinS*.01, skinV*.01);
    sf::Color shirtColor = hsvtorgb(shirtH, shirtS*.01, shirtV*.01);
    sf::Color pantsColor = hsvtorgb(pantsH, pantsS*.01, pantsV*.01);
    if (p) delete p;
    p = new Player;// (skinColor, shirtColor, pantsColor);

    respawnPlayer();


    //p->setPos(200000000, 200000000);
    //CreateMap();
}

extern int safeZone;
extern int passes;
extern int passStart;


extern vector<RenderTexture*> shadowPasses;
extern vector<RenderTexture*> shadowPasses2;

void toggleFullscreen()
{
    fullscreen = (fullscreen == false);
    optionsMenu();
    activeMenuObj = bFullscreen;
}

sf::VideoMode launchResolution(resx, resy);
vector<sf::VideoMode> resolutions;
int res_item = resolutions.size() - 1;


void changeResolution()
{
    resolutions.clear();
    resolutions.emplace_back(launchResolution);
    resolutions = sf::VideoMode::getFullscreenModes();
    if (justReleasedRightClick)
    {
        res_item--;
        if (res_item < 0)
            res_item = resolutions.size() - 1;
    }
    else // if (justReleasedLeftClick) // assume they want a left click input (for controllers)
    {
        res_item++;
        if (res_item >= resolutions.size())
            res_item = 0;
    }

    disp_resx = resolutions[res_item].width;
    disp_resy = resolutions[res_item].height;

    optionsMenu();
    activeMenuObj = bResolution;
}


//void quitGame() {
//    //threadLock.lock();
//    window->close();
//    //threadLock.unlock();
//}
void quitGame() {
    running = false;
    //threadLock.lock();
    window->close();
    //threadLock.unlock();
}

void unpause() {
    clearMenu();
    paused = false;
}



void pauseMenu() {
    clearMenu();
    // darken screen
    menuObjects.emplace_back(new MenuBox(0, 0,
                                         resx, resy,
                                         Color(0, 0, 0, 100)));
    bOptions = new Button(
        "Options",
        optionsMenu,
        4, 74, 30, 10,
        hsvtorgb(23, 0.75, 0.6),
        //hsvtorgb(23, 0.75, 0.8),
        hsvtorgb(23, 1, 1));
    Button* bResume = new Button(
        "Resume",
        unpause,
        4, 86, 30, 10,
        hsvtorgb(135, 0.75, 0.6),
        //hsvtorgb(135, 0.75, 0.8),
        hsvtorgb(135, 1, 1));
    //if (Multiplayer)
    //{
    //    menuObjects.emplace_back(new Button(
    //        "Disconnect",
    //        mainMenu,
    //        66, 86, 30, 10));
    //    //,
    //    //hsvtorgb(0, 0.75, 0.6),
    //    //hsvtorgb(0, 0.75, 0.8),
    //    //hsvtorgb(0, 1, 1));
    //}
    //else //singleplayer
    //{
    Button* bMainMenu = new Button(
        "Main Menu",
        mainMenu,
        66, 86, 30, 10);
    //,
    //hsvtorgb(0, 0.75, 0.6),
    //hsvtorgb(0, 0.75, 0.8),
    //hsvtorgb(0, 1, 1));
    //}

    bOptions->neighborR = bMainMenu;
    bOptions->neighborL = bMainMenu;
    bOptions->neighborU = bResume;
    bOptions->neighborD = bResume;
    bResume->neighborR = bMainMenu;
    bResume->neighborL = bMainMenu;
    bResume->neighborU = bOptions;
    bResume->neighborD = bOptions;
    bMainMenu->neighborL = bResume;
    bMainMenu->neighborR = bResume;

    menuObjects.emplace_back(bOptions);
    menuObjects.emplace_back(bResume);
    menuObjects.emplace_back(bMainMenu);
    activeMenuObj = bResume;
    circleButton = bResume;
}

void clearMenu() {
    activeMenuObj = NULL;
    activeMenuObjLast = NULL;
    circleButton = NULL;
    while (menuObjects.size() > 0) { delete menuObjects[0]; menuObjects.erase(menuObjects.begin()); }

}