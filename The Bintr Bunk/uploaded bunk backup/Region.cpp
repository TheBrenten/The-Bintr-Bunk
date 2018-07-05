#include "Region.h"
#include "GameObject.h"
#include "Enemy.h"
#include "MenuFunctions.h"


#ifdef _WIN32
PFNGLGENBUFFERSARBPROC pglGenBuffersARB = 0;                     // VBO Name Generation Procedure
PFNGLBINDBUFFERARBPROC pglBindBufferARB = 0;                     // VBO Bind Procedure
PFNGLBUFFERDATAARBPROC pglBufferDataARB = 0;                     // VBO Data Loading Procedure
PFNGLBUFFERSUBDATAARBPROC pglBufferSubDataARB = 0;               // VBO Sub Data Loading Procedure
PFNGLDELETEBUFFERSARBPROC pglDeleteBuffersARB = 0;               // VBO Deletion Procedure
PFNGLGETBUFFERPARAMETERIVARBPROC pglGetBufferParameterivARB = 0; // return various parameters of VBO
PFNGLMAPBUFFERARBPROC pglMapBufferARB = 0;                       // map VBO procedure
PFNGLUNMAPBUFFERARBPROC pglUnmapBufferARB = 0;                   // unmap VBO procedure
#define glGenBuffersARB           pglGenBuffersARB
#define glBindBufferARB           pglBindBufferARB
#define glBufferDataARB           pglBufferDataARB
#define glBufferSubDataARB        pglBufferSubDataARB
#define glDeleteBuffersARB        pglDeleteBuffersARB
#define glGetBufferParameterivARB pglGetBufferParameterivARB
#define glMapBufferARB            pglMapBufferARB
#define glUnmapBufferARB          pglUnmapBufferARB
#endif




map<pair<int, int>, Region*> regions;
vector<pair<int, int>> regionsToMake;
std::map<std::pair<int, int>, vec> perlinGrid;
std::map<std::pair<int, int>, vec> perlinGridBiome;


int getPerlinTerrain(int worldX, int worldY)
{
    int px = floor(float(worldX) / perlinDist);
    int py = floor(float(worldY) / perlinDist);
    vec TL, TR, BL, BR;
    TL = perlinGrid[std::make_pair(px, py)];
    TR = perlinGrid[std::make_pair(px + 1, py)];
    BL = perlinGrid[std::make_pair(px, py + 1)];
    BR = perlinGrid[std::make_pair(px + 1, py + 1)];
    float localX = float(worldX%perlinDist) / perlinDist;
    float localY = float(worldY%perlinDist) / perlinDist;
    if (localX<0)localX++;
    if (localY<0)localY++;
    float fadeX = 6 * pow(localX, 5) - 15 * pow(localX, 4) + 10 * pow(localX, 3);
    float fadeY = 6 * pow(localY, 5) - 15 * pow(localY, 4) + 10 * pow(localY, 3);
    vec pos(localX, localY);
    float TLdp = (vec(floor(localX), floor(localY)) - pos)*TL;
    float TRdp = (vec(floor(localX + 1), floor(localY)) - pos)*TR;
    float BLdp = (vec(floor(localX), floor(localY + 1)) - pos)*BL;
    float BRdp = (vec(floor(localX + 1), floor(localY + 1)) - pos)*BR;
    float AB = TLdp + fadeX*(TRdp - TLdp);
    float CD = BLdp + fadeX*(BRdp - BLdp);
    float val = AB + fadeY*(CD - AB);
    int pbx = floor(float(worldX) / perlinDistBiome);
    int pby = floor(float(worldY) / perlinDistBiome);
    vec TLB, TRB, BLB, BRB;
    TLB = perlinGridBiome[make_pair(pbx, pby)];
    TRB = perlinGridBiome[make_pair(pbx + 1, pby)];
    BLB = perlinGridBiome[make_pair(pbx, pby + 1)];
    BRB = perlinGridBiome[make_pair(pbx + 1, pby + 1)];
    float localBX = float(worldX%perlinDistBiome) / perlinDistBiome;
    float localBY = float(worldY%perlinDistBiome) / perlinDistBiome;
    if (localBX<0)localBX++;
    if (localBY<0)localBY++;
    float fadeBX = 6 * pow(localBX, 5) - 15 * pow(localBX, 4) + 10 * pow(localBX, 3);
    float fadeBY = 6 * pow(localBY, 5) - 15 * pow(localBY, 4) + 10 * pow(localBY, 3);
    vec pos2(localBX, localBY);
    float TLBdp = (vec(floor(localBX), floor(localBY)) - pos2)*TLB;
    float TRBdp = (vec(floor(localBX + 1), floor(localBY)) - pos2)*TRB;
    float BLBdp = (vec(floor(localBX), floor(localBY + 1)) - pos2)*BLB;
    float BRBdp = (vec(floor(localBX + 1), floor(localBY + 1)) - pos2)*BRB;
    float AB2 = TLBdp + fadeBX*(TRBdp - TLBdp);
    float CD2 = BLBdp + fadeBX*(BRBdp - BLBdp);
    float val2 = AB2 + fadeBY*(CD2 - AB2);
    val = val + biomeInfluence*(val2 - val);
    val = (val + 1) / 2;
    if ((val > 0.488 && val < 0.532))      return air;
    else if ((val < 0.45 || val > 0.55))   return stone;
    else                                   return dirt;
}

