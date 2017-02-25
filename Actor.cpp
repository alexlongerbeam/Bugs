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
//*******************************INSECT METHODS*************************************
////////////////////////////////////////////////////////////////////////////////////

Insect::Insect(int x, int y, StudentWorld * w, int imageID, int p): EnergyHolder(x, y, w, imageID, p){
    
    //Pick random direction to face
    randomDir();
    resetTicks();
    
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
////////////////////////////////////////////////////////////////////////////////////
//*******************************GRASSHOPPER METHODS********************************
////////////////////////////////////////////////////////////////////////////////////

Grasshopper::Grasshopper(int x, int y, StudentWorld * w, int imageID, int p):Insect(x, y, w, imageID, p){
    
    resetDistance();
    
}

Grasshopper::~Grasshopper(){}

void Grasshopper::doSomething(){}



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

////////////////////////////////////////////////////////////////////////////////////
//**************************BABY GRASSHOPPER METHODS********************************
////////////////////////////////////////////////////////////////////////////////////
BabyGrasshopper::BabyGrasshopper(int x, int y, StudentWorld * w, int imageID, int p): Grasshopper(x, y, w, imageID, p){

}

BabyGrasshopper::~BabyGrasshopper(){}

void BabyGrasshopper::doSomething(){

    subtractPoints(1);
    
    
    
    if (getPoints()<1){
        //add 100 units of food at current location
        setDead();
        return;
    }
    
    if (ticksToSleep()>0){
        sub1Tick();
        return;
    }
    
    
    
    //Otherwise, grasshopper will do something this round
    
    //FOR NOW, NO Change to adult
//    if (getPoints()>=1600){
//        //Turn into adult grasshopper
//        //Add grasshopper in same location
//        //Set baby status to dead
//        return;
//    }
    
    //Attempt to eat food, not for part 1
    
    
    //Continue walking
    if (distanceToWalk()==0){
        randomDir();
        resetDistance();
    }
    
    
    if(moveOne())
        sub1Walk();
    else
        setDistanceZero();
    
    resetTicks();
    

}


void BabyGrasshopper::stun(){
    addTicks(2);
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

Water::Water(int x, int y, StudentWorld * w): Object(x, y, w, IID_WATER_POOL){};

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
    
    
    getWorld()->getInsects(getX(), getY(), onSquare);
    
    for (int i = 0; i<onSquare.size(); i++){
        if (!wasPrevious(onSquare[i]))
            onSquare[i]->stun();
    }
    
    previous = onSquare;
    
}


















