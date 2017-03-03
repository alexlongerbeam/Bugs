#include "StudentWorld.h"
#include "Field.h"
#include "Actor.h"
#include "Compiler.h"
#include <string>
#include <iostream>
#include <list>
#include <vector>
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetDir): GameWorld(assetDir){}

StudentWorld::~StudentWorld(){}

int StudentWorld::init()
    {
        tickCount = 0;
        winner = -1;
        
        vector<string> fileNames = getFilenamesOfAntPrograms();
        
        compilerPtrs[0] = new Compiler;
        compilerPtrs[1] = new Compiler;
        compilerPtrs[2] = new Compiler;
        compilerPtrs[3] = new Compiler;
        string error;
        numColonies = fileNames.size();
        for (int i = 0; i<fileNames.size(); i++){
            if (!compilerPtrs[i]->compile(fileNames[i], error)){
                setError(fileNames[i] + " " + error);
                cerr<<error<<endl;
                
                return GWSTATUS_LEVEL_ERROR;
            }
            
            antNames.push_back(compilerPtrs[i]->getColonyName());
            antNums.push_back(0);
        }
        
        if (!loadField()){
            cerr<<"Error loading field"<<endl;
            return GWSTATUS_LEVEL_ERROR;
        }
        
    
        return GWSTATUS_CONTINUE_GAME;
    }
    
int StudentWorld::move()
    {
        tickCount++;
        moveAll();
        checkDead();
        setGameStatText(statusText());
        if (tickCount==2000){
            if (setWinner())
                return GWSTATUS_PLAYER_WON;
            else
                return GWSTATUS_NO_WINNER;
        }
        
        return GWSTATUS_CONTINUE_GAME;
    }
    
void StudentWorld::cleanUp()
{
    list<Actor *>::iterator i;
        for (int x=0; x<VIEW_WIDTH; x++){
            for (int y=0; y<VIEW_HEIGHT; y++){
                i = world[x][y].begin();
                while(i!=world[x][y].end()){
                    delete (*i);
                    i++;
                
                }
            }
        }
    

    
    for (int i = 0; i<4; i++){
        delete compilerPtrs[i];
    }
}

bool StudentWorld::setWinner(){
    if (winner==-1)
        return false;
    GameWorld::setWinner(antNames[winner]);
    return true;
}

int StudentWorld::getWinningNumber(){
    int maxIndex = -1;
    int max = 5;
    for (int i = 0; i<antNums.size(); i++){
        if (antNums[i]>max){
            maxIndex = i;
            max = antNums[i];
        }
    }
    if (winner == -1)
        winner = maxIndex;
    else
        if (max>antNums[winner])
            winner = maxIndex;
    
    return winner;
    
}

string StudentWorld::statusText(){
    int winning = getWinningNumber();
    ostringstream s;
    
    s<<"Ticks:";
    //cerr<<"******************"<<antNums[0]<<endl;
    s<<setw(5)<<tickCount;
    s<<" - ";
    for (int i = 0; i<antNames.size(); i++){
        s<<antNames[i];
        if (i==winning)
            s<<"*";
        s<<": ";
        s.fill('0');
        s<<setw(2)<<antNums[i];
        s<<" ants  ";
    }
    
    return s.str();
}

