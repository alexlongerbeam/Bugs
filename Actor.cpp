#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
#include <vector>
using namespace std;
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp



////////////////////////////////////////////////////////////////////////////////////
//*******************************ACTOR METHODS**************************************
////////////////////////////////////////////////////////////////////////////////////
Actor::Actor(int x, int y, StudentWorld * w, int imageID, Direction dir, unsigned int depth): GraphObject(imageID, x, y, dir, depth){

        m_world = w;
        m_isAlive = true;
        m_active = true;
}

Actor::~Actor(){}

bool Actor::isAlive(){
    return m_isAlive;
}

void Actor::setDead(){
    m_isAlive = false;
}

StudentWorld* Actor::getWorld(){
    return m_world;
}

bool Actor::isActive(){
    return m_active;
}
void Actor::reActivate(){
    m_active = true;
}

void Actor::moved(){
    m_active = false;
}

////////////////////////////////////////////////////////////////////////////////////
//*******************************ENERGY METHODS*************************************
////////////////////////////////////////////////////////////////////////////////////

EnergyHolder::EnergyHolder(int x, int y, StudentWorld * w, int imageID, int p, unsigned int depth): Actor(x, y, w, imageID, right, depth){
    
    m_points = p;
}

EnergyHolder::~EnergyHolder(){}

void EnergyHolder::addPoints(int add){
    m_points += add;
}
void EnergyHolder::subtractPoints(int sub){
    m_points -= sub;
}
int EnergyHolder::getPoints(){
    return m_points;
}


////////////////////////////////////////////////////////////////////////////////////
//*********************************FOOD METHODS*************************************
////////////////////////////////////////////////////////////////////////////////////

Food::Food(int x, int y, StudentWorld* w, int p): EnergyHolder(x, y, w, IID_FOOD, p, 2){}
Food::~Food(){}

int Food::pickUpFood(int amount){
    int points = getPoints();
    if (points>amount){
        subtractPoints(amount);
        return amount;
    }
    else{
        subtractPoints(points);
        setDead();
        return points;
    }
    
}

void Food::addFood(int amount){
    addPoints(amount);
}

void Food::doSomething(){}

bool Food::canMove(){
    return false;
}

////////////////////////////////////////////////////////////////////////////////////
//*******************************ANTHILL METHODS************************************
////////////////////////////////////////////////////////////////////////////////////

Anthill::Anthill(int x, int y, StudentWorld * w, Compiler * comp, int anthillNum, int p, unsigned int depth):EnergyHolder(x, y, w, IID_ANT_HILL, p, depth){
    
    m_compiler = comp;
    m_colonyNum = anthillNum;

}

Anthill::~Anthill(){}

bool Anthill::canMove(){
    return false;
}

void Anthill::doSomething(){
    
    subtractPoints(1);
    
    
    //check if dead
    if (getPoints()<=0){
        setDead();
        return;
    }
    
    //try to eat food
    int eaten = getWorld()->eatFood(getX(), getY(), 10000);
    
    if (eaten!=0){
        addPoints(eaten);
        return;
    }
    
    
    //check to try and make new ant
    if (getPoints()>=2000){
        //add ant
        subtractPoints(1500);
        getWorld()->addAntNum(m_colonyNum);
    }
    
    
    
    
}

////////////////////////////////////////////////////////////////////////////////////
//*******************************PHEROMONE METHODS**********************************
////////////////////////////////////////////////////////////////////////////////////


Pheromone::Pheromone(int x, int y, StudentWorld * w, int colonyNum, int imageID, int p, unsigned int depth): EnergyHolder(x, y, w, imageID, p, depth){
    m_colonyNum = colonyNum;
}
Pheromone::~Pheromone(){}

void Pheromone::doSomething(){
    subtractPoints(1);
    
    if (getPoints()==0)
        setDead();
}

bool Pheromone::canMove(){
    return false;
}

////////////////////////////////////////////////////////////////////////////////////
//*******************************INSECT METHODS*************************************
////////////////////////////////////////////////////////////////////////////////////

