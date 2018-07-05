#include "RenderFunc.h"
#include "SFML/Graphics.hpp"
#include "Region.h"
#include "BintrsTools.h"
#include "Particle.h"
#include "GameFunction.h"
#include "Enemy.h"
#include "Player.h"
#include "MenuFunctions.h"
#include <iostream>
#include <utility>
#include <map>
using namespace std;
#define say(x) std::cout<<x<<'\n'
#define keyInMap(x,y) y.find(x)!=y.end()
#define itemInVector(x,y) find(y.begin(),y.end(),x) != y.end()

int selectedHotbarItem = 0;

extern sf::RenderWindow* window;
bool debugDraw;
bool zoomIn  = false;
bool zoomOut = false;
float zoomScale = 1;

extern map<pair<int, int>,Region*> regions;
vector<Region*> renderRegions;
vector<GameObject*> renderObjects;

vec cam;
vec camLast;
vec camInterp;
extern int resx;
extern int resy;
float x100;
float y100;
bool windowFocused = true;

bool moveLeft = false;
bool moveRight = false;
bool moveUp = false;
bool moveDown = false;
bool noclip = false;
bool jumping = false;
bool justJumped = false;
extern bool paused;

vec globalMouse;
vec screenMouse;
sf::Vector2i savedMouse;
bool justPressedLeftClick = false;
bool justReleasedLeftClick = false;
bool justPressedRightClick = false;
bool justReleasedRightClick = false;
bool leftActionPressed = false;
bool rightActionPressed = false;
bool leftActionReleased = false;
bool rightActionReleased = false;


extern float physRate;
extern float FPS;
extern GameFunction gfPhysics;
extern GameFunction gfRender ;
extern GameFunction gfAnim   ;

extern bool loadingRegions;
extern Region* lastCreatedRegion;
extern vector<pair<int, int>> regionsToMake;
extern vector<Dot*> dots;

bool hotbar = false;
bool inventory = false;

timer fpsT;
bool running = true;
bool sprinting = false;
bool leftAction = false;
bool rightAction = false;
bool leftGrab  = false;
bool rightGrab = false;
bool reloading = false;

extern int grabSteps;
extern int leftGrabTracker;
extern int rightGrabTracker;
extern Player* p;

// we need to do some crafty thread stuff
// because if any regions are generating while we try to close
// we'll get a .exe has stopped working error on close
///extern vector<BintrSound*> activeSounds;

#include "Item.h"
vector<Item*> hotbarItems;

