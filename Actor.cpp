#include "Actor.h"
#include "StudentWorld.h"
#include "Compiler.h"
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


void Anthill::addAnt(){
    Actor * a;
    switch(m_colonyNum){
        case 0:
            a = new Ant(getX(), getY(), getWorld(), m_compiler, m_colonyNum, IID_ANT_TYPE0);
            break;
        case 1:
            a = new Ant(getX(), getY(), getWorld(), m_compiler, m_colonyNum, IID_ANT_TYPE1);
            break;
        case 2:
            a = new Ant(getX(), getY(), getWorld(), m_compiler, m_colonyNum, IID_ANT_TYPE2);
            break;
        case 3:
            a = new Ant(getX(), getY(), getWorld(), m_compiler, m_colonyNum, IID_ANT_TYPE3);
            break;
        default:
            cerr<<"Error creating Ant"<<endl;
            return;
    }
    
    getWorld()->newActor(getX(), getY(), a);
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


//returns whether or not the doSomething function should continue
bool Insect::beginningCommon(){
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

////////////////////////////////////////////////////////////////////////////////////
//**********************************ANT METHODS*************************************
////////////////////////////////////////////////////////////////////////////////////

Ant::Ant(int x, int y, StudentWorld *w, Compiler * comp, int colony, int imageID, int p): Insect(x, y, w, imageID, p){
    
    m_compiler = comp;
    m_colonyNum = colony;
    foodHeld = 0;
    previouslyBitten = false;
    previouslyBlocked = false;
    m_counter = 0;
    lastRandomNum = 0;
}

Ant::~Ant(){}

void Ant::doSomething(){
    if (!beginningCommon())
        return;
    bool cont;
    Compiler::Command cmd;
    
    for (int i = 0; i<10; i++){
        if (!m_compiler->getCommand(m_counter, cmd)){
            setDead();
            return;
        }
        m_counter++;
        //runCommand() could change m_counter
        if (!runCommand(cmd))
            return;
    }
    
}

void Ant::stun(){
    addTicks(2);
}
void Ant::poison(){
    subtractPoints(150);
    if (getPoints()<=0)
        die();
}
void Ant::getBitten(int amount){
    subtractPoints(amount);
    previouslyBitten = true;
    
    if (getPoints()<=0)
        die();
    
}

//returns true if doSomething should continue
bool Ant::runCommand(const Compiler::Command& c){
    
    switch(c.opcode){
        case Compiler::moveForward:
            if (moveOne()){
                previouslyBlocked = false;
                previouslyBitten = false;
            }
            else{
                previouslyBlocked = true;
            }
            return false;
        case Compiler::eatFood:
            if (foodHeld>=100){
                addPoints(100);
                foodHeld-=100;
            }
            else{
                addPoints(foodHeld);
                foodHeld = 0;
            }
            return false;
        case Compiler::dropFood:
            getWorld()->depositFood(getX(), getY(), foodHeld);
            foodHeld = 0;
            return false;
        case Compiler::bite:
            //CHANGE THIS TO WORK FOR ANTS
            biteRandom(15);
            return false;
        case Compiler::pickupFood:
            foodHeld += getWorld()->eatFood(getX(), getY(), max(400, 1800-foodHeld));
            return false;
        case Compiler:: emitPheromone:
            getWorld()->addPheromone(getX(), getY(), m_colonyNum);
            return false;
        case Compiler::faceRandomDirection:
            randomDir();
            return false;
        case Compiler::rotateClockwise:
        case Compiler::rotateCounterClockwise:
            return false;
        case Compiler::generateRandomNumber:
            lastRandomNum = randInt(0, stoi(c.operand1) - 1);
            return true;
        case Compiler::goto_command:
            m_counter = stoi(c.operand1);
            return true;
        case Compiler::if_command:
            evaluateIf(c);
            return true;
            
            
        default:
            cerr<<"Problem executing statements"<<endl;
            return false;
    }
    
    
}

void Ant::evaluateIf(const Compiler::Command& c){
    
    int ifCode = stoi(c.operand1);
    bool outcome = false;
    switch(ifCode){
        case 0: //smellDanger
        case 1: //smellPheromone
        case 2: //wasBit
        case 3: //amCarryingFood
            if (foodHeld>0)
                outcome = true;
            break;
        case 4: //amHungry
            if (getPoints()<=25)
                outcome = true;
            break;
        case 5: //amOnMyAnthill
            if (getWorld()->onAnthill(getX(), getY(), m_colonyNum))
                outcome = true;
            break;
        case 6: //AmOnFood
            if (getWorld()->foodAt(getX(), getY()))
                outcome = true;
            break;
        case 7: //amStandingWithEnemy
            //********Figure this out**************
            break;
        case 8: //wasBlockedbyMoving
        case 9: //lastNumZero
            if (lastRandomNum==0)
                outcome = true;
            break;
            
        default:
            cerr<<"Error checking if statement"<<endl;
    }
    
    if (outcome)
        m_counter = stoi(c.operand2);
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


bool Grasshopper::moveUnique(){
    //1 in 3 chance of trying to bite another
    int bite = randInt(0, 2);
    if (bite==0){
        if (biteRandom(50))
            return false;
    }
    
    //If it didn't decide to bite or wasn't able to bite
    cerr<<"IMPLEMENT GRASSHOPPER JUMP IN CIRCLE"<<endl;
    
    
    
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














