#include "BintrImage.h"

ImageData::ImageData(PlayerImage * source, Color skinColor, Color shirtColor, Color pantsColor)
{
    hat = source->hat;
    armFront = source->armFront;
    armBack = source->armBack;

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