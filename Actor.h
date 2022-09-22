
#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Actor

class Actor: public GraphObject
{
public:
    Actor(StudentWorld* world, int imageID, bool visible, int x, int y, Direction dir, double numsSize, unsigned int depth);
    virtual ~Actor();
    virtual void doSomething() = 0;
    void isDead();
    virtual bool isAnnoyed(unsigned int amountt);
    bool takeAStep(Direction dir);
    StudentWorld* getWorld() const;
    virtual bool canBlock() const;
    virtual bool canDig() const;
    virtual bool hunts() const;
    bool isNotDead() const;
    bool moveToXandY(int x, int y);

private:
    StudentWorld* m_world;
    bool notDead;
};


class Earth : public Actor
{
public:
    Earth(StudentWorld* world, int x, int y);
    virtual ~Earth();
    virtual void doSomething();
};

class Alive : public Actor
{
public:
    Alive(StudentWorld* world, int imageID, int x, int y, int health, Direction dir);
    virtual ~Alive();
    int getHealth() const;
    virtual void incGold() = 0;
    virtual bool isAnnoyed(unsigned int amountt);
private:
    virtual void endGame() = 0;
    int m_health;
};

class TunnelMan: public Alive
{
public:
    TunnelMan(StudentWorld* world);
    virtual ~TunnelMan();
    virtual void doSomething();
    virtual void incGold();
    unsigned int getGold() const;
    void incSonar();
    unsigned int getSonar() const;
    void incWater();
    unsigned int getWater() const;
    virtual bool canDig() const;

private:
    virtual void endGame();
    int goldNum;
    int sonarNum;
    int waterNum;
};

class Protester : public Alive
{
public:
    Protester(StudentWorld* world, int imageID = TID_PROTESTER, unsigned int health = 5, int score = 100);
    virtual ~Protester();
    virtual void doSomething();
    virtual void incGold();
    virtual bool isAnnoyed(unsigned int amount);
    virtual bool hunts() const;
protected:
    void tickAmount(int ticks);
private:
    Direction randDir() const;
    Direction perpDir(Direction dir);
    virtual bool tech();
    int ticksWhenStunning;
    int amtofMoveTicks;
    int ticksWhenStable;
    int shoutingTicks;
    bool m_leave;
    int forwardTicks;
    int ticksWhenPerp;
    int scoreInc;
    virtual void endGame();
};


class HardCoreProtester : public Protester
{
public:
    HardCoreProtester(StudentWorld* world);
    virtual ~HardCoreProtester();
    virtual void incGold();
private:
    virtual bool tech();

};

class Boulder : public Actor
{
public:
    Boulder(StudentWorld* world, int startX, int startY);
    virtual ~Boulder();
    virtual void doSomething();
    virtual bool canBlock() const;
private:
    int position;
    int boulderWaitingTick;
};

class Squirt : public Actor
{
public:
    Squirt(StudentWorld* world, int x, int y, Direction dir);
    virtual ~Squirt();
    virtual void doSomething();
private:
    int distanceSquirt;
};

class Goodie : public Actor
{
public:
    Goodie(StudentWorld* world, int imageID, bool visible, int x, int y);
    virtual ~Goodie();
    virtual void doSomething();
protected:
    void setTick(unsigned int ticks);
    void removeTicks();
private:
    virtual void  incPoints() = 0;
    int numTicks;
    virtual void sound() const;
};
class Barrel : public Goodie
{
public:
    Barrel(StudentWorld* world, int x, int y);
    virtual ~Barrel();
private:
    virtual void sound() const;
    virtual void incPoints();
};

class Gold : public Goodie
{
public:
    Gold(StudentWorld* world, int x, int y, bool temp);
    virtual ~Gold();
    virtual void doSomething();
private:
    virtual void incPoints();
    bool state;
};

class SonarKit : public Goodie
{
public:
    SonarKit(StudentWorld* world);
    virtual ~SonarKit();
    virtual void doSomething();
private:
    virtual void incPoints();
};

class WaterPool : public Goodie
{
public:
    WaterPool(StudentWorld* world, int x, int y);
    virtual ~WaterPool();
    virtual void doSomething();
private:
    virtual void incPoints();
};

#endif // ACTOR_H_