bool loadingRegions = false;

Region* lastCreatedRegion = nullptr;

void regionThread(pair<int,int> key)
{
    lastCreatedRegion = new Region(key.first,key.second);
    loadingRegions = false;
}

int getRegion(int a) {
    return floor(a / float(regionSize) + .5);
}


void confirmRegion(int RX, int RY) {
    pair<int, int> key = make_pair(RX, RY);
    if ((find(regionsToMake.begin(),regionsToMake.end(),key) == regionsToMake.end())) {
        regionsToMake.emplace_back(key);
    }
}

std::pair<int, int> getRegion(int x, int y) {
    int RX = floor(x / float(regionSize) + .5);
    int RY = floor(y / float(regionSize) + .5);
    return std::make_pair(RX, RY);
}

std::pair<int, int> getRegion(vec v) {
    int RX = floor(v.x / float(regionSize) + .5);
    int RY = floor(v.y / float(regionSize) + .5);
    return std::make_pair(RX, RY);
}


#include "GameFunction.h"
extern GameFunction gfRender;

const int IMAGE_WIDTH  = regionSize;
const int IMAGE_HEIGHT = regionSize;
const int DATA_SIZE = IMAGE_WIDTH * IMAGE_HEIGHT * 4;
const GLenum PIXEL_FORMAT = GL_BGRA;


// debug has very slow terrain generation,
// this will speed it up
#ifdef _DEBUG
const int blockSize = 64;
const int minBlockSize = 16;
#else
const int blockSize = 32;
const int minBlockSize = 1;
#endif

int nodeGrid = 6;