bool StudentWorld::loadField(){
        Field f;
        string fieldFile = getFieldFilename();
        string error;
        if (f.loadField(fieldFile, error) != Field::LoadResult::load_success) {
            cerr<<"Error: "<<error<<endl;
            setError(fieldFile + " " + error);
            return false; // something bad happened!
        }
    
    for (int x=0; x<VIEW_WIDTH; x++){
        for (int y=0; y<VIEW_HEIGHT; y++){
            Field::FieldItem item = f.getContentsOf(x,y);
            Actor * a;
            switch (item) {
                case Field::rock:
                    a = new Pebble(x,y, this);
                    world[x][y].push_front(a);
                    break;
                case Field::grasshopper:
                    a = new BabyGrasshopper(x,y, this);
                    world[x][y].push_front(a);
                    break;
                case Field::food:
                    a = new Food(x,y,this);
                    world[x][y].push_front(a);
                    break;
                case Field::water:
                    a = new Water(x, y, this);
                    world[x][y].push_front(a);
                    break;
                case Field::poison:
                    a = new Poison(x,y, this);
                    world[x][y].push_front(a);
                    break;
                //All anthill cases
                case Field::anthill0:
                    if (numColonies>0){
                        a = new Anthill(x, y, this, compilerPtrs[0], 0);
                        world[x][y].push_front(a);
                    }
                    break;
                case Field::anthill1:
                    if (numColonies>1){
                        a = new Anthill(x, y, this, compilerPtrs[1], 1);
                        world[x][y].push_front(a);
                    }
                    break;
                case Field::anthill2:
                    if (numColonies>2){
                        a = new Anthill(x, y, this, compilerPtrs[2], 2);
                        world[x][y].push_front(a);
                    }
                    break;
                case Field::anthill3:
                    if (numColonies>3){
                        a = new Anthill(x, y, this, compilerPtrs[3], 3);
                        world[x][y].push_front(a);
                    }
                    break;
                    
                case Field::empty:
                default:
                    break;
            }
            
        }
    }
return true;
}


void StudentWorld::moveAll(){
    //Vector to keep track of objects that moved for quick reactivation
    vector<Actor *> moved;
    
    
    for (int x=0; x<VIEW_WIDTH; x++){
        for (int y=0; y<VIEW_HEIGHT; y++){
            Actor *a;
            list<Actor *>::iterator i = world[x][y].begin();
            while (i != world[x][y].end()){
                a = *i;
                if (a->isAlive()&&a->isActive()){
                    a->doSomething();
                    if (a->getX()!=x || a->getY()!=y){
                        i--;
                        moved.push_back(a); //adds to list of objects that have moved
                        a->moved(); //Makes it so it won't move again
                        actorMoved(a, x, y);
                        
                    }
                }
                i++;
            }
        }
    }
    
    //Reactivate all that moved
    
    vector<Actor *>::iterator i = moved.begin();
    
    while (i != moved.end()){
        (*i)->reActivate();
        i++;
    }
    
    
    
}


void StudentWorld::checkDead(){
    for (int x=0; x<VIEW_WIDTH; x++){
        for (int y=0; y<VIEW_HEIGHT; y++){
            Actor *a;
            list<Actor *>::iterator i = world[x][y].begin();
            while (i != world[x][y].end()){
                a = (*i);
                if (!a->isAlive()){
                    world[x][y].erase(i);
                    delete a;
                    i = world[x][y].begin();
                }
                else
                    i++;
            }
        }
    }
}

void StudentWorld::actorMoved(Actor * a, int oldX, int oldY){
    //Find a in the old location
    list<Actor *>::iterator i = world[oldX][oldY].begin();
    
    while (i != world[oldX][oldY].end()){
        if (*i==a)
            break;
        i++;
    }
    
    if (i==world[oldX][oldY].end())
        cerr<<"Error with switching actor location"<<endl;
    
    //Erase from old list
    world[oldX][oldY].erase(i);
    
    //Add to new list
    world[a->getX()][a->getY()].push_back(a);
    
    
}

bool StudentWorld::pebbleAt(int x, int y){
    list<Actor *> li = world[x][y];
    
    list<Actor *>::iterator i = li.begin();
    
    Actor *a;
    while (i != li.end()){
        a = *i;
        Pebble *p = dynamic_cast<Pebble*>(a);
        if (p!=nullptr)
            //Pebble is present
            return true;
        i++;
    }
    
    return false;
}
    



bool StudentWorld::foodAt(int x, int y, Food *& p){
    list<Actor *> li = world[x][y];
    
    list<Actor *>::iterator i = li.begin();
    
    Actor *a;
    while (i != li.end()){
        a = *i;
        p = dynamic_cast<Food*>(a);
        if (p!=nullptr){
            //Food is present
            return true;
        }
        i++;
    }
    
    return false;
}

int StudentWorld::eatFood(int x, int y, int amount){
    Food * p;
    
    if (!foodAt(x, y, p))
        return 0;
    

    return p->pickUpFood(amount);
    
}

