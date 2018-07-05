#include "BintrsTools.h"
#include <iostream>
#include <random>

vector<Dot*> dots;
vector<Line*> lines;
std::default_random_engine de;

float sineEase(float t, float b, float c, float d)
{
    return -c / 2 * (cos(PI*t / d) - 1) + b;
}

void constrainAngles(float* d1, float* d2) {
    while (*d1 < *d2 - 180) *d1 += 360;
    while (*d1 > *d2 + 180) *d1 -= 360;
}

void normalizeAngle(float* ang) {
    while (*ang > 360) *ang -= 360;
    while (*ang < 0) *ang += 360;
}

float randgauss(float mean, float stddev)
{
    std::normal_distribution<float> nd(mean, stddev); //mean followed by stdiv
    return nd(de);
}

int randint(int min, int max)
{
    return rand() % (max + 1 - min) + min;
}

float randnum(float min, float max)
{
    return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}


double randtriangular(double min, double max, double avg) {
    double U = rand() / (double)RAND_MAX;
    double F = (avg - min) / (max - min);
    if (U <= F)
        return min + sqrt(U * (max - min) * (avg - min));
    else
        return max - sqrt((1 - U) * (max - min) * (max - avg));
}



timer::timer()
{
    reset();
}

void timer::reset()
{
    start = std::chrono::high_resolution_clock::now();
}
double timer::elapsed()    //returns elapsed time in seconds
{
    return (std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start).count());
}

#define PI 3.14159265359
vec::vec()
{
    x = 0;
    y = 0;
}
vec::vec(float x, float y)
{
    this->x = x;
    this->y = y;
}

vec vec::rotate(float deg)
{
    //rotate around idealAngle
    float rad = deg*PI / 180;
    float x_result;
    float y_result;
    //cout<<"D: "<<deg<< "R: "<<rad<<endl;
    x_result = x * cos(rad) - y * sin(rad);
    y_result = x * sin(rad) + y * cos(rad);
    x = x_result;
    y = y_result;
    return vec(x, y);
}
vec vec::rotated(float deg)
{
    //rotate around idealAngle
    float rad = deg*PI / 180;
    float x_result;
    float y_result;
    //cout<<"D: "<<deg<< "R: "<<rad<<endl;
    x_result = x * cos(rad) - y * sin(rad);
    y_result = x * sin(rad) + y * cos(rad);
    return vec(x_result, y_result);
}
void vec::rotateAround(vec v, float deg)
{
    //// might want to invert idealAngle here, not sure
    ////rotate around idealAngle
    //float rad = deg*PI / 180;
    //float xdiff = v.x - x;
    //float ydiff = v.y - y;
    //float x_result;
    //float y_result;
    ////cout<<"D: "<<deg<< "R: "<<rad<<endl;
    //x_result = xdiff * cos(rad) - ydiff * sin(rad);
    //y_result = xdiff * sin(rad) + ydiff * cos(rad);
    //x+= x_result;
    //y+= y_result;
    float rad = deg*PI / 180;
    float s = sin(rad);
    float c = cos(rad);

    // translate point back to origin:
    x -= v.x;
    y -= v.y;

    // rotate point
    float xnew = x * c - y * s;
    float ynew = x * s + y * c;


    // translate point back:
    x = xnew + v.x;
    y = ynew + v.y;
}
void vec::flipAround(vec v)
{
    x = (v.x - x) + v.x;
}
vec vec::flipx()
{
    return vec(-x, y);
}
vec vec::flipy()
{
    return vec(x, -y);
}

void vec::norm(float mag)
{
    if (x == 0 && y == 0)
    {
        return; // prevent division by zero
    }
    float len = sqrt(x*x + y*y);
    x /= len;
    y /= len;
    x *= mag;
    y *= mag;
}

vec vec::normalized(float mag)
{
    if (x == 0 && y == 0)
    {
        return vec(0, 0); // prevent division by zero
    }
    float len = sqrt(x*x + y*y);
    x /= len;
    y /= len;
    x *= mag;
    y *= mag;
    return vec(x, y);
}

