#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp



////////////////////////////////////////////////////////////////////////////////////
//*******************************ACTOR METHODS*************************************
////////////////////////////////////////////////////////////////////////////////////
Actor::Actor(int x, int y, int imageID, Direction dir = right, unsigned int depth = 1): GraphObject(imageID, x, y, dir){

        m_isAlive = true;
}

Actor::~Actor(){}


////////////////////////////////////////////////////////////////////////////////////
//*******************************PEBBLE METHODS*************************************
////////////////////////////////////////////////////////////////////////////////////

Pebble::Pebble(int x, int y):Actor(x, y, IID_ROCK, right){}

Pebble::~Pebble(){}

void Pebble::doSomething(){}


////////////////////////////////////////////////////////////////////////////////////
//*******************************INSECT METHODS*************************************
////////////////////////////////////////////////////////////////////////////////////

Insect::Insect(int x, int y, int imageID, int p): Actor(x, y, imageID){
    
    //Pick random direction to face
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



////////////////////////////////////////////////////////////////////////////////////
//*******************************GRASSHOPPER METHODS********************************
////////////////////////////////////////////////////////////////////////////////////

Grasshopper::Grasshopper(int x, int y, int imageID = IID_ADULT_GRASSHOPPER, int p = 1600):Insect(x, y, imageID, p){

    
    isAsleep = false;
    
    int distance = randInt(2, 10);
    
    
}

Grasshopper::~Grasshopper(){}

void Grasshopper::doSomething(){}


////////////////////////////////////////////////////////////////////////////////////
//**************************BABY GRASSHOPPER METHODS********************************
////////////////////////////////////////////////////////////////////////////////////
BabyGrasshopper::BabyGrasshopper(int x, int y, int imageID = IID_BABY_GRASSHOPPER, int p=500): Grasshopper(x, y, imageID, p){

}

BabyGrasshopper::~BabyGrasshopper(){}

void BabyGrasshopper::doSomething(){}
























