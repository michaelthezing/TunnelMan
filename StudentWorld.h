

#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GraphObject.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>

class Actor;
class TunnelMan;
class Earth;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);

    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void incActor(Actor* i);
    void showObjects(int a, int b, double i);
    void incGoodie();
    void deleteBarrels();
    void remove();
    int beAnnoying(Actor* i, int num, double r);
    bool AllowedDirection(Actor* a, GraphObject::Direction dir, int r = 1) const;
    bool bribe(Actor* a, double r);
    bool deletePart(int x, int y);
    void killEnemy();
    void shoot();
    void gettingAnnoyed();
    GraphObject::Direction MoveToLocation(int x, int y, int num);
    void giveGold();
    void giveSonar();
    void giveWater();
    GraphObject::Direction MoveToExit(int x, int y);
    bool correctMove(Actor* a, int x, int y) const;

    bool nearTo(Actor* i, double k) const;
    bool looking(Actor* i) const;
    GraphObject::Direction inFrontOf(Actor* a) const;
private:
    TunnelMan* m_player;
    Earth* m_earth[VIEW_HEIGHT][VIEW_WIDTH];
    std::vector<Actor*> actors;
    int numBarrels;

    int awaitingTicks;
    int m_ticks;
    int protestCountTick;
    int chanceOfProtest;
    int chanceOfHard;
    int exitMaze[VIEW_HEIGHT - 3][VIEW_WIDTH - 3];
    void setDisplayText();
    std:: string scores(int level, int lives, int health, int water, int gold, int barrels, int sonar, int score) const;
    std::string statistics(int stat, int numbers, char ch = ' ') const;
    void choose(int& x, int& y, int xa = 0, int xb = VIEW_WIDTH - 4, int ya = 20, int yb = 56);
    bool completed() const;

    bool nearEnough(int xa, int ya, int xb, int yb, double k) const;
    bool noEarth(int x, int y) const;
    bool positionOnEarth(int x, int y) const;
    void upgrade(int maze[][VIEW_WIDTH - 3], int x, int y);
    void valid(int& x, int& y);
    GraphObject::Direction first(int maze[][VIEW_WIDTH - 3], int x, int y, int max = VIEW_WIDTH * VIEW_HEIGHT);
    void nearDigger(Actor* i, int j, int& a, int& b);
    double length(int xa, int ya, int xb, int yb) const;
    int m_mazeForTunnelMan[VIEW_HEIGHT - 3][VIEW_WIDTH - 3];



    class area
     {
     private:
         int m_row;
         int m_col;
     public:
         area(int rows, int cols) : m_row(rows),m_col(cols){

         }
         int row()const
         {
             return m_row;
         }
         int col() const {
             return m_col;
         }
     };
    

};
int randomNumberGenerator(int a, int b);

#endif

