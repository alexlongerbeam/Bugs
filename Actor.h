#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
const int BABY_GRASSHOPPER_START_HEALTH = 500;
const int ADULT_GRASSHOPPER_START_HEALTH = 1600;

class StudentWorld;

class Actor: public GraphObject{
public:
    Actor(int x, int y, StudentWorld * w, int imageID, Direction dir = right, unsigned int depth = 1);
    virtual ~Actor();
    virtual void doSomething() = 0;
    bool isAlive();
    bool isActive();
    void reActivate();
    void moved();
    
protected:
    void setDead();
    StudentWorld* getWorld();
private:
    bool m_isAlive;
    bool m_active;
    StudentWorld *m_world;
};


class EnergyHolder: public Actor {
public:
    EnergyHolder(int x, int y, StudentWorld * w, int imageID, int p, unsigned int depth = 1);
    virtual ~EnergyHolder();
    virtual void doSomething() = 0;
    
    void addPoints(int add);
    void subtractPoints(int sub);
    int getPoints();
    
private:
    int m_points;
    
};

class Food: public EnergyHolder{
public:
    Food(int x, int y, StudentWorld* w, int p = 1600);
    virtual ~Food();
    
    virtual void doSomething();
    int pickUpFood(int amount);
    void addFood(int amount);
};

class Insect: public EnergyHolder{
public:
    Insect(int x, int y, StudentWorld * w, int imageID, int p);
    virtual ~Insect();
    virtual void doSomething() = 0;
    

protected:
    void resetTicks();
    void sub1Tick();
    int ticksToSleep();
    void randomDir();
    bool moveOne();
    
    
private:
    
    int m_ticksToSleep;
    
};



class Grasshopper: public Insect{
public:
    Grasshopper(int x, int y, StudentWorld * w, int imageID= IID_ADULT_GRASSHOPPER, int p = ADULT_GRASSHOPPER_START_HEALTH);
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
    BabyGrasshopper(int x, int y, StudentWorld * w, int imageID = IID_BABY_GRASSHOPPER, int p = BABY_GRASSHOPPER_START_HEALTH);
    virtual ~BabyGrasshopper();
    virtual void doSomething();
};






class Object: public Actor{
public:
    Object(int x, int y, StudentWorld * w, int imageID, int depth = 2, Direction dir = right);
    virtual ~Object();
    
    virtual void doSomething() = 0;
    virtual void attack();
};



class Pebble: public Object{
public:
    Pebble(int x, int y, StudentWorld * w);
    virtual ~Pebble();
    virtual void doSomething();
    
    
    
};
#endif // ACTOR_H_
