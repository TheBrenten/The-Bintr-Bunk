#pragma once
#include "Region.h"
#include "Player.h"
#include "MenuObjects.h"
#include "PhysicsStuff.h"

extern vector<MenuObject*> menuObjects;
extern bool inMainMenu;
extern bool fullscreen;
extern int resx;
extern int resy;
extern int disp_resx;
extern int disp_resy;
extern float x100;
extern float y100;
extern Player* p;
extern float zoomScale;

void updateWindow();


void singleplayerMenu();
void mainMenu();
void mainMenuFromSingleplayerMenu();
void optionsMenu();
void respawnPlayer();
void goSingleplayer();
void toggleFullscreen();

void changeResolution();



void quitGame();

void pauseMenu();
void clearMenu();