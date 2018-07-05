#include "ImageObj.h"



ImageObj::ImageObj()
{
}


ImageObj::~ImageObj()
{
}

void ImageObj::set(vec pos, float angle, bool dir)
{
    this->pos = pos;
    this->angle = angle;
    this->dir = dir;
}

void ImageObj::setInterpolation()
{
    posInterp = pos;
    angInterp = angle;
}

void ImageObj::move()
{
    if (parent) {
        pos = parent->pos + pos;
        angle = parent->angle + angle;
        dir = parent->dir;
    }


}

void ImageObj::render()
{
    vec   posRender = interpVec(posInterp, pos);
    float angRender = interpAng(angInterp, angle);
    //if (parent) {
    //    posRender = interpVec(parent->posInterp, parent->pos);
    //    angRender = interpAng(parent->angInterp, parent->angle);
    //}
    sprite.setPosition(posRender.x, posRender.y);
    sprite.setRotation(angRender);
    if (dir) sprite.setScale(1, 1);
    else sprite.setScale(-1, 1);
    window->draw(sprite);
}