void StudentWorld::depositFood(int x, int y, int amount){
    Food *p;
    
    if (amount==0)
        return;
    
    
    if (foodAt(x, y, p)){
        p->addFood(amount);
    }
    else{
        Actor * a = new Food(x, y, this, amount);
        newActor(x, y, a);
    }
}


void StudentWorld::getInsects(int x, int y, std::vector<Insect*> &v, Actor* source){
    list<Actor *> li = world[x][y];
    
    list<Actor *>::iterator i = li.begin();
    
    //If source is an insect, make sure not to include it
    if (source->canMove()){
        while (i != li.end()){
            if ((*i)->canMove() && ((*i)!=source)){
                Insect * p = dynamic_cast<Insect*>(*i);
                v.push_back(p);
            }
                i++;
        }
    }
    //otherwise every insect can be included
    else{
        while (i != li.end()){
            if ((*i)->canMove()){
                Insect * p = dynamic_cast<Insect*>(*i);
                v.push_back(p);
            }
            i++;
        }
    }
}


void StudentWorld::newActor(int x, int y, Actor *a){
    world[x][y].push_back(a);
}
    
    




void StudentWorld::addAntNum(int colony){
    antNums[colony] += 1;
}


bool StudentWorld::foodAt(int x, int y){
    Food *p;
    
    return foodAt(x, y, p);

}



bool StudentWorld::isPheromone(int x, int y, int colony){
    list<Actor *> li = world[x][y];
    
    list<Actor *>::iterator i = li.begin();
    
    
    while (i != li.end()){
        if ((*i)->isPheromone(colony))
            return true;
        i++;
    }
    
    return false;
}

bool StudentWorld::isDanger(int x, int y, int colony){
    list<Actor *> li = world[x][y];
    
    list<Actor *>::iterator i = li.begin();
    
    
    while (i != li.end()){
        if ((*i)->isDangerous(colony))
            return true;
        i++;
    }
    
    return false;
}

bool StudentWorld::isEnemy(int x, int y, int colony){
    list<Actor *> li = world[x][y];
    
    list<Actor *>::iterator i = li.begin();
    
    
    while (i != li.end()){
        if ((*i)->isEnemy(colony))
            return true;
        i++;
    }
    
    return false;
}

bool StudentWorld::biteRandom(int x, int y, int damage, Actor* source, int colony){
    list<Actor *> li = world[x][y];
    vector<Actor *> v;
    
    list<Actor *>::iterator i = li.begin();
    
    
    while (i != li.end()){
        if ((*i)->isEnemy(colony) && (*i)!=source)
            v.push_back((*i));
        i++;
    }
    
    if (v.empty())
        return false;
    
    
    int insect = randInt(0, v.size()-1);
    
    v[insect]->getBitten(damage);
    
    return true;
}

bool StudentWorld::getPheromone(int x, int y, int colony, Pheromone* p){
    list<Actor *> li = world[x][y];
    
    list<Actor *>::iterator i = li.begin();
    
    
    while (i != li.end()){
        if ((*i)->isPheromone(colony)){
            p = static_cast<Pheromone *>(*i);
            return true;
        }
        i++;
    }
    p = nullptr;
    return false;
}

void StudentWorld::addPheromone(int x, int y, int colony){
    Pheromone *p;
    
    if (getPheromone(x, y, colony, p)){
        p->increaseScent(256);
    }
    else{
        Actor *a = nullptr;
        switch (colony){
            case 0:
                a = new Pheromone(x, y, this, colony, IID_PHEROMONE_TYPE0);
                break;
            case 1:
                a = new Pheromone(x, y, this, colony, IID_PHEROMONE_TYPE1);
                break;
            case 2:
                a = new Pheromone(x, y, this, colony, IID_PHEROMONE_TYPE2);
                break;
            case 3:
                a = new Pheromone(x, y, this, colony, IID_PHEROMONE_TYPE3);
                break;
        
        }
        
        newActor(x, y, a);
        
    }
}


bool StudentWorld::onAnthill(int x, int y, int colony){
    list<Actor *> li = world[x][y];
    
    list<Actor *>::iterator i = li.begin();
    
    
    while (i != li.end()){
        if ((*i)->isAnthill(colony))
            return true;
        i++;
    }
    
    return false;
}

