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
        setInterpolation();
        updateRegions();
    }

void Bullet::setInterpolation()
{
    posInterp = pos;
    //angInterp;
}

void Bullet::move()
{
    //life--;
    //say("Bullet move");
    if (despawn) life = 0;
    else {
        tracerPositions.clear();
        tracerPositions.insert(tracerPositions.begin(),pos);
        vel *= 0.97;
        vel.y += .8;
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
                            float damage = (vel.getLength()*.5) - 40;
                            if (damage < 0) damage = 0;
                            obj->hurt(damage);
                            //if (damage) say("Damage: " << damage);
                            vel = vec(0, 0);
                            despawn = true;
                        }
                    }
                }
            }
            //new Dot(worldNozzle);
            //if (debugDraw) new Dot(worldNozzle, Color::Red, 2);
            if (checkTerrainCircle(pos, 4)) {
                tracerPositions.insert(tracerPositions.begin(), pos);
                vel *= 0.75;
                float velMag = vel.getLength();
                //vel.rotate(randgauss(0, velMag*.2));
                vel.rotate(randgauss(0, 20));
                int rad = velMag*0.1;
                if (rad > 7) rad = 7;
                if (rad > 0) {
                    if (rad > 4) {
                        destroyTerrainCircle(pos, rad);
                        if (randint(1, 10) == 1) {
                            Particle* p = new Particle(pos,
                                                       randVec(0.1, 1),
                                                       randint(30, 90), 0.1, 0.5, Color(255, 255, 255, 100), Color(255, 255, 255, 0));
                            p->setTexture(blob);
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
        tracerPositions.insert(tracerPositions.begin(), pos);
        //float mag = vel.getLength();
        samples = vel.getLength() / 8 + 1;
        //vel.norm((mag / 8.f) * samples);
        //say(vel.getLength());
    }
    if (life == 0) life = 0;
}
#include "GameFunction.h"
extern GameFunction gfPhysics;
void Bullet::render()
{
    if (tracerPositions.size() > 0) {
        //vec posRender = interpVec(posInterp, worldNozzle);
        //vec p1 = posRender;
        //vec p2 = tracerPositions[tracerPositions.size()-1];
        //vec line = p2 - p1;
        RectangleShape disp_rect;
        disp_rect.setFillColor(Color::Yellow);
        //disp_rect.setSize(Vector2f(line.getLength(), 2));
        //disp_rect.setRotation(line.getAngle() + 180);
        //disp_rect.setPosition(p2.x, p2.y);
        disp_rect.setOrigin(0, 1);
        ////new Dot(posRender);
        ////window->draw(disp_rect);
        //float interp = ((gfPhysics.clock.elapsed())*(1.f / gfPhysics.rate));
        
        for (int i = 0; i < tracerPositions.size() - 1; i++) {
            //say("i: " << i << " ... Size: " << tracerPositions.size() - 1);
            //new Dot(p1, Color::Green);
            vec p1 = tracerPositions[i];
            vec p2 = tracerPositions[i + 1];
            vec line = p2 - p1;
            disp_rect.setSize(Vector2f(line.getLength(), 2));
            disp_rect.setRotation(line.getAngle() + 180);
            disp_rect.setPosition(p2.x, p2.y);
            window->draw(disp_rect);
        }
    }
}