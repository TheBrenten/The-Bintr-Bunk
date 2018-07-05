#pragma once
#include "SFML/Graphics.hpp"
#include "BintrsTools.h"
#include "Enums.h"
#include <iostream>
#define say(x) std::cout<<x<<'\n'
#define itemInVector(x,y) find(y.begin(),y.end(),x) != y.end()
using namespace std;
extern sf::RenderWindow* window;
class GameObject;
class Region;
extern map<pair<int,int>,Region*> regions;
extern vector<pair<int, int>> regionsToMake;


// perlin business
const int regionSize = 512; 
const int perlinDist = 500 * 1.2;
const int perlinDistBiome = 4750 * 1.2;
const float biomeInfluence = 0.58;
extern std::map<std::pair<int, int>, vec> perlinGrid;
extern std::map<std::pair<int, int>, vec> perlinGridBiome;

// used for perlin noise
// cells are subdivided if their corners terrain dont match
struct cell {
    int x, y, size;
    cell(int x, int y, int size) {
        this->x = x;
        this->y = y;
        this->size = size;
    }
};


int getPerlinTerrain(int worldX, int worldY);


#include <thread>
const int fuck = regionSize*regionSize * 4;

// PBO includes
#define GL_GLEXT_PROTOTYPES
#include <SFML/OpenGL.hpp>
//#include <glut.h>
#include "glext.h"
#include "glInfo.h"

class Node {
public:
    int x, y;
    vector<Node*> neighbors;
    vector<Node*> validNeighbors;
    Region* inRegion;
    Node(int x, int y, Region* r) {
        this->x = x;
        this->y = y;
        this->inRegion = r;
    }
    bool operator == (Node n) {
        if (this->x == n.x &&
            this->y == n.y) return true;
        return false;
    }
    ~Node() {

    }
};

class Region
{
public:
    bool nodesValidated = false;
    map<pair<int, int>, Node*> nodes;
    // keep regions around until theyre offscreen for 5 seconds
    int life = 300;
    // pbo
    GLuint textureId;
    GLuint pboIds[2];
    
    sf::Texture texture;
    bool textureUpdated = false;
    sf::Sprite sprite;
    int RX, RY, x, y;
    vector<GameObject*> containedObjects;
    uint8_t pixels [regionSize*regionSize*4];
    sf::RectangleShape disp_rect;
    void render();
    /*
    constructor must do the following
    -set RX,RY,x,y
    -if regions/file_X_Y exists
      -load file into texture
    -else
      -generate texture
    -set sprite texture
    -set sprite position
    */
    Region(int RX, int RY);
    void spawnEnemies();
    void placeNodes();
    
    // for pathfinding
    void validateNodes();

    // generate terrain, update pixels/texture
    void genTerrain();


    void renderDebug();
    ~Region();
};
extern bool loadingRegions;
extern bool running;

void regionThread(pair<int, int> key);

int getRegion(int a);

// Begin region creation if not started already
void confirmRegion(int RX, int RY);

std::pair<int, int> getRegion(int x, int y);

std::pair<int, int> getRegion(vec v);
