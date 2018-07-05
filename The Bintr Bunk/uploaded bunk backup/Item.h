#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

#include "Weapon.h"
#include "Hat.h"


class Item
{
public:
    bool isWeapon;
    bool isHat;
    Weapon* weaponP;
    Hat* hatP;
    Sprite sprite;
    void setSprite(sf::Texture* textureP) {
        sprite.setTexture(*textureP);
        sprite.setOrigin(textureP->getSize().x*.5,
                         textureP->getSize().y*.5);
    }
    Item(Weapon* weaponP) {
        isWeapon = true;
        this->weaponP = weaponP;
        setSprite(&weaponP->texture);
    }
    Item(Hat* hatP) {
        isHat = true;
        this->hatP = hatP;
        setSprite(&hatP->texture);
    }
};