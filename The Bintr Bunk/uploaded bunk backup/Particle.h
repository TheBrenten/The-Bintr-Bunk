#pragma once
#include "GameObject.h"

#include "BintrsTools.h"
#include <iostream>
#define say(x) std::cout<<x<<'\n'


class Particle : public GameObject
{
public:
    int layer = 0;
    Texture* texture;
    Sprite sprite;
    Color endColor;
    Color startColor;
    Color color;
    int maxLife;
    float angle;
    float angVel;
    bool hasImg = false;
    float startSize, endSize;
    float size;
    float gravity;
    float friction;
    bool stickToTerrain = false;
    CircleShape disp_circ;
    BlendMode blendMode = BlendAlpha;
    Particle(vec pos, vec vel, int life,
             float startSize = 1,
             float endSize = 0,
             Color startColor = Color::Red,
             Color endColor = Color::Blue,
             float gravity = 0,
             float friction = 1) {
        this->pos = pos;
        this->posLast = pos-vel;
        this->maxLife = life;
        this->life = life;
        this->startSize = startSize;
        this->endSize = endSize;
        this->startColor = startColor;
        this->endColor = endColor;
        this->gravity = gravity;
        this->friction = friction;
        updateRegions();
    }
    void setTexture(Texture* texture) {
        this->hasImg = true;
        this->texture = texture;
        this->sprite.setTexture(*texture);
        this->sprite.setOrigin(texture->getSize().x / 2,
                               texture->getSize().y / 2);
    }
    void move() {
        life--;
        if (life == 0) {
            delete this; // fuck
            return;
        }
        vec vel = pos-posLast;
        posLast = pos;
        vel *= friction;
        vel.y += gravity;
        pos += vel;
        if (stickToTerrain && checkTerrain(posLast)) pos = posLast;
        angle += angVel;
        float lifePercentage = 1 - ((maxLife - life) / float(maxLife));
        size = endSize + (startSize - endSize) * lifePercentage;
        color = interpColor(startColor, endColor, lifePercentage);
        updateRegions();
        
    }
    void renderBack() {
        if (layer == 0) render();
    }
    void renderFront() {
        if (layer == 1) render();
    }
    void render() {
        // no sprite
        if (hasImg) {
            sprite.setRotation(angle);
            sprite.setPosition(posInterp.x, posInterp.y);
            sprite.setScale(size, size);
            sprite.setColor(color);
            window->draw(sprite, blendMode);
        }
        else {
            disp_circ.setFillColor(color);
            disp_circ.setRadius(size*.5);
            disp_circ.setPosition(posInterp.x, posInterp.y);
            disp_circ.setOrigin(size*.5, size*.5);
            window->draw(disp_circ, blendMode);
        }
    }

};

void createExplosion(vec pos);