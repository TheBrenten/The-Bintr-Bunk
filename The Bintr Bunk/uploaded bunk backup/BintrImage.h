#pragma once

#include <SFML/Graphics.hpp>
#include "BintrsTools.h"
using namespace sf;

class BintrImage
{
public:
    Image image;
};


// PlayerImage is generated on launch, saving and replacing special pixel type stuff
class PlayerImage :public BintrImage {
public:
    vec hat;
    vec armFront;
    vec armBack;
    PlayerImage(string filepath) {
        image.loadFromFile(filepath);
        for (int x = 0; x < image.getSize().x; x++) {
            for (int y = 0; y < image.getSize().y; y++) {
                // head top - 255,0,0
                if (image.getPixel(x, y) == Color(255, 0, 0)) {
                    replacePixel(&image, x, y);
                    hat = vec(x, y);
                }
                // head bot - 254,0,0
                if (image.getPixel(x, y) == Color(254, 0, 0)) {
                    replacePixel(&image, x, y);
                    //hat = vec(x, y);
                }
                // armFront top - 0,255,0
                if (image.getPixel(x, y) == Color(0, 255, 0)) {
                    replacePixel(&image, x, y);
                    armFront = vec(x, y);
                }
                // armFront bot - 0,254,0
                if (image.getPixel(x, y) == Color(0, 254, 0)) {
                    replacePixel(&image, x, y);
                }
                // armBack top - 0,0,255
                if (image.getPixel(x, y) == Color(0, 0, 255)) {
                    replacePixel(&image, x, y);
                    armBack = vec(x, y);
                }
                // armBack top - 0,0,254
                if (image.getPixel(x, y) == Color(0, 0, 254)) {
                    replacePixel(&image, x, y);
                }
            }
        }
    }

};

class ArmImage : public BintrImage {
public:
    vec shoulder, hand;
    ArmImage(string filepath) {
        image.loadFromFile(filepath);
        for (int x = 0; x < image.getSize().x; x++) {
            for (int y = 0; y < image.getSize().y; y++) {
                // shoulder - 0,255,0
                if (image.getPixel(x, y) == Color(0, 255, 0)) {
                    replacePixel(&image, x, y);
                    shoulder = vec(x, y);
                }
                // hand - 0,0,255
                if (image.getPixel(x, y) == Color(0, 0, 255)) {
                    replacePixel(&image, x, y);
                    hand = vec(x, y);
                }
            }
        }
    }
};

class ImageData {
public:
    Texture texture;
    vec hat;
    vec armFront;
    vec armBack;
    ImageData() {}
    ImageData(PlayerImage* source,
              Color skinColor,
              Color shirtColor,
              Color pantsColor);
};


class ArmData {
public:
    Texture texture;
    vec shoulder, hand;
    ArmData() {}
    ArmData(ArmImage* source,
            Color skinColor,
            Color shirtColor,
            Color pantsColor) {
        this->shoulder = source->shoulder;
        this->hand = source->hand;
        Image imgCopy = source->image;
        for (int x = 0; x < imgCopy.getSize().x; x++) {
            for (int y = 0; y < imgCopy.getSize().y; y++) {
                // skin color (255,214,156)
                if (imgCopy.getPixel(x, y) == Color(255, 214, 156)) {
                    imgCopy.setPixel(x, y, skinColor);
                }
                // shirt color (128,0,0)
                if (imgCopy.getPixel(x, y) == Color(128, 0, 0)) {
                    imgCopy.setPixel(x, y, shirtColor);
                }
                // pants color (0,0,128)
                if (imgCopy.getPixel(x, y) == Color(0, 0, 128)) {
                    imgCopy.setPixel(x, y, pantsColor);
                }
            }
        }
        texture.loadFromImage(imgCopy);

    }
};