Insect::Insect(int x, int y, StudentWorld * w, int imageID, int p): EnergyHolder(x, y, w, imageID, p){
    
    //Pick random direction to face
    randomDir();
    m_ticksToSleep = 0;
    
}

Insect::~Insect(){}




void Insect::addTicks(int t){
    m_ticksToSleep+=t;
}

void Insect::resetTicks(){
    m_ticksToSleep = 2;
}

void Insect::sub1Tick(){
    m_ticksToSleep--;
}
int Insect::ticksToSleep(){
    return m_ticksToSleep;
}


bool Insect::moveOne(){
    //If pebble present, return false cause it didn't move
    
    
    //If no pebble in the way
    
    Direction d = getDirection();
    
    switch (d){
        case up:
            if (getWorld()->pebbleAt(getX(), getY()+1)){
                return false;
            }
            moveTo(getX(), getY()+1);
            break;
        case right:
            if (getWorld()->pebbleAt(getX()+1, getY())){
                return false;
            }
            moveTo(getX()+1, getY());
            break;
        case down:
            if (getWorld()->pebbleAt(getX(), getY()-1)){
                return false;
            }
            moveTo(getX(), getY()-1);
            break;
        case left:
            if (getWorld()->pebbleAt(getX()-1, getY())){
                return false;
            }
            moveTo(getX()-1, getY());
            break;
        case none:
            return false;
    }
    
    return true;
}



void Insect::randomDir(){
    int d = randInt(0, 3);
    switch (d){
        case 0:
            setDirection(up);
            break;
        case 1:
            setDirection(right);
            break;
        case 2:
            setDirection(down);
            break;
        case 3:
            setDirection(left);
            break;
    }

}


bool Insect::canMove(){
    return true;
}

void Insect::die(){
    cerr<<"DEATH"<<endl;
    getWorld()->depositFood(getX(), getY(), 100);
    setDead();
}

bool Insect::biteRandom(int damage){
    vector<Insect *> v;
    getWorld()->getInsects(getX(), getY(), v, this);
    
    if (v.empty())
        return false;
    
    
    int insect = randInt(0, v.size()-1);
    
    v[insect]->getBitten(damage);
    
    return true;
}
////////////////////////////////////////////////////////////////////////////////////
//*******************************GRASSHOPPER METHODS********************************
////////////////////////////////////////////////////////////////////////////////////

Grasshopper::Grasshopper(int x, int y, StudentWorld * w, int imageID, int p):Insect(x, y, w, imageID, p){
    
    resetDistance();
    
}

Grasshopper::~Grasshopper(){}

void Grasshopper::doSomething(){
    if (beginningCommon()){
        if (moveUnique()){
            endCommon();
        }
        resetTicks();
    }
    
    
    
}



int Grasshopper::distanceToWalk(){
    return m_distanceToWalk;
}

void Grasshopper::resetDistance(){
    m_distanceToWalk = randInt(2, 10);
}

void Grasshopper::sub1Walk(){
    m_distanceToWalk--;
}

void Grasshopper::setDistanceZero(){
    m_distanceToWalk = 0;
}

void Grasshopper::stun(){}
void Grasshopper::poison(){}
void Grasshopper::getBitten(int amount){
    subtractPoints(amount);
    cerr<<"Grasshopper bitten"<<endl;
    if (getPoints()<=0){
        die();
        return;
    }
    
    //50% chance of bite back
    bool cont = randInt(0, 1);
    if (cont){
        cerr<<"Biting back"<<endl;
        biteRandom(50);
    }
    
    
    
}

//returns whether or not the doSomething function should continue
bool Grasshopper::beginningCommon(){
    subtractPoints(1);
    
    //cerr<<getPoints()<<endl;
    if (getPoints()<1){
        die();
        return false;
    }
    
    if (ticksToSleep()>0){
        sub1Tick();
        return false;
    }
    
    return true;
}

