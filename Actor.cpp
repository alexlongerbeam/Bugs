#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
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
//*******************************PEBBLE METHODS*************************************
////////////////////////////////////////////////////////////////////////////////////

Pebble::Pebble(int x, int y, StudentWorld * w):Actor(x, y, w, IID_ROCK, right){}

Pebble::~Pebble(){}

void Pebble::doSomething(){}


////////////////////////////////////////////////////////////////////////////////////
//*******************************INSECT METHODS*************************************
////////////////////////////////////////////////////////////////////////////////////

Insect::Insect(int x, int y, StudentWorld * w, int imageID, int p): Actor(x, y, w, imageID){
    
    //Pick random direction to face
    randomDir();
    m_points = p;
}

Insect::~Insect(){}



void Insect::addPoints(int add){
    m_points += add;
}
void Insect::subtractPoints(int sub){
    m_points -= sub;
}
int Insect::getPoints(){
    return m_points;
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
        std::cerr<<"Ticks: "<<getWorld()->tickCount<<"  Points: "<<getPoints()<<std::endl;
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
























