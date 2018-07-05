#include "PhysicsStuff.h"

void springForce(PhysPoint * p1, PhysPoint * p2, float minDist, float maxDist, float mass1, float mass2, float strength)
{
    float totalMass = mass1 + mass2;
    vec line = p1->pos - p2->pos;
    float len = line.getLength();
    vec correct1;
    vec correct2;
    if (len > maxDist) {
        strength *= (len - maxDist);
        line.norm(strength);
        correct1 = line*(mass2 / totalMass);
        correct2 = line*(mass1 / totalMass);
    }
    else if (len < minDist) {
        strength *= (len - minDist);
        line.norm(strength);
        correct1 = line*(mass2 / totalMass);
        correct2 = line*(mass1 / totalMass);
    }
    p1->posLast += correct1;
    p2->posLast -= correct2;
}

void stickForce(PhysPoint * p1, PhysPoint * p2, float minDist, float maxDist, float mass1, float mass2)
{
    float totalMass = mass1 + mass2;
    float percent1 = mass1 / totalMass;
    float percent2 = mass2 / totalMass;


    vec line = p1->pos - p2->pos;
    float len = line.getLength();
    vec correct1;
    vec correct2;
    if (len < minDist) {
        correct1 = vec(line).normalized(((minDist - len)*percent2));
        correct2 = vec(line).normalized(((minDist - len)*percent1));
    }
    else if (len > maxDist) {
        correct1 = vec(line).normalized(((maxDist - len)*percent2));
        correct2 = vec(line).normalized(((maxDist - len)*percent1));
    }
    p1->pos += correct1;
    p2->pos -= correct2;
}

void springForce(vec & pos1, vec & pos2, float minDist, float maxDist, float mass1, float mass2, float strength)
{
    float totalMass = mass1 + mass2;
    vec line = pos1 - pos2;
    float len = line.getLength();
    vec correct1;
    vec correct2;
    if (len > maxDist) {
        strength *= (len - maxDist);
        line.norm(strength);
        correct1 = line*(mass2 / totalMass);
        correct2 = line*(mass1 / totalMass);
    }
    else if (len < minDist) {
        strength *= (len - minDist);
        line.norm(strength);
        correct1 = line*(mass2 / totalMass);
        correct2 = line*(mass1 / totalMass);
    }
    pos1 -= correct1;
    pos2 += correct2;
}

void stickForce(vec & pos1, vec & pos2, float minDist, float maxDist, float mass1, float mass2)
{
    float totalMass = mass1 + mass2;
    float percent1 = mass1 / totalMass;
    float percent2 = mass2 / totalMass;


    vec line = pos1 - pos2;
    float len = line.getLength();
    vec correct1;
    vec correct2;
    if (len < minDist) {
        correct1 = vec(line).normalized(((minDist - len)*percent2));
        correct2 = vec(line).normalized(((minDist - len)*percent1));
    }
    else if (len > maxDist) {
        correct1 = vec(line).normalized(((maxDist - len)*percent2));
        correct2 = vec(line).normalized(((maxDist - len)*percent1));
    }
    pos1 += correct1;
    pos2 -= correct2;
}
