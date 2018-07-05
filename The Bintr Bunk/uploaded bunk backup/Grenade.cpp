#include "Grenade.h"
#include "Particle.h"
#include "Player.h"
extern float screenShake;
extern Player* p;

void Grenade::move()
{
    life--;
    
    if (!checkTerrainCircle(pos, 8)) {
        pos += vel;
        vel *= 0.95;
        vel.y += 0.7;
        updateRegions();
    }
    else vel = vec(0, 0);
    for (int i = 0; i < activeObjects.size(); i++) {
        GameObject* obj = activeObjects[i];
        if (obj->isEnemy && (obj->pos - pos).getLength() < 55)
            life = 1;
    }
    if (life == 1) {
        createExplosion(pos);
    }
    if (life <= 0) {
        delete this;
    }
}