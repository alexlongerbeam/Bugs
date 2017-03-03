#include "Actor.h"
#include "StudentWorld.h"
#include "Compiler.h"
#include <iostream>
#include <vector>
#include <cmath>
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

void Actor::coordInFront(int &x, int &y){
    int curX = getX();
    int curY = getY();
    
    switch(getDirection()){
        case GraphObject::up:
            x = curX;
            y = curY+1;
            break;
        case GraphObject::right:
            x = curX+1;
            y = curY;
            break;
        case GraphObject::down:
            x = curX;
            y = curY-1;
            break;
        case GraphObject::left:
            x = curX-1;
            y = curY;
            break;
        case GraphObject::none:
            cerr<<"Direction Error"<<endl;
    }
}

bool Actor::isEnemy(int colony){
    return false;
}

bool Actor::isDangerous(int colony){
    return false;
}

bool Actor::isAnthill(int colony){
    return false;
}

bool Actor::isPheromone(int colony){
    return false;
}

void Actor::getBitten(int amount){}
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
        addAnt();
        subtractPoints(1500);
    }
}

bool Anthill::isAnthill(int colony){
    return m_colonyNum==colony;
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
    getWorld()->addAntNum(m_colonyNum);
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

int Pheromone::getColony(){
    return m_colonyNum;
}

bool Pheromone::isPheromone(int colony){
    if (!isAlive())
        return false;
    return m_colonyNum==colony;
}

void Pheromone::increaseScent(int amount){
    if ((getPoints()+amount)<=768)
        addPoints(amount);
    else
        addPoints(768-getPoints());
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
    //cerr<<"DEATH"<<endl;
    getWorld()->depositFood(getX(), getY(), 100);
    setDead();
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


bool Insect::isDangerous(int colony){
    return isAlive();
}

bool Insect::isEnemy(int colony){
    return isAlive();
}
////////////////////////////////////////////////////////////////////////////////////
//**********************************ANT METHODS*************************************
////////////////////////////////////////////////////////////////////////////////////

Ant::Ant(int x, int y, StudentWorld *w, Compiler * comp, int colony, int imageID, int p): Insect(x, y, w, imageID, p){
    
    //cerr<<"Ant Created: "<<colony<<endl;
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
    Compiler::Command cmd;
    bool changeCount = true;
    bool cont;
    for (int i = 0; i<10; i++){
        if (!m_compiler->getCommand(m_counter, cmd)){
            cerr<<"CompilerError"<<endl;
            setDead();
            return;
        }
        
        cont = runCommand(cmd, changeCount);
        
        if (changeCount)
            m_counter++;
        if (!cont)
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

bool Ant::isEnemy(int colony){
    if (!isAlive())
        return false;
    return (m_colonyNum!=colony);
}

bool Ant::isDangerous(int colony){
    if (!isAlive())
        return false;
    return (m_colonyNum!=colony);
}

void Ant::rotateClockwise(){
    switch(getDirection()){
        case GraphObject::up:
            setDirection(right);
            break;
        case GraphObject::right:
            setDirection(down);
            break;
        case GraphObject::down:
            setDirection(left);
            break;
        case GraphObject::left:
            setDirection(up);
            break;
        case GraphObject::none:
            cerr<<"Direction Error"<<endl;
            
    }
}

void Ant::rotateCounterClockwise(){
    switch(getDirection()){
        case GraphObject::up:
            setDirection(left);
            break;
        case GraphObject::right:
            setDirection(up);
            break;
        case GraphObject::down:
            setDirection(right);
            break;
        case GraphObject::left:
            setDirection(down);
            break;
        case GraphObject::none:
            cerr<<"Direction Error"<<endl;
            
    }
}

//returns true if doSomething should continue
bool Ant::runCommand(const Compiler::Command& c, bool &changeCount){
    //cerr<<c.lineNum<<": "<<c.text<<endl;
    //cerr<<foodHeld<<endl;
    changeCount = true;
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
            getWorld()->biteRandom(getX(), getY(), 15, this, m_colonyNum);
            return false;
        case Compiler::pickupFood:
            foodHeld += getWorld()->eatFood(getX(), getY(), max(400, 1800-foodHeld));
            return false;
        case Compiler::emitPheromone:
            getWorld()->addPheromone(getX(), getY(), m_colonyNum);
            return false;
        case Compiler::faceRandomDirection:
            randomDir();
            return false;
        case Compiler::rotateClockwise:
            rotateClockwise();
            return false;
        case Compiler::rotateCounterClockwise:
            rotateCounterClockwise();
            return false;
        case Compiler::generateRandomNumber:
            lastRandomNum = randInt(0, stoi(c.operand1) - 1);
            return true;
        case Compiler::goto_command:
            m_counter = stoi(c.operand1);
            changeCount = false;
            return true;
        case Compiler::if_command:
            changeCount = !evaluateIf(c);
            return true;
            
            
        default:
            cerr<<"Problem executing statements"<<endl;
            return false;
    }
    
    
}

//returns the outcome of the if statement
bool Ant::evaluateIf(const Compiler::Command& c){
    int x, y;
    
    int ifCode = stoi(c.operand1);
    bool outcome = false;
    switch(ifCode){
        case 0: //smellDanger
            coordInFront(x, y);
            if (getWorld()->isDanger(x, y, m_colonyNum))
                outcome = true;
            break;
        case 1: //smellPheromone
            coordInFront(x, y);
            if (getWorld()->isPheromone(x, y, m_colonyNum))
                outcome = true;
            break;
        case 2: //wasBit
            if (previouslyBitten)
                outcome = true;
            break;
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
            if (getWorld()->isEnemy(getX(), getY(), m_colonyNum))
                outcome=true;
            break;
        case 8: //wasBlockedbyMoving
            if (previouslyBlocked)
                outcome = true;
            break;
        case 9: //lastNumZero
            if (lastRandomNum==0)
                outcome = true;
            break;
            
        default:
            cerr<<"Error checking if statement"<<endl;
    }
    
    if (outcome)
        m_counter = stoi(c.operand2);
    
    return outcome;
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
    //cerr<<"Grasshopper bitten"<<endl;
    if (getPoints()<=0){
        die();
        return;
    }
    
    //50% chance of bite back
    bool cont = randInt(0, 1);
    if (cont){
        //cerr<<"Biting back"<<endl;
        getWorld()->biteRandom(getX(), getY(), 50, this);
    }
    
    
    
}

//returns whether or not doSomething should continue
bool Grasshopper::moveUnique(){
    //1 in 3 chance of trying to bite another
    int bite = randInt(0, 2);
    if (bite==0){
        if (getWorld()->biteRandom(getX(), getY(), 50, this))
            return false;
    }
    //1 in 10 chance of jumping within a circle
    int jump = randInt(0, 10);
    if (jump==0){
        int x;
        int y;
        if (circle(x, y)){
            //cerr<<"Moving"<<endl;
            moveTo(x, y);
            return false;
        }
        cerr<<"No spot to move"<<endl;
    
    }
    
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

bool Grasshopper::circle(int &x, int &y){
    vector<int> xC;
    vector<int> yC;
    int tempx;
    int tempy;
    int currX = getX();
    int currY = getY();
    
    for (int i = 1; i<11; i++){
        for (int d = 0; d<360; d+=5){
            tempx=currX + i*cos(d * (M_PI/180));
            tempy=currY + i*sin(d * (M_PI/180));
            if (tempx<=VIEW_WIDTH-1 && tempx>=0 && tempy<=VIEW_HEIGHT-1 && tempy>=0 && tempx!=currX && tempy!=currY){
                xC.push_back(tempx);
                yC.push_back(tempy);
            }
        }
        
    }
    
    //cerr<<xC.size()<<endl;
    bool done = false;
    while(!done){
        if (xC.empty())
            return false;
        int location = randInt(0, xC.size()-1);
        tempx = xC[location];
        tempy = yC[location];
        
        if (getWorld()->pebbleAt(tempx, tempy)){
            xC.erase(xC.begin()+location);
            yC.erase(yC.begin()+location);
            //cerr<<"Pebble At"<<endl;
        }
        else
            done = true;
    }
    
    x = tempx;
    y = tempy;
    
    return true;
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
    //cerr<<"Baby bitten"<<endl;
    if (getPoints()<=0)
        die();
}
////////////////////////////////////////////////////////////////////////////////////
//*******************************OBJECT METHODS*************************************
////////////////////////////////////////////////////////////////////////////////////

Object::Object(int x, int y, StudentWorld * w, int imageID, int depth, Direction dir): Actor(x, y, w, imageID, dir, depth){};

Object::~Object(){}



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

bool Water::isDangerous(int colony){
    return isAlive();
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



bool Poison::isDangerous(int colony){
    return isAlive();
}












