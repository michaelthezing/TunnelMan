
#include "Actor.h"
#include "StudentWorld.h"
#include <algorithm>
using namespace std;



const int STABLE = 0;
const int WAITING = 1;
const int FALLING = 2;
//constant variables for boulder

//initializes notbeing dead to true and sets world to m_world
Actor::Actor(StudentWorld* world, int imageID, bool visible, int x, int y, Direction dir, double numsSize, unsigned int depth)
    :GraphObject(imageID, x, y, dir, numsSize, depth)
{
    m_world = world;

    setVisible(visible);

    notDead = true;
}

//returns the earth
StudentWorld* Actor::getWorld() const
{
    return m_world;
}

Actor::~Actor(){} //no need for destructor

bool Actor::isAnnoyed(unsigned int amount)
{
    return false; //set to false because noy all actors can get annoyed
}


void Actor::isDead()
{
    notDead = false; //sets all of the obkects as dead to begin
}


//allows to take one step in the given direction
bool Actor::takeAStep(Direction dir)
{
    if(dir == up)
        {
            return  moveToXandY(getX(), getY() + 1);
        }
        if(dir == down)
        {
            return moveToXandY(getX(), getY() -1);
        }
        if(dir == right)
        {
            return moveToXandY(getX() + 1, getY());
        }
        if(dir == left)
        {
            return moveToXandY(getX() -1,  getY());
        }
        return false;
}


bool Actor::moveToXandY(int a, int b) //code moves the object to the coordinate if the corrdinate is moveable
{
    if ((*getWorld()).correctMove(this, a, b))
    {
        moveTo(a, b);
        return true;
    }
    return false;
}

bool Actor::isNotDead() const
{
    return notDead; //if person is alive use this
}


bool Actor::canBlock() const
{
    return false; //not every object can block
}

bool Actor::canDig() const
{ //not every  pbject can dig
    return false;
}

bool Actor::hunts() const
{
    return false; //not every object can hunt
}


Earth::Earth(StudentWorld* world, int x, int y)
    :Actor(world, TID_EARTH, true, x, y, right, 0.25, 3) //put appropriate dimensions and set image ID to TID_earth
{}
void Earth::doSomething() //earth does not do anything
{}

Earth::~Earth()
{} //no need for destriuctor




//this class is striclyy for living objects
Alive::Alive(StudentWorld* world, int imageID, int x, int y, int health, Direction dir)
    :Actor(world, imageID, true, x, y, dir, 1.0, 0)
{
    m_health = health;
}
Alive::~Alive(){} //no need


int Alive::getHealth() const
{
    return m_health; //returns the health of the pbject
}

bool Alive::isAnnoyed(unsigned int amount)
{
    m_health = m_health - amount;
    if (m_health <= 0)
    { //if the health is 0 then the obkject is dead
        endGame();
    }
    return true;
}

TunnelMan::TunnelMan(StudentWorld* world)
    :Alive(world, TID_PLAYER, 30, 60, 10, right) //takes the TID_player image id and spawns tunnelman in the right spot
{
    goldNum = 0; sonarNum = 1; waterNum = 5; //makes tunnelamn start with 0 gold, 1 sonar, and 5 water
    
}

TunnelMan::~TunnelMan(){} //no need

void TunnelMan::doSomething() //allows the tunnelman to succesfully use the said keys. this allows tunnleman to move and to do what he gotta do
{

    if(!isNotDead())
       {
           return;
       }
       int ch;
       if((*getWorld()).getKey(ch) == true)
       {
          if(ch == KEY_PRESS_UP)
          {
                  if(getDirection() != up)
                  {
                      setDirection(up);
                  }
                  else if(getY() == 60)
                  {
                      moveTo(getX(), getY());
                  }
                  else{
                      moveToXandY(getX(), getY() + 1);
                  }
          }
           if(ch == KEY_PRESS_DOWN)
           {

                  if(getDirection() != down)
                  {
                      setDirection(down);
                  }
                  else if(getY() == 0)
                  {
                      moveTo(getX(), getY());
                  }
                  else{
                      moveToXandY(getX(), getY() - 1);
                  }
           }

           if(ch == KEY_PRESS_RIGHT)
           {
                  if(getDirection() != right)
                  {
                      setDirection(right);
                  }
                  else if(getX() == 60)
                  {
                      moveTo(getX(), getY());
                  }
                  else{
                      moveToXandY(getX() + 1 , getY());
                  }

           }
           if(ch == KEY_PRESS_SPACE)
           {
               if(waterNum > 0)
               {
                   getWorld()->shoot();
                   waterNum--;
               }
           }

           if(ch == KEY_PRESS_LEFT)
           {

                  if(getDirection() != left)
                  {
                      setDirection(left);
                  }
                  else if(getX() == 61)
                  {
                      moveTo(getX(), getY());

                  }
                  else{
                      moveToXandY(getX() - 1, getY());
                  }

           }
           if(ch == 'z' || ch == 'Z')
           {
               if(sonarNum != 0)
               {
                  getWorld()->showObjects(getX(), getY(), 12.0);
                  getWorld()->playSound(SOUND_SONAR);
                  sonarNum--;
               }

           }


           if(ch == KEY_PRESS_ESCAPE)
           {
                  isDead();

           }
           if(ch == KEY_PRESS_TAB)
           {
                  if(goldNum > 0)
                  {
                      getWorld()->incActor(new Gold(getWorld(),getX(),getY(),true));
                      goldNum--;
                  }

           }

       }
    if((*getWorld()).deletePart(getX(), getY()))
    {
        (*getWorld()).playSound(SOUND_DIG);
    }
    (*getWorld()).showObjects(getX(), getY(), 4.0);
}