bool Grasshopper::moveUnique(){
    //1 in 3 chance of trying to bite another
    int bite = randInt(0, 2);
    if (bite==0){
        if (biteRandom(50))
            return false;
    }
    
    //If it didn't decide to bite or wasn't able to bite
    
    
    
    
    return true;
}

void Grasshopper::endCommon(){
    //Attempt to eat food
    int foodEaten = getWorld()->eatFood(getX(), getY(), 200);
    
    if (foodEaten!=0){
        addPoints(foodEaten);
        bool cont = randInt(0, 1);
        if (!cont){
            resetTicks();
            return;
        }
    }
    //Continue walking
    if (distanceToWalk()==0){
        randomDir();
        resetDistance();
    }
    if(moveOne())
        sub1Walk();
    else
        setDistanceZero();
}
////////////////////////////////////////////////////////////////////////////////////
//**************************BABY GRASSHOPPER METHODS********************************
////////////////////////////////////////////////////////////////////////////////////
BabyGrasshopper::BabyGrasshopper(int x, int y, StudentWorld * w, int imageID, int p): Grasshopper(x, y, w, imageID, p){

}

BabyGrasshopper::~BabyGrasshopper(){}

bool BabyGrasshopper::moveUnique(){
    
    if (getPoints()>=1600){
        //Turn into adult grasshopper
        Actor * a = new Grasshopper(getX(), getY(), getWorld());
        getWorld()->newActor(getX(), getY(), a);
        setDead();
        return false;
    }
    
    return true;

}


void BabyGrasshopper::stun(){
    addTicks(2);
}

void BabyGrasshopper::poison(){
    subtractPoints(150);
    if (getPoints()<=0)
        die();
}

void BabyGrasshopper::getBitten(int amount){
    subtractPoints(amount);
    cerr<<"Baby bitten"<<endl;
    if (getPoints()<=0)
        die();
}
////////////////////////////////////////////////////////////////////////////////////
//*******************************OBJECT METHODS*************************************
////////////////////////////////////////////////////////////////////////////////////

Object::Object(int x, int y, StudentWorld * w, int imageID, int depth, Direction dir): Actor(x, y, w, imageID, dir, depth){};

Object::~Object(){}

void Object::attack(){}

bool Object::canMove(){
    return false;
}

////////////////////////////////////////////////////////////////////////////////////
//*******************************PEBBLE METHODS*************************************
////////////////////////////////////////////////////////////////////////////////////

Pebble::Pebble(int x, int y, StudentWorld * w):Object(x, y, w, IID_ROCK, 1){}

Pebble::~Pebble(){}

void Pebble::doSomething(){}

////////////////////////////////////////////////////////////////////////////////////
//********************************WATER METHODS*************************************
////////////////////////////////////////////////////////////////////////////////////

Water::Water(int x, int y, StudentWorld * w): Object(x, y, w, IID_WATER_POOL){}

Water::~Water(){}

bool Water::wasPrevious(Insect *p){
    for (int i = 0; i<previous.size(); i++){
        if (previous[i]==p)
            return true;
    }
    return false;
}

void Water::doSomething(){
    vector<Insect *> onSquare;
    
    
    getWorld()->getInsects(getX(), getY(), onSquare, this);
    
    for (int i = 0; i<onSquare.size(); i++){
        if (!wasPrevious(onSquare[i]))
            onSquare[i]->stun();
    }
    
    previous = onSquare;
    
}

////////////////////////////////////////////////////////////////////////////////////
//********************************POISON METHODS************************************
////////////////////////////////////////////////////////////////////////////////////
Poison::Poison(int x, int y, StudentWorld *w): Object(x, y, w, IID_POISON){}
Poison::~Poison(){}

void Poison::doSomething(){
    vector<Insect *> onSquare;
    
    
    getWorld()->getInsects(getX(), getY(), onSquare, this);
    for (int i = 0; i<onSquare.size(); i++){
            onSquare[i]->poison();
    }
    
}














