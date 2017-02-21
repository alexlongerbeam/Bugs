#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp


class Actor: public GraphObject{
public:
    Actor(int x, int y, int imageID, Direction dir = right, unsigned int depth = 1);
    virtual ~Actor();
    virtual void doSomething() = 0;
    bool isAlive();
    
protected:
    void setDead();
    
private:
    bool m_isAlive;
};

class Pebble: public Actor{
public:
    Pebble(int x, int y);
    virtual ~Pebble();
    virtual void doSomething();
    
};

class Insect: public Actor{
public:
    Insect(int x, int y, int imageID, int p);
    virtual ~Insect();
    virtual void doSomething() = 0;
    
    void addPoints(int add);
    void subtractPoints(int sub);
    int getPoints();
    
    
protected:
    void resetTicks();
    void sub1Tick();
    int ticksToSleep();
    void randomDir();
    bool moveOne();
    
    
private:
    int m_points;
    int m_ticksToSleep;
    
};



class Grasshopper: public Insect{
public:
    Grasshopper(int x, int y, int imageID= IID_ADULT_GRASSHOPPER, int p = 1600);
    virtual ~Grasshopper();
    virtual void doSomething();
    
protected:
    int distanceToWalk();
    void resetDistance();
    void sub1Walk();
    void setDistanceZero();
private:
    int m_distanceToWalk;
};

class BabyGrasshopper: public Grasshopper{
public:
    BabyGrasshopper(int x, int y, int imageID = IID_BABY_GRASSHOPPER, int p = 500);
    virtual ~BabyGrasshopper();
    virtual void doSomething();
};
#endif // ACTOR_H_
