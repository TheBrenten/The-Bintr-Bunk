#include "Bullet.h"
#include "Particle.h"
#define say(x) std::cout<<x<<'\n'
#define randomChoice(vector) vector[randint(0,vector.size()-1)]

extern bool debugDraw;

Bullet::Bullet(vec pos, float angle, float speed)
    {
        //speed *= 4;
        this->vel = vec(speed, 0).rotated(angle);
        vec nVel = vel;
        nVel.norm(5);
        while (checkTerrain(pos)) {
            pos -= nVel;
        }
        this->pos = pos;
        samples = speed / 8;
        //life = 20;
        updateRegions();
    }

void Bullet::move()
{
    //life--;
    //say("Bullet move");
    if (despawn) life = 0;
    else {
        tracerPositions.clear();
        tracerPositions.emplace_back(pos);
        vel *= 0.97;
        vel.y += 1.4;
        bool canPlaySound = true; // prevent excessive terrain noises
        for (int i = 0; i < samples; i++) {
            vec posLast = pos;
            pos += vel*(1.f / samples);
            // "hurt" enemies
            for (int i = 0; i < inRegions.size(); i++) {
                for (int j = 0; j < inRegions[i]->containedObjects.size(); j++) {
                    GameObject* obj = inRegions[i]->containedObjects[j];
                    if (obj->isEnemy) {
                        if ((obj->pos - pos).getLength() < 48) {
                            obj->pos += (vel*0.014);
                            obj->hurt(vel.getLength()*.055);
                            vel = vec(0, 0);
                            despawn = true;
                        }
                    }
                }
            }
            //new Dot(pos);
            if (debugDraw) new Dot(pos, Color::Red, 2);
            if (checkTerrainCircle(pos, 4)) {
                tracerPositions.emplace_back(pos);
                vel *= 0.75;
                float velMag = vel.getLength();
                vel.rotate(randgauss(0, velMag*.2));
                int rad = velMag*0.05;
                if (rad > 0) {
                    if (rad > 4) {
                        destroyTerrainCircle(pos, rad);
                        if (randint(1, 10) == 1) {
                            Particle* p = new Particle(pos,
                                                       randVec(0.1, 1),
                                                       randint(30, 90), 0.1, 0.5, Color(255, 255, 255, 100), Color(255, 255, 255, 0));
                            p->setTexture(smokeCloud);
                            p->angle = randnum(0, 360);
                            p->angVel = randgauss(0, 1);
                        }
                    }
                    else pos = posLast;
                    if (canPlaySound) {
                        playSound(randomChoice(digSounds), pos, 15, randnum(0.5, 2));
                        canPlaySound = false;
                    }
                }
                else {
                    despawn = true;
                    i = samples;
                }
            }
            updateRegions();
        }
        tracerPositions.emplace_back(pos);
        //float mag = vel.getLength();
        samples = vel.getLength() / 8 + 1;
        //vel.norm((mag / 8.f) * samples);
        //say(vel.getLength());
    }
    if (life == 0) delete this;
}