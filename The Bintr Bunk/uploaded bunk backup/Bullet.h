#pragma once
#include "GameObject.h"
#include "LoadResources.h"
#include "BintrsTools.h"

class Bullet : public GameObject
{
public:
    bool despawn = false; // leave tracer around before despawning bullet
    vec vel;
    int samples;
    vector<vec> tracerPositions;
    Bullet(vec pos, float angle, float speed);

    void move();
    void renderFront() {
        if (tracerPositions.size() > 0) {
            for (int i = 0; i < tracerPositions.size() - 1; i++) {
                //say("i: " << i << " ... Size: " << tracerPositions.size() - 1);
                vec p1 = tracerPositions[i];
                vec p2 = tracerPositions[i + 1];
                vec line = p2 - p1;
                RectangleShape disp_rect;
                disp_rect.setFillColor(Color::Yellow);
                disp_rect.setSize(Vector2f(line.getLength(), 2));
                disp_rect.setOrigin(0, 1);
                disp_rect.setRotation(line.getAngle() + 180);
                disp_rect.setPosition(p2.x, p2.y);
                window->draw(disp_rect);
            }
        }
    }

};

