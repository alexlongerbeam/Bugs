#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <list>
#include <vector>
#include "Actor.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    virtual ~StudentWorld();

    virtual int init();
	

    virtual int move();
	

    virtual void cleanUp();
	
    bool pebbleAt(int x, int y);
    bool foodAt(int x, int y, Food *&p);
    
    int eatFood(int x, int y, int amount);
    
    void depositFood(int x, int y, int amount);
    
    void getInsects(int x, int y, std::vector<Insect*> &v, Actor* source);
    
    void newActor(int x, int y, Actor * a);
    
private:
    bool loadField();
    std::list<Actor*> world[VIEW_HEIGHT][VIEW_WIDTH];
    int tickCount;
    void moveAll();
    void actorMoved(Actor * a, int oldX, int oldY);
    void checkDead();
};

#endif // STUDENTWORLD_H_
