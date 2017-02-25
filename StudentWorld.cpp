#include "StudentWorld.h"
#include "Field.h"
#include "Actor.h"
#include <string>
#include <iostream>
#include <list>
#include <vector>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetDir): GameWorld(assetDir){}

StudentWorld::~StudentWorld(){
    cleanUp();
}

int StudentWorld::init()
    {
        
        tickCount = 0;
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
        //Temp for testing
        setGameStatText(to_string(tickCount));
        if (tickCount==2000){
            //setWinner()
            //return GWSTATUS_PLAYER_WON if there is a winner
            return GWSTATUS_NO_WINNER;
        }
        
        return GWSTATUS_CONTINUE_GAME;
    }
    
void StudentWorld::cleanUp()
{
        for (int x=0; x<VIEW_WIDTH; x++){
            for (int y=0; y<VIEW_HEIGHT; y++){
                while (!world[x][y].empty()){
                    delete world[x][y].front();
                    world[x][y].pop_front();
                }
            }
        }
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
                //case Field::poison:
                    //a = new Poison(x,y);
                    //world[x][y].push_front(a);
                    //break;
                //All anthill cases
                    
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
    



bool StudentWorld::foodAt(int x, int y, Food * p){
    list<Actor *> li = world[x][y];
    
    list<Actor *>::iterator i = li.begin();
    
    Actor *a;
    while (i != li.end()){
        a = *i;
        p = dynamic_cast<Food*>(a);
        if (p!=nullptr)
            //Food is present
            return true;
        i++;
    }
    
    return false;
}

int StudentWorld::eatFood(int x, int y, int amount, Food * a){
    Food * p;
    
    if (!foodAt(x, y, p))
        return 0;
    
    
    return p->pickUpFood(amount);
    
}

void StudentWorld::depositFood(int x, int y, int amount){
    Food *p;
    
    if (foodAt(x, y, p)){
        p->addFood(amount);
    }
    else{
        p = new Food(x, y, this, amount);
    }
}


void StudentWorld::getInsects(int x, int y, std::vector<Insect*> &v){
    list<Actor *> li = world[x][y];
    
    list<Actor *>::iterator i = li.begin();
    
    
    while (i != li.end()){
        if ((*i)->canMove()){
            Insect * p = dynamic_cast<Insect*>(*i);
            v.push_back(p);
        }
            i++;
    }

    
}



    
    


