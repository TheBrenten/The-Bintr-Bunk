#include "Enemy.h"
#include "Player.h"
#include "Particle.h"
#include "BintrsTools.h"
extern Player* p;
extern bool debugDraw;

extern float physRate;

Enemy::Enemy(vec pos) {
    // init plane sound
    sound.setBuffer(*planeLoop);
    sound.setLoop(true);
    

    spinOutAngle = randgauss(0, 8);
    
    //activeObjects.emplace_back(this);
    //localHull = rect(-50, -50, 100, 100);
    this->pos = pos;
    posLast = pos;
    this->posLast = pos + randVec(1, 1);
    isEnemy = true;
    maxHealth = 100;
    health = maxHealth;
    texture.loadFromImage(flyingBomb);
    sprite.setTexture(texture);
    sprite.setScale(1, 1);
    sprite.setOrigin(sprite.getLocalBounds().width *.5,
                     sprite.getLocalBounds().height*.5);
    updateRegions();
}

//class Node {
//public:
//    int x, y;
//    float gScore, fScore;
//    Node* cameFrom = nullptr;
//    Node(int x, int y) {
//        this->x = x;
//        this->y = y;
//    }
//    bool operator == (Node n) {
//        if (this->x == n.x &&
//            this->y == n.y) return true;
//        return false;
//    }
//    ~Node() {
//
//    }
//};

float heuristic(Node* n1, Node* n2) {
    return (vec(n2->x - n1->x, n2->y - n1->y).getLength());
}

#include <unordered_map>

vector<vec> AStar(Node* start, Node* goal) {
    
    vector<Node*> closedSet;
    vector<Node*> openSet = { start };
    unordered_map<Node*, Node*> cameFrom;
    unordered_map<Node*, float> gScore;
    unordered_map<Node*, float> fScore;
    gScore[start] = 0;
    fScore[start] = heuristic(start, goal);
    int attempts = 0;
    while (openSet.size() > 0 && attempts < 150) {
        attempts++;
        // the node in openSet having the lowest fScore[] value
        Node* current = openSet[0];
        int cInd = 0;
        for (int i = 0; i < openSet.size(); i++) {
            if (fScore[openSet[i]] < fScore[current]) {
                current = openSet[i];
                cInd = i;
            }
        }
        if (debugDraw) new Dot(vec(current->x, current->y), Color::Blue, 10);
        if (current == goal) {
            vector<vec> path = { vec(current->x, current->y) };
            //while current in cameFrom.Keys:
            while (cameFrom.find(current) != cameFrom.end()) {
                current = cameFrom[current];
                path.insert(path.begin(), vec(current->x, current->y));
                if (debugDraw) new Line(path[0], path[1], 6, sf::Color::Blue);
            }
            return path;
        }
        // confirm region nodes are validated
        if (!current->inRegion->nodesValidated) current->inRegion->validateNodes();
        openSet.erase(openSet.begin() + cInd);
        closedSet.emplace_back(current);
        // for each neighbor of current
        for (int i = 0; i < current->validNeighbors.size(); i++) {
            Node* ne = current->validNeighbors[i];
            // confirm region nodes are validated
            if (!ne->inRegion->nodesValidated) ne->inRegion->validateNodes();

            if (itemInVector(ne, closedSet))
                continue;
            if (!(itemInVector(ne, openSet)))
                openSet.emplace_back(ne);
            // The distance from start to a neighbor
            float tentative_gScore = gScore[current] + (vec(current->x - ne->x, current->y - ne->y).getLength());

            // prevent map creating new item
            if (gScore.find(ne) == gScore.end()) gScore[ne] = INFINITY;
            if (tentative_gScore >= gScore[ne])
                continue;		// This is not a better path.

            // This path is the best until now. Record it!
            cameFrom[ne] = current;
            gScore[ne] = tentative_gScore;
            fScore[ne] = gScore[ne] + heuristic(ne, goal);
        }
    }
    
    
    // faliure
    return vector<vec> {};
}

