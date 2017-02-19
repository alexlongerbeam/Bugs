#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp


class Actor: public GraphObject{
public:
    Actor(int x, int y, int imageID, Direction dir, unsigned int depth);
    virtual ~Actor();
    virtual void doSomething() = 0;
    bool isAlive();
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
    
    
private:
    int m_points;
};



class Grasshopper: public Insect{
public:
    Grasshopper(int x, int y, int imageID, int p);
    virtual ~Grasshopper();
    virtual void doSomething();
    
private:
    bool isAsleep;
};

class BabyGrasshopper: public Grasshopper{
public:
    BabyGrasshopper(int x, int y, int imageID, int p);
    virtual ~BabyGrasshopper();
    virtual void doSomething();
};
#endif // ACTOR_H_
