#include "Weapon.h"



void Weapon::shoot()
{
    vec itemNozzle;
    //if (this == p->mainItem) itemNozzle = mainItemNozzle;
    //else if (this == p->lItem) itemNozzle = lItemNozzle;
    //else if (this == p->rItem) itemNozzle = rItemNozzle;
    if (shootTracker == 0 &&
        magBullets > 0 &&
        reloadTracker == 0) {
        playSound(shootSound, pos, 20);
        magBullets--;
        shootTracker = shootSteps;
        for (int i = 0; i < projectilesPerShot; i++) {
            //float angle = aimAngle;
            //if (!p->dir) angle -= 180;
            if (projectile == bullet) {
                new Bullet(pos, angle + randgauss(0, inaccuracy), randgauss(150, 10));// muzzlePosition, angle, speed, inaccuracyDegree);
                                                                                             //p->posLast += vec(150,0).rotated(angle) * 0.004;
            }
            else if (projectile == grenade) {
                //new Grenade(mainItemNozzle,angle+randgauss(0,14), randgauss(34,6));
                new Grenade(pos, angle + randgauss(0, inaccuracy), 30);
            }
        }
    }
}