//extern void suicide();
extern bool noclip;
extern float screenShake;
void Enemy::move() {
    //new Dot(pos);
    vec vel = pos - posLast;
    posLast = pos;
    float angle = vel.getAngle();

    if (health <= 0) {
        sound.setPosition(pos.x, pos.y, -500);
        soundVolume += .2;
        if (soundVolume > 10) soundVolume = 10;
        sound.setVolume(soundVolume);
        soundPitch *= 0.995;
        sound.setPitch(soundPitch * physRate / 60);
        if (vel.getLength() < 5) vel *= 1.1;
        vel *= 0.99; // spin out friction
        spinOutSine += randnum(20,30);
        while (spinOutSine > 360) spinOutSine -= 360;
        if (justHurt == 0) justHurt = 4;
        //if (randint(0, 10) < 9) {
        for (int i=0;i<4;i++) {
            float smokeAngle = sin(spinOutSine*PI/180.f) * 70;
            Color smokeColor = hsvtorgb(0, 0, randnum(0, 1));
            vec smokeVel = (vel*-1).normalized(8).rotated(smokeAngle);
            Particle* p = new Particle(pos,
                                       smokeVel,
                                       randint(20, 40),
                                       randnum(0.2, .4),
                                       randnum(.8, 1.3),
                                       Color(smokeColor.r, smokeColor.g, smokeColor.b, 80),
                                       Color(smokeColor.r, smokeColor.g, smokeColor.b, 0));
            p->setTexture(blob);
        }
        spinOutAngle += randgauss(0,.5);
        spinOutAngle *= 0.95;
        vel.rotate(spinOutAngle);
        pos.y += .5;
        pos += vel;
        if (checkTerrain(pos) || health <= -50) {
            life = 0;
            createExplosion(pos);
        }
    }
    else {

        vel *= 0.96; // alive friction

        // pathfinding
        // note: quit trying to do super elaborate shit, just get basic enemy movement
        // lol nvm
        // determine where to head to

        // select start/end nodes
        Node* start = nullptr;
        Node* goal  = nullptr;
        float closestNodeLength;
        closestNodeLength = INFINITY;
        for (int i=0;i<inRegions.size();i++) {
            // validate region nodes
            if (!inRegions[i]->nodesValidated) {
                inRegions[i]->validateNodes();
            }
            for (auto it = inRegions[i]->nodes.begin(); it != inRegions[i]->nodes.end(); it++) {
                Node* n = it->second;
                if (n->validNeighbors.size() > 0) {
                    float nodeLength = (vec(n->x, n->y) - pos).getLength();
                    if (nodeLength < closestNodeLength) {
                        start = n;
                        closestNodeLength = nodeLength;
                    }
                }
            }
        }
        closestNodeLength = INFINITY;
        
        // define goal position

        vec goalPos;

        bool canSeePlayer = true;
        vec sight = pos;
        vec toPlayer = (p->pos - pos).normalized(20);
        if ((p->pos - pos).getLength() < 1500) {
            while ((p->pos - sight).getLength() > 20) {
                if (checkTerrain(sight)) {
                    canSeePlayer = false;
                    break;
                }
                sight += toPlayer;
            }
        }
        else canSeePlayer = false;
        if (canSeePlayer) {
            playerSpottedAt = p->pos;
            wandering = false;
            goalPos = p->pos;
        }
        else if (!wandering) {
            goalPos = playerSpottedAt;
        }
        else if (allowedToHearPlayer) {
            playerSpottedAt = p->pos;
            goalPos = playerSpottedAt;
            wandering = false;
        }


        //new Dot(goalPos);
        for (int i = 0; i<p->inRegions.size(); i++) {
            for (auto it = p->inRegions[i]->nodes.begin(); it != p->inRegions[i]->nodes.end(); it++) {
                Node* n = it->second;
                if (n->validNeighbors.size() > 0) {
                    float nodeLength = (vec(n->x, n->y) - goalPos).getLength();
                    if (nodeLength < closestNodeLength) {
                        goal = n;
                        closestNodeLength = nodeLength;
                    }
                }
            }
        }
        //if (!start) say("No start node found!");
        //if (!goal)   say("No end node found!");
        if (!wandering) {
            ///disp_circ.setFillColor(Color::Red);
            vector<vec> path;
            if (start && goal) {
                path = AStar(start, goal);
            }
            {
                
                vec traverse;
                // found path to player
                if (path.size() > 0) {
                    traverse = vec(path[1]);
                    vel += (traverse - pos).normalized(0.6);
                    //if (debugDraw) new Dot(traverse, Color::Green, 10);
                    if (!canSeePlayer && (path[path.size()-1] - pos).getLength() < 100) {
                        wandering = true;
                    }
                }
                // couldnt find path to player
                else {
                    if (!canSeePlayer) {
                        wandering = true;
                    }
                }
                //else {
                //    gotoLastPos = true;
                //}


            }
        }
        else {
            ///disp_circ.setFillColor(Color::Blue);
            
            vec leftWhisker = pos +  vec(120, 0).rotated(angle-30);
            vec rightWhisker = pos + vec(120, 0).rotated(angle+30);

            if (checkTerrain(leftWhisker)) {
                vel.rotate(4);
            }
            else if (checkTerrain(rightWhisker)) {
                vel.rotate(-4);
            }
            vec addForce = vel;
            addForce.rotate(randgauss(0,40));
            addForce.norm(.18);
            vel += addForce;
            //pos += vel;
        }
        // repel from other enemies
        for (int i = 0; i < activeObjects.size(); i++) {
            if (activeObjects[i]->isEnemy) {
                if ((activeObjects[i]->pos - pos).getLength() < 75) {
                    vec toOther = (activeObjects[i]->pos - pos).normalized();
                    pos -= toOther;
                    activeObjects[i]->pos += toOther;
                }
            }
        }
        pos += vel;
        if ((p->pos - pos).getLength() < 50 && !noclip) {
            if (!p->invincibilitySteps) {
                p->pos.x += vel.x * 3;
                p->pos.y -= 5;
            }
            //p->hurt(vel.getLength());
            p->hurt(10);
        }
        int b = 0;
        while (checkTerrainCircle(pos, 16)) {
            pos += vec(1, 0).rotated(getTerrainNormal(pos, 16));
            b++;
            if (b == 10) break;
        }
    }
    updateRegions();

    sprite.setRotation(angle);
    if (angle > 110 && angle < 250) {
        sprite.setScale(1, -1);
    }
    else if (angle < 70 || angle > 290) {
        sprite.setScale(1, 1);
    }
    if (justHurt > 0) justHurt--;
    if (canPlaySound > 0) canPlaySound--;
    allowedToHearPlayer = false;
    if (life == 0 || (p->pos - pos).getLength() > 4500) {
        delete this;
    }
}