void TunnelMan::endGame()
{
    (*getWorld()).playSound(SOUND_PLAYER_GIVE_UP); //whenever tunnelman dies than play the sound and set him to dead to lose a life
    isDead();
}
void TunnelMan::incGold() //increase gold when its collected
{
    goldNum++;
}
void TunnelMan::incSonar() //increase sonar when its collected
{
    sonarNum++;
}
void TunnelMan::incWater() //incrwease water when its collected
{
    waterNum = waterNum + 5;
}
unsigned int TunnelMan::getGold() const
{
    return goldNum; //return the amount of gold
}
unsigned int TunnelMan::getWater() const
{
    return waterNum; //return the amount of water
}
unsigned int TunnelMan::getSonar() const
{
    return sonarNum; //return the amount of sonar
}

bool TunnelMan::canDig() const
{
    return true; //the tunnelamn is an object that can dig
}


Protester::Protester(StudentWorld* world, int imageID, unsigned int health, int score)
    :Alive(world, imageID, 60, 60, health, left) //starts the protester at 60,60
{
    forwardTicks= randomNumberGenerator(8, 60);
    amtofMoveTicks = max(0, 3 - (int)getWorld()->getLevel() / 4);
    ticksWhenStable = 0;

    scoreInc = score;
    ticksWhenPerp = 0;
    m_leave = false;
    ticksWhenStunning= max(50, 100 - (int)getWorld()->getLevel() * 10);
    shoutingTicks = 0;

}

Protester::~Protester() //no need
{}

void Protester::doSomething() //allows gthe protester to stun, makes hiom leave when he gets annnoyed, and implements the random direction functions to get the protester to move randomly
{
    if (!isNotDead())
    {

            return;
    }
    
        if (ticksWhenStable > 0)
        {
            ticksWhenStable--;
            return;
        }
        tickAmount(amtofMoveTicks);

    
    
    if (m_leave)
    {
        int x = getX();
        int y = getY();
        if (x == 60 && y == 60)
        {
            isDead();
            getWorld()->killEnemy();
        }
        else
        {
       
            Direction move = getWorld()->MoveToExit(x, y);
            setDirection(move);
            takeAStep(move);
        }
        return;
    }
        if (ticksWhenPerp < 200)
        {
            ticksWhenPerp++;
        }
        bool nearTun = getWorld()->nearTo(this, 4.0);
        if (nearTun && getWorld()->looking(this) && shoutingTicks== 0)
        {
            shoutingTicks = 15;
            getWorld()->playSound(SOUND_PROTESTER_YELL);
            getWorld()->gettingAnnoyed();
            return;
        }
        else if (shoutingTicks > 0)
        {
            shoutingTicks--;
            return;
        }
        if (!nearTun && tech())
        {
            return;
        }
        Direction toDigger = getWorld()->inFrontOf(this);
        if (!nearTun && toDigger != none)
        {
            setDirection(toDigger);
           takeAStep(getDirection());
            forwardTicks = 0;
            return;
        }

       forwardTicks--;
        Direction goingPerp =perpDir(getDirection());
        if (forwardTicks <= 0)
        {
            Direction randDire;
            do
            {
                randDire = randDir();
            } while (!getWorld()->AllowedDirection(this, randDire));
            setDirection(randDire);
           forwardTicks = randomNumberGenerator(8, 60);
        }
        else if (goingPerp != none && ticksWhenPerp == 200)
        {
            setDirection(goingPerp);
            forwardTicks= randomNumberGenerator(8, 60);
           ticksWhenPerp = 0;
        }
        if (!takeAStep(getDirection()))
        {
            forwardTicks = 0;

        }
   

if (m_leave)
{
    int x = getX();
    int y = getY();
    if (x == 60 && y == 60)
    {
        isDead();
        getWorld()->killEnemy();
        
        
    }
    else
    {
   
        Direction move = getWorld()->MoveToExit(x, y);
        setDirection(move);
        takeAStep(move);

    }
    return;
}

    
}
void Protester::incGold() //when protester colects gold he leaves
{
    (*getWorld()).playSound(SOUND_PROTESTER_FOUND_GOLD);
    (*getWorld()).increaseScore(25);
    m_leave = true;
    tickAmount(0);
}


