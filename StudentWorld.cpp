#include "StudentWorld.h"
#include "Field.h"
#include "Actor.h"
#include <string>
#include <iostream>
#include <list>
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
        setGameStatText("Temporary");
        
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
            setError(fieldFile + " " + error);
            return false; // something bad happened!
        }
    
    
    for (int x=0; x<VIEW_WIDTH; x++){
        for (int y=0; y<VIEW_HEIGHT; y++){
            Field::FieldItem item = f.getContentsOf(x,y);
            
            Actor * a;
            
            switch (item) {
                case Field::rock:
                    a = new Pebble(x,y);
                    world[x][y].push_front(a);
                    break;
                case Field::grasshopper:
                    a = new BabyGrasshopper(x,y);
                    world[x][y].push_front(a);
                    break;
                //case Field::food:
                    //a = new Food(x,y);
                    //world[x][y].push_front(a);
                    //break;
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
    for (int x=0; x<VIEW_WIDTH; x++){
        for (int y=0; y<VIEW_HEIGHT; y++){
            Actor *a;
            list<Actor *>::iterator i = world[x][y].begin();
            while (i != world[x][y].end()){
                a = *i;
                if (a->isAlive()){
                    a->doSomething();
                    if (a->getX()!=x || a->getY()!=y){
                        actorMoved(a, x, y);
                    }
                }
            }
        }
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
                    i--;
                    delete a;
                }
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
    world[a->getX()][a->getY()].push_front(a);
    
    
}
    
    
    
    
    
    
    
    