void drawHotbar(vec hbPos, float hbRad) {
    float hbRadInner = hbRad*.15;
    CircleShape inv_circ(hbRad);
    inv_circ.setOrigin(hbRad, hbRad);
    inv_circ.setPosition(hbPos.x, hbPos.y);
    inv_circ.setFillColor(Color::Transparent);
    inv_circ.setOutlineColor(Color::Black);
    inv_circ.setOutlineThickness(4);
    CircleShape inv_circ_inner(hbRadInner);
    inv_circ_inner.setOrigin(hbRadInner, hbRadInner);
    inv_circ_inner.setPosition(hbPos.x, hbPos.y);
    inv_circ_inner.setFillColor(Color::Transparent);
    inv_circ_inner.setOutlineColor(Color::Black);
    inv_circ_inner.setOutlineThickness(4);


    // draw lines in circle
    float hbItems = hotbarItems.size();

    RectangleShape hbDivider(Vector2f(hbRad - hbRadInner, 4));
    hbDivider.setFillColor(Color::Black);
    hbDivider.setOrigin(-hbRad*.15, 1);
    hbDivider.setPosition(hbPos.x, hbPos.y);
    vec selector; // could be keyboard or controller
    if (Joystick::isButtonPressed(0, 2)) {
        selector = vec(Joystick::getAxisPosition(NULL, Joystick::Axis::X),
                       Joystick::getAxisPosition(NULL, Joystick::Axis::Y))*(.01*hbRad);
        if (selector.getLength() > hbRad) selector.norm(hbRad - 5);
        selector += vec(resx*.5, resy*.5);
    }
    else {
        selector = screenMouse;
    }
    //selector.print();
    if ((selector - hbPos).getLength() < hbRadInner) {
        inv_circ_inner.setFillColor(Color(255, 255, 255, 50));
        if (justPressedLeftClick) p->setLeftItem(NULL);
        if (justPressedRightClick) p->setRightItem(NULL);
    }
    else if ((selector - hbPos).getLength() < hbRad) {
        // fill in pie piece
        VertexArray piePiece(TriangleStrip, ceil(30.f / hbItems) * 2 + 2);
        float ang = (selector - hbPos).getAngle();
        int slot = ang / 360 * hbItems;
        if (hotbarItems[slot]->isWeapon) {
            if (justPressedLeftClick) { 
                p->setLeftItem(hotbarItems[slot]->weaponP);
                selectedHotbarItem = slot;
            }
            // p->leftHanded = true; }
            ////////////////if (justPressedRightClick) { p->setRightItem(hotbarItems[slot]->weaponP); p->leftHanded = false; }
        }
        for (int i = 0; i < ceil(30.f / hbItems); i++) {
            //CircleShape ass(4);
            //ass.setOrigin(4, 4);
            //ass.setFillColor(Color::Red);
            vec uh = hbPos + vec(hbRadInner, 0).rotated(slot*(360.f / hbItems) + (360.f / 30) * i);
            piePiece[i * 2].position = Vector2f(uh.x, uh.y);
            piePiece[i * 2].color = Color(255, 255, 255, 50);
            //ass.setPosition(uh.x,uh.y);
            //window->draw(ass);
            vec uh2 = hbPos + vec(hbRad, 0).rotated(slot*(360.f / hbItems) + (360.f / 30) * i);
            piePiece[i * 2 + 1].position = Vector2f(uh2.x, uh2.y);
            piePiece[i * 2 + 1].color = Color(255, 255, 255, 50);
            //ass.setPosition(uh2.x, uh2.y);
            //window->draw(ass);
        }
        // add last 2 pie piece points
        vec uh = hbPos + vec(hbRadInner, 0).rotated((slot + 1)*(360.f / hbItems));
        vec uh2 = hbPos + vec(hbRad, 0).rotated((slot + 1)*(360.f / hbItems));

        piePiece[piePiece.getVertexCount() - 2].position = Vector2f(uh.x, uh.y);
        piePiece[piePiece.getVertexCount() - 2].color = Color(255, 255, 255, 50);
        piePiece[piePiece.getVertexCount() - 1].position = Vector2f(uh2.x, uh2.y);
        piePiece[piePiece.getVertexCount() - 1].color = Color(255, 255, 255, 50);
        window->draw(piePiece);

    }
    window->draw(inv_circ);
    window->draw(inv_circ_inner);
    Sprite weaponSprite;
    for (int i = 0; i < hbItems; i++) {
        weaponSprite = hotbarItems[i]->sprite;
        hbDivider.setRotation((360 / hbItems)*i);
        window->draw(hbDivider);
        float ang = (360 / hbItems) * (i + .5);
        vec itemPos = hbPos + vec(hbRad*.5, 0).rotated(ang);
        weaponSprite.setRotation(ang);
        if (ang > 90 && ang < 270) weaponSprite.setScale(2, -2);
        else weaponSprite.setScale(2, 2);
        weaponSprite.setPosition(itemPos.x, itemPos.y);
        window->draw(weaponSprite);
    }

}

extern vec screenShakePos;
extern float screenShakeAngle;
extern vec screenShakePosInterp;
extern float screenShakeAngInterp;

