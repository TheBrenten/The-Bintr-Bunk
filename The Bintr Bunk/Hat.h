#pragma once
#include <SFML/Graphics.hpp>
#include "BintrsTools.h"

using namespace sf;
using namespace std;
class Hat
{
public:
    vec headPoint;
    Texture texture;
    Hat(string filepath) {
        Image image;
        image.loadFromFile(filepath);
        for (int x = 0; x < image.getSize().x; x++) {
            for (int y = 0; y < image.getSize().y; y++) {
                if (image.getPixel(x, y) == Color(0, 255, 0)) {
                    replacePixel(&image, x, y);
                    headPoint = vec(x, y);
                }
            }
        }
        texture.loadFromImage(image); // after image is processed
    }
};