float vec::getLength()
{
    return sqrt(x*x + y*y);
}
float vec::getAngle()
{
    float rad = atan2(y, x);
    float deg = rad * 180 / PI;
    while (deg < 0) deg += 360;
    return deg;
}
vec vec::operator + (vec v)
{
    float rx = x + v.x;
    float ry = y + v.y;
    return vec(rx, ry);
}

vec vec::operator-(vec v)
{
    float rx = x - v.x;
    float ry = y - v.y;
    return vec(rx, ry);
}

vec vec::operator * (float f)
{
    float rx = x * f;
    float ry = y * f;
    return vec(rx, ry);
}

vec vec::operator*=(float f)
{
    x *= f;
    y *= f;
    return vec(x, y);
}

bool vec::operator==(vec v)
{
    if (x == v.x &&
        y == v.y)
        return true;
    return false;
}

bool vec::operator!=(vec v)
{
    if (x != v.x ||
        y != v.y)
        return true;
    return false;
}

vec vec::operator+=(vec v)
{
    x += v.x;
    y += v.y;
    return vec(x, y);
}

vec vec::operator-=(vec v)
{
    x -= v.x;
    y -= v.y;
    return vec(x, y);
}

float vec::operator*(vec v)
{
    //A*B = Ax*Bx + Ay*By.
    return v.x*x + v.y*y;
}

void vec::print()
{
    std::cout << "X: " << x << " \tY: " << y << std::endl;
}


vec randVec(float minLength, float maxLength)
{
    return vec(randnum(minLength,maxLength),0).rotated(randnum(0,360));
}

//uint8_t* convertTo1D(uint8_t** arr2D, int w, int h) {
//    uint8_t* pixels = new uint8_t[w*h * 4];
//    for (int x = 0; x < w; x++) {
//        for (int y = 0; y < h * 4; y += 4) {
//            int i = y*w + x * 4;
//            pixels[i + 0] = arr2D[x][y + 0];
//            pixels[i + 1] = arr2D[x][y + 1];
//            pixels[i + 2] = arr2D[x][y + 2];
//            pixels[i + 3] = arr2D[x][y + 3];
//        }
//    }
//    return pixels;
//}
//
//std::vector<Line*> lines;
//std::vector<Dot*> dots;
//
sf::Color hsvtorgb(float h, float s, float v, float a)
{
    double r = 0, g = 0, b = 0;
    if (s == 0)
    {
        r = v;
        g = v;
        b = v;
    }
    else
    {
        int i;
        double f, p, q, t;

        if (h == 360)
            h = 0;
        else
            h = h / 60;

        i = (int)trunc(h);
        f = h - i;

        p = v * (1.0 - s);
        q = v * (1.0 - (s * f));
        t = v * (1.0 - (s * (1.0 - f)));

        switch (i)
        {
        case 0:
            r = v;
            g = t;
            b = p;
            break;

        case 1:
            r = q;
            g = v;
            b = p;
            break;

        case 2:
            r = p;
            g = v;
            b = t;
            break;

        case 3:
            r = p;
            g = q;
            b = v;
            break;

        case 4:
            r = t;
            g = p;
            b = v;
            break;

        default:
            r = v;
            g = p;
            b = q;
            break;
        }
    }
    return sf::Color(r * 255, g * 255, b * 255, a * 255);
}
//
//vector<BintrSound*> activeSounds;
//
//#include "Player.h"
//extern Player* p;
//
//void playSound(sf::SoundBuffer* soundBuffer, vec* worldNozzle,
//               int volume, float pitch,
//               int minDist, int attenuation,
//               float pitchVel)
//{
//    //if (sendToServer && Multiplayer) {
//    //    sf::Packet soundPacket;
//    //    soundPacket << play_sound
//    //        << bintrSound->soundID
//    //        << x
//    //        << y
//    //        << volume;
//    //    new ConfirmPacket(soundPacket);
//    //}
//    if (!worldNozzle) worldNozzle = &p->worldNozzle;
//    if (soundBuffer) {
//        activeSounds.emplace_back(new BintrSound(worldNozzle, pitchVel));
//        sf::Sound* sound = &(activeSounds[activeSounds.size() - 1]->sound);
//        sound->setVolume(volume);
//        sound->setPosition(worldNozzle->x, worldNozzle->y, -500);
//        sound->setMinDistance(minDist);
//        sound->setAttenuation(attenuation);
//        sound->setPitch(pitch*physRate / 60);
//        sound->setBuffer(*soundBuffer);
//        sound->play();
//    }
//}
//
//
//void playSound(sf::SoundBuffer* soundBuffer, GameObject* obj,
//               int volume, float pitch,
//               int minDist, int attenuation,
//               float pitchVel)
//{
//    //if (sendToServer && Multiplayer) {
//    //    sf::Packet soundPacket;
//    //    soundPacket << play_sound
//    //        << bintrSound->soundID
//    //        << x
//    //        << y
//    //        << volume;
//    //    new ConfirmPacket(soundPacket);
//    //}
//    //if (worldNozzle == nullptr) worldNozzle = &p->worldNozzle;
//    if (soundBuffer) {
//        BintrSound* bSound = new BintrSound(obj, pitchVel);
//        obj->linkedSounds.emplace_back(bSound);
//        activeSounds.emplace_back(bSound);
//        sf::Sound* sound = &(activeSounds[activeSounds.size() - 1]->sound);
//        sound->setVolume(volume);
//        sound->setPosition(obj->worldNozzle.x, obj->worldNozzle.y, -500);
//        sound->setMinDistance(minDist);
//        sound->setAttenuation(attenuation);
//        sound->setPitch(pitch*physRate / 60);
//        sound->setBuffer(*soundBuffer);
//        sound->play();
//    }
//}
//
//void playSound(sf::SoundBuffer * soundBuffer, int volume, float pitch, int minDist, int attenuation, float pitchVel)
//{
//    playSound(soundBuffer, &p->worldNozzle, volume, pitch, minDist, attenuation, pitchVel);
//}
//


