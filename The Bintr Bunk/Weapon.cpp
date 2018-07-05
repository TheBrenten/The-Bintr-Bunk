#include "Weapon.h"

vector<KeyFrame*> activeKeyFrames;


Weapon::Weapon(vector<string> filepaths, vec origin, vec offset)
{
    textures.reserve(filepaths.size());
    this->origin = origin;
    this->offset = offset;
    for (int i = 0; i < filepaths.size(); i++) {
        Image image;
        textures.emplace_back(Texture());
        sprites.emplace_back(Sprite());
        image.loadFromFile(filepaths[i]);
        for (int x = 0; x < image.getSize().x; x++) {
            for (int y = 0; y < image.getSize().y; y++) {
                // GREEN - REQUIRED
                if (image.getPixel(x, y) == Color(0, 255, 0)) {
                    replacePixel(&image, x, y);
                    if (i == 0) grip1 = vec(x, y);
                    else {
                        sprites[i].setOrigin(x-grip1.x+origin.x,
                                             y-grip1.y+origin.y);
                    }
                }
                // OTHER POINTS OPTIONAL, ONLY APPLICIABLE TO MAIN [0] ITEM
                if (i == 0) {
                    if (image.getPixel(x, y) == Color(255, 0, 0)) {
                        replacePixel(&image, x, y);
                        nozzle = vec(x, y);
                    }
                    // armBack top - 0,0,255
                    // 2 handed
                    if (image.getPixel(x, y) == Color(0, 0, 255)) {
                        replacePixel(&image, x, y);
                        grip2 = vec(x, y);
                        isTwoHanded = true;
                    }
                }
            }
        }
        textures[i].loadFromImage(image); // after image is processed
        sprites[i].setTexture(textures[i],true);
    }
    sprites[0].setOrigin(origin.x, origin.y);
}

void Weapon::shoot()
{
    vec itemNozzle;
    //if (this == p->mainItem) itemNozzle = mainItemNozzle;
    //else if (this == p->lItem) itemNozzle = lItemNozzle;
    //else if (this == p->rItem) itemNozzle = rItemNozzle;
    if (shootTracker == 0 &&
        magBullets > 0 &&
        reloadTracker == 0) {
        playSound(shootSound, worldNozzle, 20);
        magBullets--;
        shootTracker = shootSteps;
        for (int i = 0; i < projectilesPerShot; i++) {
            //float angle = aimAngle;
            //if (!p->dir) angle -= 180;
            if (shootFrame) {
                // reset active frame timer if exists
                if (activeFrame) activeFrame->elapsed = 0;
                activeFrame = shootFrame;
            }
            if (projectile == bullet) {
                new Bullet(worldNozzle, angle + randgauss(0, inaccuracy), randgauss(projectileSpeed, 10));// muzzlePosition, angle, speed, inaccuracyDegree);
                                                                                             //p->posLast += vec(150,0).rotated(angle) * 0.004;
            }
            else if (projectile == grenade) {
                //new Grenade(mainItemNozzle,angle+randgauss(0,14), randgauss(34,6));
                new Grenade(worldNozzle, angle + randgauss(0, inaccuracy), projectileSpeed);
            }
        }
    }
}

void Weapon::reload()
{
    if (reloadTracker == 0) {
        if (reloadFrame) {
            // reset active frame timer if exists
            if (activeFrame) activeFrame->elapsed = 0;
            activeFrame = reloadFrame;
        }
        reloadTracker = reloadSteps;
        playSound(reloadSound, worldNozzle, 20);
    }
}

void Weapon::pressEvent()
{

    if (hasTrigger)
        playSound(triggerPull, worldNozzle, 10);
    else if (hasPin)
        playSound(grenadePin, worldNozzle, 20);

    if (shootOnPress) {
        shoot();
    }

}

void Weapon::releaseEvent()
{
    if (hasTrigger)
        playSound(triggerRelease, worldNozzle, 10);
    if (shootOnRelease) {
        shoot();
    }
}

void Weapon::render()
{
    for (int i = sprites.size()-1; i >= 0; i--) {
    //for (int i=0;i<sprites.size();i++) {
        if (!(itemInVector(i, invisibleSprites))) {
            window->draw(sprites[i]);
        }
    }
}

void linkFrames(KeyFrame * kf1, KeyFrame * kf2)
{
    kf1->nextFrame = kf2;
    kf2->lastFrame = kf1;
}