bool Protester::isAnnoyed
 (unsigned int amount) //points increae when the protester gets annoyed
{
    if (m_leave)
    {
        return false;
    }
    Alive::isAnnoyed(amount);
    if (getHealth() > 0)
    {
        (*getWorld()).playSound(SOUND_PROTESTER_ANNOYED);
        tickAmount(ticksWhenStunning);
    }
    else if (amount == 2)
    {
        (*getWorld()).increaseScore(scoreInc);

    }
    else if (amount == 100) //boulder
    {

        (*getWorld()).increaseScore(500);
    }
    return true;
}
void Protester::tickAmount(int ticks) //initialize ticks
{
    ticksWhenStable = ticks;
}


bool Protester::hunts() const //the protester can indeed hunt
{
    return true;
}

void Protester::endGame() //whenever the protester leaves he dissapears
{
    ticksWhenStable = 0;
    m_leave = true;
    (*getWorld()).playSound(SOUND_PROTESTER_GIVE_UP);
}
GraphObject::Direction Protester::perpDir(Direction dir) //appropriately lets the protester move in perpindicualr random directions in spaces that are allowed, aka no earth
{
    if(dir == right || dir == left)
       {
           if(getWorld()->AllowedDirection(this, up))
           {
               return up;
           }
           else if(getWorld()->AllowedDirection(this, down))
           {
               return down;
           }
       }
       if(dir == up || dir == down)
       {
           if(getWorld()->AllowedDirection(this,right))
           {
               return right;
           }
           else if(getWorld()->AllowedDirection(this, left))
           {
               return left;
           }
       }
       return none;
   }

bool Protester::tech() //only hardcore use technology
{
    return false;
}


GraphObject::Direction Protester::randDir() const //same reasonning for perp, but they move horizontally
{
    int random = randomNumberGenerator(1, 4);


    if(random== 1)
    {
        return right;
    }
    if(random == 2)
    {
        return left;
    }
    if(random == 3)
    {
        return up;
    }
    if(random == 4)
    {
        return down;
    }

    return none;
}



HardCoreProtester::HardCoreProtester(StudentWorld* world)
    :Protester(world, TID_HARD_CORE_PROTESTER, 20, 250) //set the points earned to 250 and changes image ID

{}
void HardCoreProtester::incGold()
{
    (*getWorld()).playSound(SOUND_PROTESTER_FOUND_GOLD);
    (*getWorld()).increaseScore(50);
    int ticks_to_stare = max(50, 100 - (int)getWorld()->getLevel() * 10);
    tickAmount(ticks_to_stare);
}

HardCoreProtester::~HardCoreProtester() //no need

{}

bool HardCoreProtester::tech()
{ //uses technology to find tunnelman
    int M = 16 + (int)(*getWorld()).getLevel() * 2;
    Direction dir2 = (*getWorld()).MoveToLocation(getX(), getY(), M);
    if (dir2 != none)
    {
        setDirection(dir2);
        takeAStep(dir2);
        return true;
    }
    return false;
}



Boulder::Boulder(StudentWorld* world, int x, int y) //intializes TID_boulder for image ID
    :Actor(world, TID_BOULDER, true, x, y, down, 1.0, 1)
{
    position = STABLE;
    (*getWorld()).deletePart(x, y); boulderWaitingTick = 0; //deletes the esrth part so the boulder can be visible
}