vector<BintrSound*> activeSounds;
extern float physRate;

#include "Enemy.h"

void playSound(sf::SoundBuffer * soundBuffer, vec pos, int volume, float pitch, int minDist, int attenuation, float pitchVel)
{
    if (soundBuffer) {
        float numSamples = soundBuffer->getSampleCount();
        const sf::Int16* samples = soundBuffer->getSamples();
        float avgVolume = 0;
        int volChecks = 20;
        for (int i = 0; i < volChecks; i++) {
            avgVolume += (abs(samples[int(numSamples / i)]) * volume * (1.f / volChecks));
        }
        avgVolume = sqrt(avgVolume);
        //say("AvgVol: " << avgVolume);
        makeNoise(avgVolume*1.5, pos);
        BintrSound* bSound = new BintrSound(pos, pitchVel);
        activeSounds.emplace_back(bSound);
        sf::Sound* sound = &bSound->sound;
        sound->setVolume(volume);
        sound->setPosition(pos.x, pos.y, -500);
        sound->setMinDistance(minDist);
        sound->setAttenuation(attenuation);
        sound->setPitch(pitch*physRate / 60);
        sound->setBuffer(*soundBuffer);
        sound->play();
    }
}

int getTerrainType(uint8_t * sample)
{
    if (sample[0] == 0 &&
        sample[1] == 0 &&
        sample[2] == 0 &&
        sample[3] == 0) {
        return air;
    }
    else if (sample[0] == 60 &&
             sample[1] == 60 &&
             sample[2] == 60 &&
             sample[3] == 255) {
        return stone;
    }
    else if (sample[0] == 69 &&
             sample[1] == 36 &&
             sample[2] == 12 &&
             sample[3] == 255) {
        return dirt;
    }
}
//
#include "Region.h"
// Samples a point for terrain
// -true if contains terrain
// -false if open air
// ???? if region not generated
extern bool debugDraw;

