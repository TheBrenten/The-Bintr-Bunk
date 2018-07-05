#include "Particle.h"
#include "LoadResources.h"
#include "Player.h"
extern float screenShake;
extern Player* p;
void createExplosion(vec pos)
{
    playSound(explosion, pos, 20,randgauss(1,.1),3000);
    
    
    // deal damage to nearby objects
    // apply explosion force to objects
    for (int i = 0; i < activeObjects.size(); i++) {
        GameObject* obj = activeObjects[i];
        if (obj->isEnemy || obj == p) {
            float dist = (obj->pos - pos).getLength();
            if (dist < 250) {
                float damage = (250 - dist)*.7;
                float force = damage * .8;
                if (force > 25) force = 25;
                //say("F: " << force);
                obj->pos += vec(force, 0).rotated((obj->pos - pos).getAngle());
                if (obj == p) damage *= .4; // decrease explosion damage for player
                obj->hurt(damage);
            }
        }
    }

    // create particles
    for (int i = 0; i < 10; i++) {
        vec smokeVel = randVec(1, 1);
        Color smokeColor = hsvtorgb(0, 0, randnum(0, 1));
        Particle* p = new Particle(pos + smokeVel*randnum(80, 135),
                                   smokeVel*randnum(0, 1) + randVec(0.1, 1),
                                   randint(100, 200),
                                   randnum(0.8, 1.2),
                                   randnum(2, 3),
                                   Color(smokeColor.r, smokeColor.g, smokeColor.b, 80),
                                   Color(smokeColor.r, smokeColor.g, smokeColor.b, 0),
                                   randgauss(-.01,.002),
                                   0.998);
        p->setTexture(blob);
        p->angle = randnum(0, 360);
        p->angVel = randgauss(0, 1);
        p->layer = 1;
        //p->blendMode = BlendAdd;

    }

    Color flashColor = hsvtorgb(randnum(30, 60), randnum(0.5, 1), randnum(0.5, 1));
    for (int i = 0; i < 10; i++) {
        Particle* p = new Particle(pos,
                                   randVec(0, 1),
                                   randint(4, 7), 0.1, randnum(2.5, 4.5),
                                   Color(flashColor.r, flashColor.g, flashColor.b, 255),
                                   Color(flashColor.r, flashColor.g, flashColor.b, 0));
        p->setTexture(blob);
        p->blendMode = BlendAdd;
        p->layer = 1;
    }

    // break terrain
    float playerDist = (p->pos - pos).getLength();
    screenShake += 500.f / playerDist;
    if (screenShake > 5) screenShake = 5;
    destroyTerrainCircle(pos, randgauss(100, 5));
}
