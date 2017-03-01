#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <list>
#include <vector>
#include "Actor.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Compiler;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    virtual ~StudentWorld();

    virtual int init();
	

    virtual int move();
	

    virtual void cleanUp();
	
    bool pebbleAt(int x, int y);
    
    bool foodAt(int x, int y);
    
    int eatFood(int x, int y, int amount);
    
    void depositFood(int x, int y, int amount);
    
    void getInsects(int x, int y, std::vector<Insect*> &v, Actor* source);
    
    void newActor(int x, int y, Actor * a);
    
    void addAntNum(int colony);
    
    void addPheromone(int x, int y, int colony);
    
    bool onAnthill(int x, int y, int colony);
private:
    bool loadField(std::vector<Compiler *> compilers);
    void moveAll();
    void actorMoved(Actor * a, int oldX, int oldY);
    void checkDead();
    bool foodAt(int x, int y, Food *&p);
    
    std::list<Actor*> world[VIEW_HEIGHT][VIEW_WIDTH];
    int tickCount;
    std::vector<int> antNums;
};

#endif // STUDENTWORLD_H_
