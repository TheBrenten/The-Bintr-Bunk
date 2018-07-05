#include "Grenade.h"
#include "Particle.h"
#include "Player.h"
extern float screenShake;
extern Player* p;

Grenade::Grenade(vec pos, float angle, float speed)
{
    this->pos = pos;
    this->vel = vec(speed, 0).rotated(angle);
    life = 60;
    setInterpolation();
    updateRegions();
}

void Grenade::setInterpolation()
{
    posInterp = pos;
    //angInterp;
}

void Grenade::move()
{
    life--;
    
    if (!checkTerrainCircle(pos, 8)) {
        pos += vel;
        vel *= 0.97;
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
        life = 0;
    }
}

void Grenade::render()
{
    CircleShape disp_circ;
    disp_circ.setFillColor(Color::Blue);
    disp_circ.setRadius(10);
    disp_circ.setOrigin(10, 10);
    vec posRender = interpVec(posInterp, pos);
    disp_circ.setPosition(posRender.x, posRender.y);
    window->draw(disp_circ);
}