bool checkTerrain(vec pos)
{
    pair<int, int> key = getRegion(pos.x, pos.y);
    sf::Color color;
    color = sf::Color::Red;
    //if (debugDraw) new Dot(worldNozzle, color);

    if (keyInMap(key,regions)) {
        Region* reg = regions[key];
        int x = pos.x - reg->x;
        int y = pos.y - reg->y;
        int i = x * 4 + y*regionSize * 4;
        if (getTerrainType(&reg->pixels[i]) == air)
            return false;
        return true;
    }
    // else, point is open air
    return false;
}

bool checkTerrainCircle(vec pos, int radius)
{
    pair<int, int> rTL = getRegion(pos.x - radius, pos.y - radius);
    pair<int, int> rBR = getRegion(pos.x + radius, pos.y + radius);
    for (int rx = rTL.first; rx <= rBR.first; rx++) {
        for (int ry = rTL.second; ry <= rBR.second; ry++) {
            Region* reg = nullptr;
            pair<int, int> key = make_pair(rx, ry);
            if (keyInMap(key,regions)) {
                reg = regions[key];
                for (int circy = -radius; circy <= radius; circy += 1) {
                    for (int circx = -radius; circx <= radius; circx += 1) {
                        int worldx = floor(pos.x) + circx;
                        int worldy = floor(pos.y) + circy;
                        if (worldx >= reg->x && worldx < reg->x + regionSize &&
                            worldy >= reg->y && worldy < reg->y + regionSize) {
                            int x = pos.x + circx - reg->x;
                            int y = pos.y + circy - reg->y;
                            int i = x * 4 + y*regionSize * 4;;
                            if (getTerrainType(&reg->pixels[i]) != air) return true;
                        }
                    }
                }
            }
            //else return true; // there might be terrain in the undefined region?
        }
    }
    return false;
}
//
float getTerrainNormal(vec v, int samples)
{
    vec total = vec(0, 0);
    for (int i = 0; i < samples; i++) {
        vec displacement = vec(32, 0).rotated(i * 360 / samples);
        vec sample = v + displacement;
        //sf::Color col = sf::Color::Green;
        if (checkTerrain(sample)) {
            total -= displacement;
            //col = sf::Color::Red;
        }
        //new Dot(sample, col);
    }
    float angle = total.getAngle();
    if (abs(total.x) + abs(total.y) < 1) angle = 270;
    //new Dot(v + vec(10, 0).rotated(idealAngle), 10, sf::Color::Green);
    return angle;
}
//
sf::Color interpColor(sf::Color c1, sf::Color c2, float percent)
{
    uint8_t r, g, b, a;
    r = c2.r + (c1.r - c2.r) * percent;
    g = c2.g + (c1.g - c2.g) * percent;
    b = c2.b + (c1.b - c2.b) * percent;
    a = c2.a + (c1.a - c2.a) * percent;
    return sf::Color(r, g, b, a);
}
//
bool pointRectCollision(float PX, float PY, float RX, float RY, float RW, float RH)
{
    if (RX < PX && PX < RX + RW &&
        RY < PY && PY < RY + RH) {
        return true;
    }
    return false;
}
//
void clamp(float * valPointer, float min, float max)
{
    if (*valPointer < min) *valPointer = min;
    else if (*valPointer > max) *valPointer = max;
}
//

void replacePixel(sf::Image * imageP, int x, int y)
{
    int r = 0; int g = 0; int b = 0; int a = 0;
    Color lC = imageP->getPixel(x - 1, y);
    Color rC = imageP->getPixel(x + 1, y);
    Color uC = imageP->getPixel(x, y - 1);
    Color dC = imageP->getPixel(x, y + 1);
    r = (lC.r + rC.r + uC.r + dC.r) / 4;
    g = (lC.g + rC.g + uC.g + dC.g) / 4;
    b = (lC.b + rC.b + uC.b + dC.b) / 4;
    a = (lC.a + rC.a + uC.a + dC.a) / 4;

    imageP->setPixel(x, y, Color(r, g, b, a));
}
//
#include "Particle.h"
//

