#pragma once
#include "GameObject.h"
class ImageObj :
    public GameObject
{
public:
    ImageObj();
    ~ImageObj();
    sf::Sprite sprite;
    void set(vec pos, float angle, bool dir);
    void setInterpolation();
    void move()   ;
    void render() ;
};

