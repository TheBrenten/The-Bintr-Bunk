#pragma once

#include <chrono>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Enums.h"
#define PI 3.14159265359
using namespace std;
using namespace sf;
#define say(x) std::cout<<x<<'\n'
#define randomChoice(vector) vector[randint(0,vector.size()-1)]
#define keyInMap(x,y) y.find(x)!=y.end()
extern sf::RenderWindow* window;
class Dot;
class Line;
extern vector<Dot*> dots;
extern vector<Line*> lines;


float sineEase(float t, float b, float c, float d);

void constrainAngles(float * d1, float * d2);

void normalizeAngle(float * ang);

float randgauss(float mean, float stddev);

int randint(int min, int max);

float randnum(float min, float max);

double randtriangular(double min, double max, double avg);


struct timer
{
    std::chrono::time_point<std::chrono::high_resolution_clock> start;

    timer();
    void reset();
    double elapsed();
};

struct rect {
    float x, y, w, h;
    rect() {
        this->x = 0;
        this->y = 0;
        this->w = 0;
        this->h = 0;
    }
    rect(float x, float y, float w, float h) {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }
};

struct vec
{
    // Stored Values (2D Vector)
    float x;
    float y;
    vec();
    vec(float x, float y);
    vec rotate(float deg);     // (+clockwise) (-counterclockwise)
    vec rotated(float deg);
    void rotateAround(vec v, float deg);
    void flipAround(vec v);
    vec flipx();               // mirror vector left/right
    vec flipy();               // mirror vector left/right
    void norm(float mult = 1);  // set to length 1, or 'mult'
    vec normalized(float mult = 1);
    float getLength();         // return distance from 0
    float getAngle();
    // Math Operators
    vec operator + (vec v);
    vec operator - (vec v);
    vec operator * (float f);  // scale
    vec operator += (vec v);
    vec operator -= (vec v);
    vec operator *= (float f);
    float operator * (vec v);  // dot product
                               // Logic Operators
    bool operator == (vec v);
    bool operator != (vec v);
    void print();              // output X/Y to console
};

vec randVec(float minLength, float maxLength);
#include <iostream>

//uint8_t* convertTo1D(uint8_t** arr2D, int w, int h);
class Dot;
extern std::vector<Dot*> dots;
class Dot {
public:
    sf::CircleShape disp_circ;
    int r;
    Dot(vec v, sf::Color color = sf::Color::Red, int r = 4) {
        dots.emplace_back(this);
        disp_circ.setFillColor(color);
        disp_circ.setPosition(v.x, v.y);
        this->r = r;
        disp_circ.setRadius(r);
        disp_circ.setOrigin(r, r);
    }
    void render() {
        window->draw(disp_circ);
        //std::cout << "drawing\n";
    }
};
class Line {
public:
    sf::RectangleShape disp_rect;
    Line(vec v1, vec v2, float thickness = 3,sf::Color color = sf::Color::Red) {
        lines.emplace_back(this);
        vec v3 = v2 - v1;
        int len = v3.getLength();
        disp_rect.setFillColor(color);
        disp_rect.setSize(sf::Vector2f(len, thickness));
        disp_rect.setOrigin(0, thickness / 2.f);
        disp_rect.setRotation(v3.getAngle());
        disp_rect.setPosition(v1.x, v1.y);
    }
    void render() {
        window->draw(disp_rect);
    }
};

sf::Color hsvtorgb(float h, float s, float v, float a = 1);

class BintrSound {
public:
    //GameObject* linkedObject = nullptr;
    vec pos;
    bool updatePos = true;
    sf::Sound sound;
    // weirdo pitch modification stuff for fun
    float pitchVel = 1;
    BintrSound(vec pos,float pitchVel = 1) {
        this->pos = pos;
        this->pitchVel = pitchVel;
    }
    //BintrSound(GameObject* linkedObject, float pitchVel = 1) {
    //    this->linkedObject = linkedObject;
    //    this->pitchVel = pitchVel;
    //}

};
//    
//
//extern std::vector<BintrSound*> activeSounds;
//

void playSound(sf::SoundBuffer* soundBuffer, 
               vec pos,
               int volume = 100, float pitch = 1,
               int minDist = 2000, int attenuation = 60,
               float pitchVel = 1);

//void playSound(sf::SoundBuffer* soundBuffer, vec* worldNozzle = nullptr,
//               int volume = 100, float pitch = 1,
//               int minDist = 1000, int attenuation = 60,
//               float pitchVel = 1);
//
//void playSound(sf::SoundBuffer* soundBuffer, GameObject* obj,
//               int volume = 100, float pitch = 1,
//               int minDist = 1000, int attenuation = 60,
//               float pitchVel = 1);
//
//void playSound(sf::SoundBuffer* soundBuffer, // no worldNozzle supplied
//               int volume = 100, float pitch = 1,
//               int minDist = 1000, int attenuation = 60,
//               float pitchVel = 1);
//
int getTerrainType(uint8_t* sample);
//
//
bool checkTerrain(vec pos);

// True if any sample points find terrain
bool checkTerrainCircle(vec pos, int radius);
//
float getTerrainNormal(vec v, int samples);
//
sf::Color interpColor(sf::Color c1, sf::Color c2, float percent);
//
bool pointRectCollision(float PX, float PY,
                        float RX, float RY, float RW, float RH);
//
void clamp(float* valPointer, float min, float max);



void replacePixel(sf::Image* imageP, int x, int y);

void destroyTerrainCircle(vec pos, int radius);

void placeTerrainCircle(vec pos, int radius, int type);

vec interpVec(vec vLast, vec vCurrent);

float interpAng(float angLast, float angCurrent);