void renderFunc()
{
    
    // input events
    {
        moveLeft               = 
        moveRight              = 
        moveUp                 = 
        moveDown               = 
        zoomIn                 = 
        zoomOut                = 
        jumping                = 
        sprinting              = 
        leftAction             = 
        rightAction            = 
        justPressedLeftClick   = 
        justReleasedLeftClick  =
        justPressedRightClick  =
        justReleasedRightClick = 
        reloading              = false;    
        if (windowFocused) {
            if (!paused) {
                if (sf::Keyboard::isKeyPressed(Keyboard::A)) moveLeft = true;
                if (sf::Keyboard::isKeyPressed(Keyboard::D)) moveRight = true;
                if (sf::Keyboard::isKeyPressed(Keyboard::W)) moveUp = true;
                if (sf::Keyboard::isKeyPressed(Keyboard::S)) moveDown = true;
                if (sf::Keyboard::isKeyPressed(Keyboard::Space)) jumping = true;
                if (sf::Keyboard::isKeyPressed(Keyboard::LShift)) sprinting = true;
                if (sf::Keyboard::isKeyPressed(Keyboard::R)) reloading = true;
            }
            if (!hotbar && !inventory && !paused) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) leftAction = true;
                if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) rightAction = true;
            }
            if (sf::Keyboard::isKeyPressed(Keyboard::Add))      zoomIn = true;
            if (sf::Keyboard::isKeyPressed(Keyboard::Subtract)) zoomOut = true;
            sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
            screenMouse = vec(mousePos.x, mousePos.y);
            globalMouse = vec(int(cam.x + (-resx / 2 + mousePos.x)*zoomScale),
                              int(cam.y + (-resy / 2 + mousePos.y)*zoomScale));

            if (Keyboard::isKeyPressed(Keyboard::Key::Num1)) {
                //drawingTerrain = true;
                //drawTerrain = air;
                destroyTerrainCircle(globalMouse, 69);
                for (int i = activeObjects.size() - 1; i >= 0; i--) {
                    if (activeObjects[i]->isEnemy &&
                        (activeObjects[i]->pos - globalMouse).getLength() < 69) {
                        activeObjects[i]->hurt(activeObjects[i]->health);
                        //activeObjects[i]->health = -1;
                        //delete activeObjects[i];
                    }
                }
            }
            if (Keyboard::isKeyPressed(Keyboard::Key::Num2)) {
                placeTerrainCircle(globalMouse, 69, dirt);
            }
            if (Keyboard::isKeyPressed(Keyboard::Key::Num3)) {
                placeTerrainCircle(globalMouse, 69, stone);
            }

            if (sf::Keyboard::isKeyPressed(Keyboard::Key::Num0)) new Enemy(globalMouse);

        }
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                quitGame();
                //window->close();
            }
            if (event.type == sf::Event::GainedFocus) {
                windowFocused = true;
            }
            if (event.type == sf::Event::LostFocus) {
                windowFocused = false;
            }
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (p) {
                    for (int i = 0; i < event.mouseWheelScroll.delta;i++) {
                        selectedHotbarItem--;
                        if (selectedHotbarItem < 0) selectedHotbarItem += hotbarItems.size();
                    }
                    for (int i = 0; i > event.mouseWheelScroll.delta; i--) {
                        selectedHotbarItem++;
                        if (selectedHotbarItem > hotbarItems.size()) selectedHotbarItem = 0;
                    }
                    p->setLeftItem(hotbarItems[selectedHotbarItem]->weaponP);
                }

            }
            if (event.type == event.MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    justPressedLeftClick = true;
                    if (!hotbar && !inventory && !paused) {
                        leftActionPressed = true;
                        leftGrabTracker = grabSteps;
                        if (!leftGrab) leftGrabTracker = 0;
                        leftGrab = false;
                    }
                }
                if (event.mouseButton.button == sf::Mouse::Right) {
                    justPressedRightClick = true;
                    if (!hotbar && !inventory && !paused) {
                        rightActionPressed = true;
                        rightGrabTracker = grabSteps;
                        if (!rightGrab) rightGrabTracker = 0;
                        rightGrab = false;
                    }
                }
            }
            if (event.type == event.MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    justReleasedLeftClick = true;
                    if (!hotbar && !inventory && !paused) {
                        leftActionReleased = true;
                    }
                }
                if (event.mouseButton.button == sf::Mouse::Right) {
                    justReleasedRightClick = true;
                    if (!hotbar && !inventory && !paused) {
                        rightActionReleased = true;
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Q) {
                    if (p) {
                        if (lItem || rItem || mainItem) {
                            p->setLeftItem(nullptr);
                        }
                        else {
                            p->setLeftItem(hotbarItems[selectedHotbarItem]->weaponP);
                        }
                    }
                }
                if (event.key.code == sf::Keyboard::X) {
                    hotbar = true;
                    savedMouse = sf::Mouse::getPosition(*window);
                    sf::Mouse::setPosition(sf::Vector2i(resx*.5, resy*.5),*window);
                }
                if (event.key.code == sf::Keyboard::Comma) {

                }
                if (event.key.code == sf::Keyboard::Period) {

                }

                if (event.key.code == sf::Keyboard::Up);//{ if (!sf::Keyboard::isKeyPressed(Keyboard::LShift)) { testOrigin.y--; cout << "Origin: "; testOrigin.print(); } else { testOffset.y--; cout << "Offset: "; testOffset.print(); } }
                if (event.key.code == sf::Keyboard::Down);//{ if (!sf::Keyboard::isKeyPressed(Keyboard::LShift)) { testOrigin.y++; cout << "Origin: "; testOrigin.print(); } else { testOffset.y++; cout << "Offset: "; testOffset.print(); } }
                if (event.key.code == sf::Keyboard::Left);//{ if (!sf::Keyboard::isKeyPressed(Keyboard::LShift)) { testOrigin.x--; cout << "Origin: "; testOrigin.print(); } else { testOffset.x--; cout << "Offset: "; testOffset.print(); } }
                if (event.key.code == sf::Keyboard::Right);//{ if (!sf::Keyboard::isKeyPressed(Keyboard::LShift)) { testOrigin.x++; cout << "Origin: "; testOrigin.print(); } else { testOffset.x++; cout << "Offset: "; testOffset.print(); } }

                if (event.key.code == sf::Keyboard::P) {
                    if (physRate == 60) physRate = 13;
                    else if (physRate == 13) physRate = 240;
                    else if (physRate == 240) physRate = 60;
                }

                //if (event.key.code == sf::Keyboard::W) if (activeMenu Obj != NULL && activeMenuObj->neighborU) { activeMenuObj->pressed = false; activeMenuObj = activeMenuObj->neighborU; }
                //else if (activeMenuObjLast) activeMenuObj = activeMenuObjLast;
                //if (event.key.code == sf::Keyboard::S) if (activeMenuObj != NULL && activeMenuObj->neighborD) { activeMenuObj->pressed = false; activeMenuObj = activeMenuObj->neighborD; }
                //else if (activeMenuObjLast) activeMenuObj = activeMenuObjLast;
                //if (event.key.code == sf::Keyboard::A) if (activeMenuObj != NULL && activeMenuObj->neighborL) { activeMenuObj->pressed = false; activeMenuObj = activeMenuObj->neighborL; }
                //else if (activeMenuObjLast) activeMenuObj = activeMenuObjLast;
                //if (event.key.code == sf::Keyboard::D) if (activeMenuObj != NULL && activeMenuObj->neighborR) { activeMenuObj->pressed = false; activeMenuObj = activeMenuObj->neighborR; }
                //else if (activeMenuObjLast) activeMenuObj = activeMenuObjLast;
                //if (event.key.code == sf::Keyboard::E) inventory = !inventory;
                if (event.key.code == sf::Keyboard::T) {
                    if (!paused) {
                        noclip = !noclip;
                    }
                }
                if (event.key.code == sf::Keyboard::Num5) new Enemy(globalMouse);
                if (event.key.code == sf::Keyboard::P) {
                }
                if (event.key.code == sf::Keyboard::Space) {
                    if (!hotbar && !inventory && !paused) {
                        justJumped = true;
                    }
                }
                if (event.key.code == sf::Keyboard::Return) zoomScale = 1;
                if (event.key.code == sf::Keyboard::BackSpace) debugDraw = !debugDraw;
                if (event.key.code == sf::Keyboard::Escape) {
                    if (!inMainMenu) {
                        if (!paused) {
                            pauseMenu();
                        }
                        else {
                            clearMenu();
                        }
                        paused = !paused;
                    }
                }
                if (event.key.code == sf::Keyboard::K) {

                }
                if (event.key.code == sf::Keyboard::E) {
                    if (!paused && !hotbar) {
                        inventory = !inventory;
                    }
                }

            }
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::X) {
                    hotbar = false;
                    if ((screenMouse - vec(resx*.5, resy*.5)).getLength() < x100 * 15) {
                        sf::Mouse::setPosition(savedMouse,*window);
                    }
                }

                if (event.key.code == sf::Keyboard::Space) {

                }
            }
        }
        
    }
    
    // interpolate camera
    float interp = ((gfPhysics.clock.elapsed())*(1.f / gfPhysics.rate));
    if (interp > 1) interp = 1;
    camInterp = camLast + (cam - camLast) * interp;

    View camView1 = View(Vector2f(camInterp.x, camInterp.y), Vector2f(resx*zoomScale, resy*zoomScale));
    vec   screenShakePosRender = interpVec(screenShakePosInterp, screenShakePos);
    float screenShakeAngRender = interpAng(screenShakeAngInterp,screenShakeAngle);
    camView1.move(screenShakePosRender.x, screenShakePosRender.y);
    camView1.setRotation(screenShakeAngRender);

    // place camera
    window->setView(camView1);
    
    // region/object rendering
    renderRegions.clear();
    renderObjects.clear();
    

    int RXmin = getRegion(camInterp.x - (resx / 2)*zoomScale) - 1;
    int RXmax = getRegion(camInterp.x + (resx / 2)*zoomScale) + 1;
    int RYmin = getRegion(camInterp.y - (resy / 2)*zoomScale) - 1;
    int RYmax = getRegion(camInterp.y + (resy / 2)*zoomScale) + 1;
    //int RXmin = getRegion(camInterp.x - (resx / 2)) - 1;
    //int RXmax = getRegion(camInterp.x + (resx / 2)) + 1;
    //int RYmin = getRegion(camInterp.y - (resy / 2)) - 1;
    //int RYmax = getRegion(camInterp.y + (resy / 2)) + 1;

    // for all visible regions...
    int regionsMade = 0;
    timer rTimer;
    for (int RX = RXmin; RX <= RXmax; RX++) {
        for (int RY = RYmin; RY <= RYmax; RY++) {
            pair<int, int> key = make_pair(RX, RY);
            // if region exists...
            if (keyInMap(key,regions)) {
            //if (keyInMap(make_pair(RX,RY) , regions)) {
                // and region texture is updated...
                Region* r = regions[key];
                r->life = 300;
                // add region to render list
                renderRegions.emplace_back(r);
                // for all regions contained objects...
                for (int i = 0; i < r->containedObjects.size(); i++) {
                    // if object isnt already in render list...
                    if (!(itemInVector(r->containedObjects[i], renderObjects))) {
                        // add object to render list
                        renderObjects.emplace_back(r->containedObjects[i]);
                    }
                }
            }
            // else create it
            else {
                confirmRegion(RX, RY);
            }
        }
    }

    for (auto pl : players) {
        pl->setImages();
    }

    window->clear(sf::Color(30, 30, 30));
    
    sort(renderObjects.begin(), renderObjects.end(), [](GameObject* a, GameObject* b) {return a->layer < b->layer; });
    //for (int i = renderObjects.size() - 1; i >= 0; i--) {
    //    GameObject* obj = renderObjects[i];
    //    obj->renderBack();
    //    if (debugDraw) obj->renderDebug();
    //}

    // draw objects in layer order
    int i;
    for (i = 0; i<renderObjects.size(); i++) {
        if (renderObjects[i]->layer >= 0) break; // stop short of layer 0
        renderObjects[i]->render();
    }

    // draw terrain on layer 0
    for (int i = 0; i < renderRegions.size(); i++) {
        renderRegions[i]->render();
        if (debugDraw) renderRegions[i]->renderDebug();
    }

    for (; i<renderObjects.size(); i++) {
        renderObjects[i]->render();
    }

    if (debugDraw) {
        for (auto obj : renderObjects) {
            obj->renderDebug();
        }
    }
    // note: this following code is really good
    // just, holy shit, fuck

    // if region thread is not busy
    if (!loadingRegions) {
        // if thread finished creating region
        if (lastCreatedRegion) {
            // add region to map
            regions[regionsToMake[0]] = lastCreatedRegion;
            // remove region from queue
            regionsToMake.erase(regionsToMake.begin());
            // other region initialization (important for main thread)
            if (!inMainMenu) {
                lastCreatedRegion->placeNodes();
                lastCreatedRegion->validateNodes();
                lastCreatedRegion->spawnEnemies();
            }
            // dereference region
            lastCreatedRegion = nullptr;
        }
        // if we want to make regions
        if (regionsToMake.size() > 0) {
            loadingRegions = true;
            // queue the region closest to player to be created next
            // aka set it as regionsToMake[0]
            if (p) {
                pair<int, int> pReg = getRegion(p->pos);
                vec pVec = vec(pReg.first, pReg.second);
                int closestInd = 0;
                float lowestDist = 10;
                for (int i = 0; i < regionsToMake.size(); i++) {
                    float dist = (vec(regionsToMake[i].first, regionsToMake[i].second) - pVec).getLength();
                    if (dist < lowestDist) {
                        lowestDist = dist;
                        closestInd = i;
                    }
                }
                if (closestInd) {
                    regionsToMake.insert(regionsToMake.begin(), regionsToMake[closestInd]);
                    regionsToMake.erase(regionsToMake.begin() + closestInd + 1);
                }
            }
            pair<int, int> key = regionsToMake[0];
            thread rThread(regionThread, key);
            rThread.detach();
        }

    }

    for (int i = regionsToUpdate.size()-1; i >= 0; i--) {
        Region* reg = regionsToUpdate[i];
        if (!reg->updating);
        thread t(&Region::updateTexture, reg);
        t.detach();
        regionsToUpdate.erase(regionsToUpdate.begin() + i);
    }

    
    // drawing GUI stuff
    window->setView(View(Vector2f(resx / 2, resy / 2),
                         Vector2f(resx, resy)));

    // health bar
    {
        if (p && showHealthBar>0) {
            sf::RectangleShape r;
            r.setPosition(x100,
                          y100 * 96);
            r.setSize(Vector2f(x100 * 16,
                               y100 * 2));
            r.setFillColor(sf::Color(50, 50, 50));
            r.setOutlineColor(sf::Color::Black);
            r.setOutlineThickness(4);
            window->draw(r);
            sf::RectangleShape r2;
            r2.setPosition(x100,
                           y100 * 96);
            r2.setSize(Vector2f((x100 * 16) * (p->health/p->maxHealth),
                               y100 * 2));
            r2.setFillColor(sf::Color(150,0,0));
            if (p->health > 0)
                window->draw(r2);
            
        }
    }

    for (int i = 0; i < menuObjects.size(); i++) {
        MenuObject* m = menuObjects[i];
        m->render();
    }

    if (hotbar) {
        drawHotbar(vec(x100 * 50, y100 * 50), y100 * 40);
    }
    if (inventory) {
        vec hbPos(x100 * 17, y100 * 50);
        float hbRad = x100 * 15;
        drawHotbar(hbPos, hbRad);

        //float invStartX = x100*34;
        //float invStartY = y100*10;
        //

        float imgW = resx;
        float imgRatio = 600 / 400.f;
        float imgH = imgW / float(imgRatio);

        float scale = (x100 * 68 - 50) / 600.f;
        float invW = 600 * scale;
        float invH = 400 * scale;
        float invX = x100 * 32 + (resx - (invW + x100 * 32)) * .5f;
        float invY = (resy - invH) *.5f;
        RectangleShape inv_rect(Vector2f(invW, invH));
        inv_rect.setPosition(invX, invY);
        inv_rect.setFillColor(Color(30, 30, 100));
        inv_rect.setOutlineThickness(4);
        inv_rect.setOutlineColor(Color::Black);
        window->draw(inv_rect);
        RectangleShape inv_gridpiece(Vector2f(40 * scale,
                                              40 * scale));
        inv_gridpiece.setFillColor(Color(100, 100, 100));
        inv_gridpiece.setOutlineColor(Color(0, 0, 0));
        inv_gridpiece.setOutlineThickness(2);
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                inv_gridpiece.setPosition(invX + (20 + x * 40)*scale,
                                          invY + (20 + y * 40)*scale);
                window->draw(inv_gridpiece);
            }
        }
        RectangleShape inv_playerbox(Vector2f(150 * scale,
                                              240 * scale));
        inv_playerbox.setPosition(invX + (395)*scale,
                                  invY + (60)*scale);
        inv_playerbox.setFillColor(Color(100, 100, 100));
        inv_playerbox.setOutlineColor(Color(0, 0, 0));
        inv_playerbox.setOutlineThickness(2);
        window->draw(inv_playerbox);

        RectangleShape inv_arrow(Vector2f(55 * scale,
                                          30 * scale));

        inv_arrow.setPosition(invX + (10)*scale,
                              invY + (355)*scale);
        inv_arrow.setFillColor(Color(100, 100, 100));
        inv_arrow.setOutlineColor(Color(0, 0, 0));
        inv_arrow.setOutlineThickness(2);
        window->draw(inv_arrow);
        inv_arrow.setPosition(invX + (295)*scale,
                              invY + (355)*scale);
        window->draw(inv_arrow);

        CircleShape inv_pagecirc(6 * scale);
        inv_pagecirc.setOrigin(6 * scale,
                               6 * scale);
        inv_pagecirc.setFillColor(Color(100, 100, 100));
        //inv_pagecirc.setOutlineThickness(2);
        //inv_pagecirc.setOutlineColor(Color::Black);
        for (int i = 0; i < 16; i++) {
            inv_pagecirc.setPosition(invX + (75 + i * 14)*scale,
                                     invY + (370)*scale);
            window->draw(inv_pagecirc);
        }

    }

    // Draw physical stuff
    window->setView(camView1);

    //vector<Body*> bodies = { &bPlayer, &bMainHand, &bSecondHand, &bMainItem };
    //for (int i = 0; i < bodies.size(); i++) {
    //    new Dot(bodies[i]->pos);
    //    new Line(bodies[i]->pos, bodies[i]->pos+vec(100, 0).rotated(bodies[i]->angle));
    //}

    // Debug Lines
    for (int i = 0; i < lines.size(); i++) {
        lines [i]->render();
    }

    // Debug Dots
    for (int i = 0; i < dots.size(); i++) {
        dots[i]->render();
    }
    
    window->display();
    //float time = fpsT.elapsed();
    //if (time > 0.02) say("Lag spike! " << time);
    //fpsT.reset();
}
