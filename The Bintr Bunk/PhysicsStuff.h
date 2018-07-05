#pragma once
#include "BintrsTools.h"



class PhysPoint {
public:
    float friction = 0.999;
    float surfacefriction = 0.8;
    float gravity = 0.5;
    vec pos;
    vec posLast;
    vec vel;
    float mass = 1;
    void setPosition(vec pos) {
        this->pos = pos;
        this->posLast = pos;
    }
    void addFriction(float f) {
        posLast = pos + (pos - posLast)*f;
    }
    void move() {
        vec diff = pos - posLast;
        addFriction(friction);
        vel = (posLast - pos);
        posLast = pos;
        pos = pos + vel;
        pos.y += gravity;
    }
    void collide() {
        if (checkTerrain(pos)) {
            float speed = vel.getLength();
            vec toOutside = vec(1, 0).rotated(getTerrainNormal(pos, speed + 4));
            float normAng = toOutside.getAngle();
            while (checkTerrain(pos)) {
                pos += toOutside;
            }
            float revAng = vel.getAngle() + 180;
            float newAng = normAng + (normAng - revAng);
            vec correction(speed * surfacefriction, 0);
            posLast = pos - correction.rotated(newAng);
        }
    }
};

class Spring {
public:
    bool visible = true;
    sf::RectangleShape disp_rect;
    PhysPoint* p1;
    PhysPoint* p2;
    float minDist;
    float maxDist;
    float Strength;
    Spring(PhysPoint* p1, PhysPoint* p2,
           float Strength, float minDist = 0, float maxDist = 0)
    {
        //Springs.emplace_back(this);
        this->minDist = minDist;
        this->maxDist = maxDist;
        this->Strength = Strength;
        //else {
        //    minDist = (p1->worldNozzle - p2->worldNozzle).getLength();
        //}
        disp_rect.setFillColor(sf::Color::Green);
        disp_rect.setOrigin(2, 2);
        //disp_rect.setSize(sf::Vector2f(minDist + 4, 4));
        this->p1 = p1;
        this->p2 = p2;
    }
    void move()
    {
        float str = Strength;
        float mass1 = p1->mass;
        float mass2 = p2->mass;
        float totalMass = mass1 + mass2;
        vec line = p1->pos - p2->pos;
        float len = line.getLength();
        vec correct1;
        vec correct2;
        if (len > maxDist) {
            str *= (len - maxDist);
            line.norm(str);
            correct1 = line*(mass2 / totalMass);
            correct2 = line*(mass1 / totalMass);

            ///correct1 = vec(line).norm(((maxDist - len)*percent2));
            //correct2 = vec(line).norm(((maxDist - len)*Strength));

        }
        else if (len < minDist) {
            //Strength *= (len - minDist);
            str *= (len - minDist);
            line.norm(str);
            correct1 = line*(mass2 / totalMass);
            correct2 = line*(mass1 / totalMass);
        }
        p1->posLast += correct1;
        p2->posLast -= correct2;
    }
    void render(sf::RenderWindow * window)
    {
        if (visible) {
            float ang = (p2->pos - p1->pos).getAngle();
            disp_rect.setPosition(sf::Vector2f(p1->pos.x, p1->pos.y));
            float dist = (p2->pos - p1->pos).getLength();
            disp_rect.setSize(sf::Vector2f(dist + 4, 4));
            disp_rect.setRotation(ang);
            window->draw(disp_rect);
        }
    }
};

class Stick {
public:
    PhysPoint* p1;
    PhysPoint* p2;
    float minDist, maxDist;
    Stick(PhysPoint* p1, PhysPoint* p2, float minDist = 0, float maxDist = 0) {
        this->p1 = p1;
        this->p2 = p2;
        if (minDist == 0 && maxDist == 0) {
            float dist = (p2->pos - p1->pos).getLength();
            this->minDist = dist;
            this->maxDist = dist;
        }
        else {
            this->minDist = minDist;
            this->maxDist = maxDist;
        }
    }
    void constrain() {
        float totalMass = p1->mass + p2->mass;
        float percent1 = p1->mass / totalMass;
        float percent2 = p2->mass / totalMass;


        vec line = p1->pos - p2->pos;
        float len = line.getLength();
        vec correct1;
        vec correct2;
        if (len < minDist) {
            correct1 = vec(line).normalized(((minDist - len)*percent2));
            correct2 = vec(line).normalized(((minDist - len)*percent1));
        }
        else if (len > maxDist) {
            correct1 = vec(line).normalized(((maxDist - len)*percent2));
            correct2 = vec(line).normalized(((maxDist - len)*percent1));
        }
        //if (!p1->locked) p1->worldNozzle += correct1;
        //if (!p2->locked) p2->worldNozzle -= correct2;
        p1->pos += correct1;
        p2->pos -= correct2;
    }
};

void springForce(PhysPoint* p1, PhysPoint* p2,
                 float minDist, float maxDist,
                 float mass1 = 1, float mass2 = 1,
                 float strength = 1);

void stickForce(PhysPoint* p1, PhysPoint* p2,
                float minDist, float maxDist,
                float mass1 = 1, float mass2 = 1);

void springForce(vec& pos1, vec& pos2,
                 float minDist, float maxDist,
                 float mass1 = 1, float mass2 = 1,
                 float strength = 1);

void stickForce(vec& pos1, vec& pos2,
                float minDist, float maxDist,
                float mass1 = 1, float mass2 = 1);
