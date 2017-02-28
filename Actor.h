#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <vector>
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
const int BABY_GRASSHOPPER_START_HEALTH = 500;
const int ADULT_GRASSHOPPER_START_HEALTH = 1600;

class StudentWorld;
class Compiler;

class Actor: public GraphObject{
public:
    Actor(int x, int y, StudentWorld * w, int imageID, Direction dir = right, unsigned int depth = 1);
    virtual ~Actor();
    virtual void doSomething() = 0;
    virtual bool canMove() = 0;
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
    virtual bool canMove() = 0;
    
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
    virtual bool canMove();

    
    virtual void doSomething();
    int pickUpFood(int amount);
    void addFood(int amount);
};


class Anthill: public EnergyHolder{
public:
    Anthill(int x, int y, StudentWorld * w, Compiler * comp, int anthillNum, int p = 8999, unsigned int depth = 2);
    virtual ~Anthill();
    
    virtual void doSomething();
    virtual bool canMove();
    
private:
    Compiler * m_compiler;
    int m_colonyNum;
    
};

class Pheromone: public EnergyHolder{
public:
    Pheromone(int x, int y, StudentWorld * w, int colonyNum, int imageID, int p = 256, unsigned int depth = 2);
    virtual ~Pheromone();
    
    virtual void doSomething();
    virtual bool canMove();
private:
    int m_colonyNum;
};
class Insect: public EnergyHolder{
public:
    Insect(int x, int y, StudentWorld * w, int imageID, int p);
    virtual ~Insect();
    virtual void doSomething() = 0;
    virtual bool canMove();

    
    virtual void stun() = 0;
    virtual void poison() = 0;
    virtual void getBitten(int amount) = 0;
protected:
    void resetTicks();
    void sub1Tick();
    int ticksToSleep();
    void randomDir();
    bool moveOne();
    void addTicks(int t);
    void die();
    bool biteRandom(int damage);
    
    
private:
    
    int m_ticksToSleep;
    
};



class Grasshopper: public Insect{
public:
    Grasshopper(int x, int y, StudentWorld * w, int imageID= IID_ADULT_GRASSHOPPER, int p = ADULT_GRASSHOPPER_START_HEALTH);
    virtual ~Grasshopper();
    virtual void doSomething();
    
    virtual void stun();
    virtual void poison();
    virtual void getBitten(int amount);
protected:
    int distanceToWalk();
    void resetDistance();
    void sub1Walk();
    void setDistanceZero();
    bool beginningCommon();
    void endCommon();
    virtual bool moveUnique();
private:
    int m_distanceToWalk;
};

class BabyGrasshopper: public Grasshopper{
public:
    BabyGrasshopper(int x, int y, StudentWorld * w, int imageID = IID_BABY_GRASSHOPPER, int p = BABY_GRASSHOPPER_START_HEALTH);
    virtual ~BabyGrasshopper();
    
    
    virtual void stun();
    virtual void poison();
    virtual void getBitten(int amount);
    
    virtual bool moveUnique();
};






class Object: public Actor{
public:
    Object(int x, int y, StudentWorld * w, int imageID, int depth = 2, Direction dir = right);
    virtual ~Object();
    
    
    virtual bool canMove();
    virtual void doSomething() = 0;
    virtual void attack();
};



class Pebble: public Object{
public:
    Pebble(int x, int y, StudentWorld * w);
    virtual ~Pebble();
    virtual void doSomething();
    
};

class Water: public Object{
public:
    Water(int x, int y, StudentWorld * w);
    virtual ~Water();
    
    virtual void doSomething();
    
private:
    bool wasPrevious(Insect * p);
    std::vector<Insect *> previous;
    
};

class Poison: public Object{
public:
    Poison(int x, int y, StudentWorld *w);
    virtual ~Poison();
    
    virtual void doSomething();
};






#endif // ACTOR_H_