Region::Region(int RX, int RY)
{
    std::srand(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
    this->RX = RX;
    this->RY = RY;
    x = (RX*regionSize) - regionSize / 2;
    y = (RY*regionSize) - regionSize / 2;

    



    if (!inMainMenu) {

        // place perlin points
        int minpx, minpy, maxpx, maxpy;
        minpx = x / perlinDist - 1;
        minpy = y / perlinDist - 1;
        maxpx = (x + regionSize) / perlinDist + 1;
        maxpy = (y + regionSize) / perlinDist + 1;
        // ensure all needed perlin grid points exist
        for (int px = minpx; px <= maxpx; px++) {
            for (int py = minpy; py <= maxpy; py++) {
                if (perlinGrid.find(std::make_pair(px, py)) == perlinGrid.end())
                    perlinGrid[std::make_pair(px, py)] = vec(randnum(0, 1), 0).rotated(randnum(0, 360)); // 360
            }
        }
        // repeat for biomes
        // place perlin points
        int minpbx, minpby, maxpbx, maxpby;
        minpbx = x / perlinDistBiome - 1;
        minpby = y / perlinDistBiome - 1;
        maxpbx = (x + regionSize) / perlinDistBiome + 1;
        maxpby = (y + regionSize) / perlinDistBiome + 1;
        // ensure all needed perlin grid points exist
        for (int pbx = minpbx; pbx <= maxpbx; pbx++) {
            for (int pby = minpby; pby <= maxpby; pby++) {
                if (perlinGridBiome.find(std::make_pair(pbx, pby)) == perlinGridBiome.end()) {
                    perlinGridBiome[std::make_pair(pbx, pby)] = vec(randnum(0, 1), 0).rotated(randnum(0, 360)); // 360
                }
            }
        }


        //pixels = new uint8_t[regionSize*regionSize * 4];

        // if region file exists, load it
        texture.create(regionSize, regionSize);
        // else
        {
            std::vector<cell> cells;
            for (int x = 0; x < regionSize; x += blockSize) {
                for (int y = 0; y < regionSize; y += blockSize) {
                    cells.emplace_back(cell(x, y, blockSize));
                }
            }
            for (int cellnum = 0; cellnum < cells.size(); cellnum++) {
                cell c = cells[cellnum];
                int worldX = x + c.x;
                int worldY = y + c.y;
                int terrainTL = getPerlinTerrain(worldX, worldY);
                int terrainTR = getPerlinTerrain(worldX + c.size, worldY);
                int terrainBL = getPerlinTerrain(worldX, worldY + c.size);
                int terrainBR = getPerlinTerrain(worldX + c.size, worldY + c.size);
                if (c.size > minBlockSize &&
                    (terrainTL != terrainTR ||
                     terrainTR != terrainBL ||
                     terrainBL != terrainBR)) {
                    int childSize = c.size / 2;
                    cells.emplace_back(cell(c.x, c.y, childSize));
                    cells.emplace_back(cell(c.x + childSize, c.y, childSize));
                    cells.emplace_back(cell(c.x, c.y + childSize, childSize));
                    cells.emplace_back(cell(c.x + childSize, c.y + childSize, childSize));
                }
                else {
                    for (int i = 0; i < c.size; i++) {
                        for (int j = 0; j < c.size; j++) {
                            if (terrainTL == air) {
                                pixels[((c.x + i) + (c.y + j)*regionSize) * 4 + 0] = 0;
                                pixels[((c.x + i) + (c.y + j)*regionSize) * 4 + 1] = 0;
                                pixels[((c.x + i) + (c.y + j)*regionSize) * 4 + 2] = 0;
                                pixels[((c.x + i) + (c.y + j)*regionSize) * 4 + 3] = 0;
                            }
                            else if (terrainTL == stone) {
                                pixels[((c.x + i) + (c.y + j)*regionSize) * 4 + 0] = 60;
                                pixels[((c.x + i) + (c.y + j)*regionSize) * 4 + 1] = 60;
                                pixels[((c.x + i) + (c.y + j)*regionSize) * 4 + 2] = 60;
                                pixels[((c.x + i) + (c.y + j)*regionSize) * 4 + 3] = 255;
                            }
                            else /*if (terrainTL == dirt)*/ {
                                pixels[((c.x + i) + (c.y + j)*regionSize) * 4 + 0] = 69;
                                pixels[((c.x + i) + (c.y + j)*regionSize) * 4 + 1] = 36;
                                pixels[((c.x + i) + (c.y + j)*regionSize) * 4 + 2] = 12;
                                pixels[((c.x + i) + (c.y + j)*regionSize) * 4 + 3] = 255;
                            }
                        }
                    }
                }
            }
            texture.update(pixels);


        }
        sprite.setTexture(texture);
        sprite.setPosition(x, y);

        textureUpdated = true;
        
    }
    else textureUpdated = true; // probably necessary


}

void Region::spawnEnemies()
{
    for (int i = 0; i < 1; i++) {
        if (randint(0, 10) < 3) {
            vec enemyPos(x + randint(0, regionSize),
                         y + randint(0, regionSize));
            if (!checkTerrainCircle(enemyPos, 50)) {
                new Enemy(enemyPos);
            }
        }
    }
}

void Region::placeNodes()
{
    // place pathfinding nodes
    for (int nx = 0; nx < nodeGrid; nx++) {
        for (int ny = 0; ny < nodeGrid; ny++) {
            nodes[make_pair(nx, ny)] = new Node(x + nx*(regionSize / nodeGrid),
                                                y + ny*(regionSize / nodeGrid), this);
        }
    }
    // link nodes as neighbors
    for (int nx = 0; nx < nodeGrid; nx++) {
        for (int ny = 0; ny < nodeGrid; ny++) {
            Node* n1 = nodes[make_pair(nx, ny)];
            for (int a = -1; a <= 1; a++) {
                for (int b = -1; b <= 1; b++) {
                    // exclude (0,0)
                    if (a != 0 || b != 0) {
                        Node* n2;
                        int x2 = nx + a;
                        int y2 = ny + b;
                        int regX = RX;
                        int regY = RY;
                        if (x2 < 0) {
                            regX--;
                            x2 = nodeGrid - 1;
                        }
                        else if (x2 == nodeGrid) {
                            regX++;
                            x2 = 0;
                        }
                        if (y2 < 0) {
                            regY--;
                            y2 = nodeGrid - 1;
                        }
                        else if (y2 == nodeGrid) {
                            regY++;
                            y2 = 0;
                        }
                        // uh oh... run this part in the main thread
                        // so we dont modify a regions nodes while theyre being accessed
                        pair<int, int> rKey = make_pair(regX, regY);
                        if (regX == RX && regY == RY) n2 = nodes[make_pair(x2, y2)];
                        else if (keyInMap(rKey, regions)) n2 = regions[rKey]->nodes[make_pair(x2, y2)];
                        else continue;
                        if (!(itemInVector(n1, n2->neighbors))) n2->neighbors.emplace_back(n1);
                        if (!(itemInVector(n2, n1->neighbors))) n1->neighbors.emplace_back(n2);
                    }
                }
            }
        }
    }
}

void Region::validateNodes() {
    // sever validNeighbor connections
    if (textureUpdated) {
        for (int nx = 0; nx < nodeGrid; nx++) {
            for (int ny = 0; ny < nodeGrid; ny++) {
                Node* n1 = nodes[make_pair(nx, ny)];
                //if (n1 == nullptr) say("Fuck!");
                for (int i = 0; i < n1->validNeighbors.size(); i++) {
                    Node* n2 = n1->validNeighbors[i];
                    for (int j = 0; j < n2->validNeighbors.size(); j++) {
                        Node* n3 = n2->validNeighbors[j];
                        if (n3 == n1) {
                            n2->validNeighbors.erase(n2->validNeighbors.begin() + j);
                            //say("Im not crazy");
                            break;
                        }
                    }
                }
                n1->validNeighbors.clear();
            }
        }

        // check for valid nodes
        for (int nx = 0; nx < nodeGrid; nx++) {
            for (int ny = 0; ny < nodeGrid; ny++) {
                Node* n1 = nodes[make_pair(nx, ny)];
                for (int i = 0; i < n1->neighbors.size(); i++) {
                    Node* n2 = n1->neighbors[i];
                    vec pos1 = vec(n1->x, n1->y);
                    vec pos2 = vec(n2->x, n2->y);
                    vec toPos2 = (pos2 - pos1).normalized(20);
                    bool isValid = true;
                    while (isValid && (pos2 - pos1).getLength() > 20) {
                        if (checkTerrain(pos1)) isValid = false;
                        else pos1 += toPos2;
                    }
                    if (isValid) {
                        if (!(itemInVector(n1, n2->validNeighbors))) n2->validNeighbors.emplace_back(n1);
                        if (!(itemInVector(n2, n1->validNeighbors))) n1->validNeighbors.emplace_back(n2);
                    }
                }
            }
        }
        nodesValidated = true;
    }
}

void Region::render()
{

    
    //timer t3;
    window->draw(sprite);
    //say("Update/Render: " << t3.elapsed());
}



void Region::genTerrain()
{
    int blockSize = 32;
    std::vector<cell> cells;
    for (int x = 0; x < regionSize; x += blockSize) {
        for (int y = 0; y < regionSize; y += blockSize) {
            cells.emplace_back(cell(x, y, blockSize));
        }
    }
    for (int cellnum = 0; cellnum < cells.size(); cellnum++) {
        cell c = cells[cellnum];
        int worldX = x + c.x;
        int worldY = y + c.y;
        int terrainTL = getPerlinTerrain(worldX, worldY);
        int terrainTR = getPerlinTerrain(worldX + c.size, worldY);
        int terrainBL = getPerlinTerrain(worldX, worldY + c.size);
        int terrainBR = getPerlinTerrain(worldX + c.size, worldY + c.size);
        if (c.size > 1 &&
            (terrainTL != terrainTR ||
             terrainTR != terrainBL ||
             terrainBL != terrainBR)) {
            int childSize = c.size / 2;
            cells.emplace_back(cell(c.x, c.y, childSize));
            cells.emplace_back(cell(c.x + childSize, c.y, childSize));
            cells.emplace_back(cell(c.x, c.y + childSize, childSize));
            cells.emplace_back(cell(c.x + childSize, c.y + childSize, childSize));
        }
        else {
            for (int i = 0; i < c.size; i++) {
                for (int j = 0; j < c.size; j++) {
                    if (terrainTL == air) {
                        pixels[((c.x + i) + (c.y + j)*regionSize) * 4 + 0] = 0;
                        pixels[((c.x + i) + (c.y + j)*regionSize) * 4 + 1] = 0;
                        pixels[((c.x + i) + (c.y + j)*regionSize) * 4 + 2] = 0;
                        pixels[((c.x + i) + (c.y + j)*regionSize) * 4 + 3] = 0;
                    }
                    else if (terrainTL == stone) {
                        pixels[((c.x + i) + (c.y + j)*regionSize) * 4 + 0] = 80;
                        pixels[((c.x + i) + (c.y + j)*regionSize) * 4 + 1] = 80;
                        pixels[((c.x + i) + (c.y + j)*regionSize) * 4 + 2] = 80;
                        pixels[((c.x + i) + (c.y + j)*regionSize) * 4 + 3] = 255;
                    }
                    else /*if (terrainTL == dirt)*/ {
                        pixels[((c.x + i) + (c.y + j)*regionSize) * 4 + 0] = 100;
                        pixels[((c.x + i) + (c.y + j)*regionSize) * 4 + 1] = 45;
                        pixels[((c.x + i) + (c.y + j)*regionSize) * 4 + 2] = 20;
                        pixels[((c.x + i) + (c.y + j)*regionSize) * 4 + 3] = 255;
                    }
                }
            }
        }
    }
    //texture.update(pixels);
    textureUpdated = true;
}

void Region::renderDebug()
{
    for (auto it = nodes.begin(); it != nodes.end(); it++) {
        new Dot(vec(it->second->x, it->second->y), Color::Green);
        for (int i = 0; i < it->second->neighbors.size(); i++) {
            Node* n2 = it->second->neighbors[i];
            vec pos1 = vec(it->second->x, it->second->y);
            vec pos2 = vec(n2->x, n2->y);
            //cout << "1: "; pos1.print();
            //cout << "2: "; pos2.print();
            new Line(pos1,
                     pos2);
        }
        for (int i = 0; i < it->second->validNeighbors.size(); i++) {
            Node* n2 = it->second->validNeighbors[i];
            vec pos1 = vec(it->second->x, it->second->y);
            vec pos2 = vec(n2->x, n2->y);
            //cout << "1: "; pos1.print();
            //cout << "2: "; pos2.print();
            new Line(pos1,
                     pos2, 3, sf::Color::Green);
        }
    }
    disp_rect.setFillColor(Color(0, 0, 0, 0));
    disp_rect.setOutlineColor(sf::Color(255, 255, 255, 128));
    disp_rect.setOutlineThickness(-2);
    disp_rect.setPosition(x, y);
    disp_rect.setSize(Vector2f(regionSize, regionSize));
    window->draw(disp_rect);
}

Region::~Region()
{
    // sever connections with other regions
    for (auto it = nodes.begin(); it != nodes.end(); it++) {
        Node* n1 = it->second;
        for (int i = 0; i < n1->neighbors.size(); i++) {
            Node* n2 = n1->neighbors[i];
            // neighbors
            for (int j = 0; j < n2->neighbors.size(); j++) {
                if (n2->neighbors[j] == n1) {
                    n2->neighbors.erase(n2->neighbors.begin() + j);
                    break;
                }
            }
            // validNeighbors
            for (int j = 0; j < n2->validNeighbors.size(); j++) {
                if (n2->validNeighbors[j] == n1) {
                    n2->validNeighbors.erase(n2->validNeighbors.begin() + j);
                    break;
                }
            }
        }
    }
    for (auto it = nodes.begin(); it != nodes.end(); it++) {
        delete it->second;
    }
    nodes.clear();

    for (int i = containedObjects.size()-1; i >=0; i--) {
        GameObject* obj = containedObjects[i];
        //if (obj == nullptr) say("Shitting Fuck.");
        for (int j = obj->inRegions.size()-1; j >=0 ; j--) {
            if (obj->inRegions[j] == this) {
                obj->inRegions.erase(obj->inRegions.begin() + j);
            }
        }
        delete obj;
    }
}
