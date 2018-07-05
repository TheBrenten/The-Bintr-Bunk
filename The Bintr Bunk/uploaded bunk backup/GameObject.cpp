#include "GameObject.h"

vector<GameObject*> activeObjects;

void GameObject::updateRegions()
{
    canMove = true;
    // remove self reference from any containing regions
    for (int i = 0; i < inRegions.size(); i++) {
        Region* r = inRegions[i];
        for (int j = r->containedObjects.size()-1; j >=0 ; j--) {
            if (r->containedObjects[j] == this) {
                r->containedObjects.erase(r->containedObjects.begin() + j);
            }
        }
    }
    // rebuild vector of containing regions
    inRegions.clear();
    int regionX1 = getRegion(pos.x + localHull.x);
    int regionY1 = getRegion(pos.y + localHull.y);
    int regionX2 = getRegion(pos.x + localHull.x + localHull.w);
    int regionY2 = getRegion(pos.y + localHull.y + localHull.h);
    for (int RX = regionX1; RX <= regionX2; RX++) {
        for (int RY = regionY1; RY <= regionY2; RY++) {
            pair<int, int> key = make_pair(RX, RY);
            if (keyInMap(key,regions)) {
                Region* r = regions[key];
                r->life = 300; // refresh region life
                //if (r->life < 2) say("Region Life: " << r->life);
                inRegions.emplace_back(r);
                r->containedObjects.emplace_back(this);
            }
            // if region doesnt exist, make it
            else {
                confirmRegion(RX, RY);
                canMove = false;
            }
        }
    }
    //if (!canMove) say("Movement blocked!");
}

GameObject::~GameObject()
{
    // remove self references...

    // from regions...
    for (int i = 0; i < inRegions.size(); i++) {
        if (!inRegions[i]->textureUpdated) say("Fuck this is bad");
        for (int j = inRegions[i]->containedObjects.size()-1; j >=0 ; j--) {
            if (inRegions[i]->containedObjects[j] == this)
                inRegions[i]->containedObjects.erase(inRegions[i]->containedObjects.begin() + j);
        }
    }
    // from activeObjects...
    int jew = 0;
    for (int i = activeObjects.size() - 1; i >= 0; i--) {
        if (activeObjects[i] == this) {
            activeObjects.erase(activeObjects.begin() + i);
            jew++;
        }
        if (jew > 1) say("Holy fucking cunt turds");
    }
}