void Enemy::renderFront() {

    //BlendMode blendMode = sf::BlendMultiply;
    ////if (justHurt) {
    ////sprite.setColor(Color(255,255,255, 0));
    //blendMode.colorSrcFactor = sf::BlendMode::Factor::SrcAlpha;
    //blendMode.colorDstFactor = sf::BlendMode::Factor::DstAlpha;
    //blendMode.alphaSrcFactor = sf::BlendMode::Factor::SrcAlpha;
    //blendMode.alphaDstFactor = sf::BlendMode::Factor::DstAlpha;


    //}
    sprite.setPosition(pos.x, pos.y);
    ////disp_circ.setFillColor(Color(0,0,254));
    //disp_circ.setPosition(pos.x,pos.y);
    //disp_circ.setOrigin(50,50);
    //disp_circ.setRadius(50);
    //window->draw(disp_circ);
    if (justHurt == 1) {
        //shaderFlashWhite.setUniform("texture",texture);
        window->draw(sprite, &shaderFlashWhite);
    }
    else
        window->draw(sprite);
}


void Enemy::hurt(float damage)
{
    float healthBefore = health;
    if (canPlaySound == 0)
    {
        playSound(randomChoice(hits), pos, 15, randnum(.9, 1.1));
        canPlaySound = 2;
    }
    health -= damage;
    justHurt = 2; // for flashing effect
    if (healthBefore > 0 && health <= 0) {
        sound.setMinDistance(2000);
        sound.setAttenuation(60);
        sound.setPosition(pos.x, pos.y, -500);
        soundVolume = 0;
        soundPitch = randnum(0.9,1.1);
        sound.setVolume(soundVolume * physRate / 60);
        sound.play();
        float startOffset = planeLoop->getDuration().asMicroseconds();
        startOffset *= randnum(0, 1);
        sound.setPlayingOffset(sf::microseconds(startOffset));
    }
}

void makeNoise(float radius, vec pos)
{
    if (debugDraw) new Dot(pos,Color(255,50,50,50),radius);
    for (int i = 0; i < activeObjects.size(); i++) {
        if ((activeObjects[i]->pos - pos).getLength() < radius) {
            activeObjects[i]->allowedToHearPlayer = true;
        }
    }
}