void destroyTerrainCircle(vec pos, int radius)
{
    int particleSize = radius*4;
    int totalDirt = 0;
    int totalStone = 0;
    pair<int, int> rTL = getRegion(pos.x - radius, pos.y - radius);
    pair<int, int> rBR = getRegion(pos.x + radius, pos.y + radius);
    for (int rx = rTL.first; rx <= rBR.first; rx++) {
        for (int ry = rTL.second; ry <= rBR.second; ry++) {
            Region* reg = nullptr;
            if (regions.find(make_pair(rx, ry)) != regions.end()) {
                int rSize = regions.size();
                reg = regions[make_pair(rx, ry)];
                if (regions.size() > rSize) say("What the fucking shit");
            }
            if (reg) {
                reg->nodesValidated = false;
                reg->textureUpdated = false; // mark to update texture later
                // destroying - factor material strength
                //if (drawTerrain == air) {
                for (int circy = -radius; circy <= radius; circy += 1) {
                    for (int circx = -radius; circx <= radius; circx += 1) {
                        int worldx = floor(pos.x) + circx;
                        int worldy = floor(pos.y) + circy;
                        if (worldx >= reg->x && worldx < reg->x + regionSize &&
                            worldy >= reg->y && worldy < reg->y + regionSize) {

                            int x = pos.x + circx - reg->x;
                            int y = pos.y + circy - reg->y;
                            int i = x * 4 + y*regionSize * 4;
                            float strength = 1;
                            int tType = getTerrainType(&reg->pixels[i]);
                            if (tType == stone) strength = 1.2;
                            else if (tType == dirt) strength = 1;
                            if (circx*circx + circy*circy <= (radius / strength)*(radius / strength)) {
                                reg->pixels[i + 0] = 0;
                                reg->pixels[i + 1] = 0;
                                reg->pixels[i + 2] = 0;
                                reg->pixels[i + 3] = 0;
                                if (tType == dirt) {
                                    totalDirt++;
                                    if (totalDirt  > particleSize && randint(1, particleSize) == 1) {
                                        float r = sqrt(float(totalDirt) / PI);
                                        Particle* p = new Particle(pos + vec(circx, circy), randVec(0, 20), randint(60, 120), r, r, Color(69, 36, 12), Color(69, 36, 12, 0), 0.5, 0.95);
                                        p->stickToTerrain = true;
                                        totalDirt -= particleSize;
                                    }
                                }
                                else if (tType == stone) {
                                    totalStone++;
                                    if (totalStone > particleSize && randint(1, particleSize) == 1) {
                                        float r = sqrt(float(totalStone) / PI);
                                        Particle* p = new Particle(pos + vec(circx, circy), randVec(0, 20), randint(60, 120), r, r, Color(60, 60, 60), Color(60, 60, 60, 0), 0.5, 0.95);
                                        p->stickToTerrain = true;
                                        totalStone -= particleSize;
                                    }
                                }
                            }
                        }
                    }
                }
                //}
                //// placing - only on open space
                //else {
                //    for (int circy = -radius; circy <= radius; circy += 1) {
                //        for (int circx = -radius; circx <= radius; circx += 1) {
                //            int worldx = globalMouse.x + circx;
                //            int worldy = globalMouse.y + circy;
                //            if (circx*circx + circy*circy <= radius*radius) {
                //                if (worldx >= reg->x && worldx < reg->x + regionSize &&
                //                    worldy >= reg->y && worldy < reg->y + regionSize) {
                //                    int x = globalMouse.x + circx - reg->x;
                //                    int y = globalMouse.y + circy - reg->y;
                //                    int i = x * 4 + y*regionSize * 4;
                //                    // only draw if the space is air
                //                    if (getTerrainType(&reg->pixels[i]) == air) {
                //                        reg->pixels[i + 0] = tR;
                //                        reg->pixels[i + 1] = tG;
                //                        reg->pixels[i + 2] = tB;
                //                        reg->pixels[i + 3] = tA;
                //                    }
                //                }
                //            }
                //        }
                //    }
                //}
                //reg->texture.update(reg->pixels, regionSize, regionSize, 0, 0);
                if (!reg->updating && !(itemInVector(reg,regionsToUpdate))) regionsToUpdate.emplace_back(reg);
            }
        }
    }
    
}