Boulder::~Boulder(){} //no need
void Boulder::doSomething()
{ //if the boulder is standing still, it doesnt do anything besides block, but if dirt is taken underneath, it will enter falling state


    if (!isNotDead())
    {
        return;
    }

    if(position == STABLE)
    {

        if ((*getWorld()).correctMove(this, getX(), getY() - 1))
        {
            position = WAITING;
        }
    }
    if(position == WAITING)
    {
        if (boulderWaitingTick == 30)
        {
            position = FALLING;
            (*getWorld()).playSound(SOUND_FALLING_ROCK);
        }
        else
        {
            boulderWaitingTick++;
        }
    }
    if(position == FALLING)
    {
        if (!moveToXandY(getX(), getY() - 1))
        {
            isDead();


        }
        getWorld()->beAnnoying(this, 100, 3.0);
    }
}


bool Boulder::canBlock() const
{
    return true; //boulders block protest and tunnelman
}

Squirt::Squirt(StudentWorld* world, int x, int y, Direction dir)
    :Actor(world, TID_WATER_SPURT, true, x, y, dir, 1.0, 1) //initializes TID_WATER_SPURT
{distanceSquirt = 4;} //squirt can only go 4 distance

void Squirt::doSomething() //if tunnelman isnt facing the earth it can shoot and decrement water count
{
    if (!isNotDead())
    {
        return;
    }
    if ((*getWorld()).beAnnoying(this, 2, 3.0) > 0 || !getWorld()->AllowedDirection(this, getDirection())|| distanceSquirt <= 0)
    {
        isDead();
    }
    else
    {
        takeAStep(getDirection());
        distanceSquirt--;
    }
}
Squirt::~Squirt()
{} //no need

Goodie::Goodie(StudentWorld* world, int imageID, bool visible, int x, int y)
    :Actor(world, imageID, visible,x,y, right, 1.0, 2) //base class
{}

Goodie::~Goodie()
{}



void Goodie::setTick(unsigned int ticks)
{
    numTicks = ticks;
}

void Goodie::removeTicks()
{
    numTicks--;
    if (numTicks == 0)
    {
        isDead();
    }
}

void Goodie::sound() const //when goodie aquired play sound for all of them
{
    (*getWorld()).playSound(SOUND_GOT_GOODIE);
}

void Goodie::doSomething()
{
    if (!isNotDead())
    {
        return;
    }
    if ((*getWorld()).nearTo(this, 3.0)) //if you aquire goodie score increases, it dissapears, and sound plays
    {
        incPoints();
        isDead();
        sound();

    }
}
Barrel::Barrel(StudentWorld* world, int x, int y)
    :Goodie(world, TID_BARREL, false, x, y) //barrel isnt bisible
{}

Barrel::~Barrel()
{}

void Barrel::sound() const
{
    (*getWorld()).deleteBarrels();
    (*getWorld()).playSound(SOUND_FOUND_OIL); //when you collect delete the barral and play sound
}

void Barrel::incPoints()
{
    (*getWorld()).increaseScore(1000); //increase the points when you get it
}


Gold::Gold(StudentWorld* world, int x, int y, bool temp)
    :Goodie(world, TID_GOLD, false, x, y)
{
    state = temp;
    if (temp)
    {
        setTick(100); //gold has a tick of 100
    }
}

Gold::~Gold()
{}

void Gold::doSomething()
{
    if (!isNotDead())
    {
        return; //if you walk near it will appear and if you drop it to bribe it will dissapear after certain ticks
    }
    if (state)
    {
        removeTicks();
        if ((*getWorld()).bribe(this, 3.0))
            isDead();
    }
    else
    {
        Goodie::doSomething(); //does what baseclass says
    }
}

void Gold::incPoints()
{
    getWorld()->increaseScore(10); //if you collect it score inc by 10 and amount of gold incrememnts
    getWorld()->giveGold();
}



SonarKit::SonarKit(StudentWorld* world)
    :Goodie(world, TID_SONAR, true, 0, 60) //only spawns on top left (0,60)
{
    setTick(max(100, 300 - 10 * (int)getWorld()->getLevel()));
}

SonarKit::~SonarKit()
{
}

void SonarKit::incPoints()
{
    getWorld()->increaseScore(75); //if you collect score increase by 75 and sonar inc
    getWorld()->giveSonar();
}

void SonarKit::doSomething()
{
    Goodie::doSomething(); //does what baseclass does
    removeTicks();
}


WaterPool::WaterPool(StudentWorld* world, int x, int y)
    :
    Goodie(world, TID_WATER_POOL, true, x, y)
{
    setTick(max(100, 300 - 10 * (int)getWorld()->getLevel()));
}

WaterPool::~WaterPool()
{
}

void WaterPool::incPoints()
{
    getWorld()->increaseScore(100); //increase by 100 and increments water
    getWorld()->giveWater();
}

void WaterPool::doSomething()
{
    Goodie::doSomething(); //does what baseclass says to do
    removeTicks();
}