void placeTerrainCircle(vec pos, int radius, int type)
{
    uint8_t tR, tG, tB, tA;
    if (type == air) {
        tR = 0;
        tG = 0;
        tB = 0;
        tA = 0;
    }
    else if (type == stone) {
        tR = 60;
        tG = 60;
        tB = 60;
        tA = 255;
    }
    else if (type == dirt) {
        tR = 69;
        tG = 36;
        tB = 12;
        tA = 255;
    }

    pair<int, int> rTL = getRegion(pos.x - radius, pos.y - radius);
    pair<int, int> rBR = getRegion(pos.x + radius, pos.y + radius);
    for (int rx = rTL.first; rx <= rBR.first; rx++) {
        for (int ry = rTL.second; ry <= rBR.second; ry++) {
            Region* reg = nullptr;
            if (regions.find(make_pair(rx, ry)) != regions.end()) {
                int rSize = regions.size();
                reg = regions[make_pair(rx, ry)];
                //if (regions.size() > rSize) say("What the fucking shit");
            }
            if (reg && reg->textureUpdated) {
                reg->nodesValidated = false;
                // destroying - factor material strength
                //if (drawTerrain == air) {
                for (int circy = -radius; circy <= radius; circy += 1) {
                    for (int circx = -radius; circx <= radius; circx += 1) {
                        int worldx = floor(pos.x) + circx;
                        int worldy = floor(pos.y) + circy;
                        if (worldx >= reg->x && worldx < reg->x + regionSize &&
                            worldy >= reg->y && worldy < reg->y + regionSize) {

                            int x = pos.x + circx - reg->x;
                            int y = pos.y + circy - reg->y;
                            int i = x * 4 + y*regionSize * 4;
                            // only draw if the space is air
                            if (circx*circx + circy*circy <= radius *radius ) {
                                if (getTerrainType(&reg->pixels[i]) == air) {
                                    reg->pixels[i + 0] = tR;
                                    reg->pixels[i + 1] = tG;
                                    reg->pixels[i + 2] = tB;
                                    reg->pixels[i + 3] = tA;
                                }
                            }
                        }
                    }
                }
                //}
                //// placing - only on open space
                //else {
                //    for (int circy = -radius; circy <= radius; circy += 1) {
                //        for (int circx = -radius; circx <= radius; circx += 1) {
                //            int worldx = globalMouse.x + circx;
                //            int worldy = globalMouse.y + circy;
                //            if (circx*circx + circy*circy <= radius*radius) {
                //                if (worldx >= reg->x && worldx < reg->x + regionSize &&
                //                    worldy >= reg->y && worldy < reg->y + regionSize) {
                //                    int x = globalMouse.x + circx - reg->x;
                //                    int y = globalMouse.y + circy - reg->y;
                //                    int i = x * 4 + y*regionSize * 4;
                //                    // only draw if the space is air
                //                    if (getTerrainType(&reg->pixels[i]) == air) {
                //                        reg->pixels[i + 0] = tR;
                //                        reg->pixels[i + 1] = tG;
                //                        reg->pixels[i + 2] = tB;
                //                        reg->pixels[i + 3] = tA;
                //                    }
                //                }
                //            }
                //        }
                //    }
                //}
                reg->texture.update(reg->pixels, regionSize, regionSize, 0, 0);
            }
        }
    }


}

#include "GameFunction.h"
extern GameFunction gfPhysics;
vec interpVec(vec vLast, vec vCurrent)
{
    float interp = ((gfPhysics.clock.elapsed())*(1.f / gfPhysics.rate));
    if (interp > 1) interp = 1;
    
    return vLast + (vCurrent - vLast) * interp;
}

float interpAng(float angLast, float angCurrent)
{
    float interp = ((gfPhysics.clock.elapsed())*(1.f / gfPhysics.rate));
    if (interp > 1) interp = 1;
    constrainAngles(&angLast, &angCurrent);
    return angLast + (angCurrent - angLast) * interp;
}

