
#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <algorithm>
#include <random>
#include <queue>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}

//implemented a random number generator for 2  numbers in order to not have to continue to put srand().
int randomNumberGenerator(int a, int b)
{
    if (b < a)
    {
        swap(b, a);
    }
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> rand(a, b);
    return rand(gen);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
 //initializes the player to nullptr, as well as eeatth
StudentWorld::StudentWorld(std::string assetDir)
    : GameWorld(assetDir)
{
    m_player = nullptr;
        int i = 0;
        while(i < VIEW_HEIGHT - 4)
        {
            for(int j  = 0; j < VIEW_WIDTH; j++)
            {
                m_earth[i][j] = nullptr;

            }
            i++;
        }
    }

StudentWorld::~StudentWorld() //deletes the player, earth and all of the actors in the actors vector
{
    delete m_player;
     for(int i = 0; i < VIEW_HEIGHT -4; i++)
     {
         for(int j =0; j < VIEW_WIDTH; j++)
         {
             delete m_earth[i][j];
         }
     }
     vector<Actor*> :: iterator it;
     it = actors.begin();
     while(it != actors.end())
     {
         delete *it;
         it = actors.erase(it);
     }
}

int StudentWorld::init()
{


        awaitingTicks= max(25, 200 - (int)getLevel());
        m_ticks = awaitingTicks;
        protestCountTick = 0;
        chanceOfProtest = min(15, 2 + (int)((int)getLevel() * 1.5));
       chanceOfHard = min(90, (int)getLevel() * 10 + 30);

        m_player = new TunnelMan(this);
        for (int i = 0; i < VIEW_HEIGHT - 4; i++)
        {
            for (int j = 0; j < VIEW_WIDTH; j++)
            {
                if (j >= 30 && j <= 33 && i >= 4 && i <= 59)
                {
                    m_earth[i][j] = nullptr;
                }
                else
                {
                    m_earth[i][j] = new Earth(this, j, i); //allocates earth
                }
            }
        }

        int B = min((int)getLevel() / 2+2, 9);
    int i = 0;
    while(i < B)
    {
        int x, y;
       valid(x, y);
        incActor(new Boulder(this, x, y)); //adds boulders to the earth in proper positions
        i++;
    }


        int G = max(5 - (int)getLevel() / 2, 2);

    while(i < G)
    {
        int x, y;
        valid(x, y);
        incActor(new Gold(this, x, y, false)); //add golds to earth in proper positions
        i++;
    }


       numBarrels = min(2 + (int)getLevel(), 21);
    i = 0;
    while(i < numBarrels)
    {
        int x, y;
        valid(x, y);
        incActor(new Barrel(this, x, y)); //adds barrels to earth in proper positions
        i++;
    }

        return GWSTATUS_CONTINUE_GAME;
    }



//lets the game actually compile and run
int StudentWorld::move()
{


    setDisplayText();
    if (m_ticks == awaitingTicks && protestCountTick < chanceOfProtest)
        {
            int randomNum = randomNumberGenerator(1, 100);
            if (randomNum > chanceOfHard)
            {
                incActor(new Protester(this)); //adds new protesters
            }
            else
            {
                incActor(new HardCoreProtester(this)); //adds new hardcore protesters
            }
           protestCountTick++;
            m_ticks = 0;
        }
        else if (m_ticks < awaitingTicks)
            m_ticks++;

        incGoodie();
        vector<Actor*>::iterator it;
        it = actors.begin();
        while(it != actors.end())
        {
            if((*it)->isNotDead())
            {
                (*it)->doSomething();
                if(!(*m_player).isNotDead())
                {
                    decLives();
                    return GWSTATUS_PLAYER_DIED;
                }
                if(completed())
                {
                    return GWSTATUS_FINISHED_LEVEL; //allows for the level to chang eif the player isnt dead
                }
            }
            it++;
        }
        (*m_player).doSomething();
        if(!(*m_player).isNotDead())
        {
            decLives();
            return GWSTATUS_PLAYER_DIED; //if player died then  lose lives
        }
        if(completed())
        {
            return GWSTATUS_FINISHED_LEVEL;
        }
        remove();
        return GWSTATUS_CONTINUE_GAME;
    }


void StudentWorld::cleanUp()
{
    //same as a destructor
    delete m_player;
       for(int i = 0; i < VIEW_HEIGHT -4; i++)
       {
           for(int j =0; j < VIEW_WIDTH; j++)
           {
               delete m_earth[i][j];
           }
       }
       vector<Actor*> :: iterator it;
       it = actors.begin();
       while(it != actors.end())
       {
           delete *it;
           it = actors.erase(it);
       }
   }


void StudentWorld::showObjects(int a, int b, double i)
{

    vector<Actor*>::iterator it;
       it = actors.begin();
       while(it!=actors.end())
       {
           if(nearEnough(a, b, (*it)->getX(), (*it)->getY(), i)) //if objects are near the tunnleman the make them visible for every objcet
           {
               (*it)->setVisible(true);
           }
           it++;
       }
   }

void StudentWorld::incActor(Actor* i)
{
    actors.push_back(i); //add actors unto the vector stack
}
void StudentWorld::incGoodie()
{
    //adds goodies to the map

    int a = getLevel() * 25 + 300;
    int random =randomNumberGenerator(1, a);
    if(random == 1)
    {
        int anotherRand = randomNumberGenerator(1, 5);
        if(anotherRand != 1)
        {
            int a;
            int b;
            do{
             choose(a, b,0,VIEW_WIDTH - 4,0,VIEW_HEIGHT-4);
            }while(!noEarth(a, b));
            incActor(new WaterPool(this,a,b)); //where there is no earth, add waterpools and sonar kits
        }
        else{
            incActor(new SonarKit(this));
        }
    }

}


void StudentWorld::deleteBarrels()
{
   numBarrels--; //when barrels are collected decrement
}


void StudentWorld::remove()
{
    vector<Actor*>::iterator ptr;
    ptr = actors.begin();
    while(ptr != actors.end()) //when actors die delete them from the vector
    {
        if (!(*ptr)->isNotDead())
    {
        delete *ptr;
        ptr = actors.erase(ptr);
    }
    else
        ptr++;
}

    }




bool StudentWorld::deletePart(int x, int y)
{

    //change to make code more efficient
    bool release = false;
    for (int i = y; i < VIEW_HEIGHT - 4 && i <= y + 3; i++)
    {
        for (int j = x; j < VIEW_WIDTH && j <= x + 3; j++)
        {
            if (m_earth[i][j] != nullptr)
            {
                m_earth[i][j]->setVisible(false); //everytime tunnelman digs then change visibility from true to false.
                m_earth[i][j] = nullptr; //set it to nullptr
                release = true;
            }
        }
    }
    return release;
}

int StudentWorld::beAnnoying(Actor* i, int num, double r)
{

    int annoy = 0;
    if (nearEnough(i->getX(), i->getY(), m_player->getX(), m_player->getY(), r) && m_player->isAnnoyed(num))
    {
        annoy++; //if object is near an object that can be annoyed then annoying them is allowed
    }
    vector<Actor* > :: iterator it;
    it = actors.begin();
    while(it!=actors.end())
    {
        if (nearEnough(i->getX(), i->getY(), (*it)->getX(), (*it)->getY(), r) && (*it)->isAnnoyed(num))
        {
            annoy++;
        }
        it++;

    }
    return annoy;
    }

bool StudentWorld::bribe(Actor* a, double r)
{
    //if tunnelamn is drops gold and its near the protester enough, then the protester will pick it up, increasing tunnleman score

        vector<Actor*>::iterator it;
        it = actors.begin();
        while(it!=actors.end())
        {
            if(nearEnough(a->getX(), a->getY(), (*it)->getX(), (*it)->getY(), r)&& (*it)->hunts())
            {
                Protester* ptr = dynamic_cast<Protester*>(*it);
                (*ptr).incGold();
                return true;
            }
            it++;
        }
        return false;
}

void StudentWorld::killEnemy()
{
  protestCountTick--;
}



void StudentWorld::shoot()
{
    
    //if tunnelman is not facing the earth, he can shoot a squirt and the player
    playSound(SOUND_PLAYER_SQUIRT); //sound will play everytime
        int a;
        int b;
       nearDigger(m_player, 4, a, b);
        if(correctMove(m_player,a,b)&&noEarth(a, b))
        {
            incActor(new Squirt(this,a,b,(*m_player).getDirection()));
        }
}

void StudentWorld::gettingAnnoyed()
{
    m_player->isAnnoyed(2);
}

void StudentWorld::giveGold()
{
    m_player->incGold(); //score will icn
}

void StudentWorld::giveSonar()
{
    m_player->incSonar(); //score will inc
}

void StudentWorld::giveWater()
{
    m_player->incWater(); //score will inc
}


GraphObject::Direction StudentWorld::MoveToExit(int x, int y)
{

    upgrade(exitMaze, 60, 60); //when protester dies, it goes to where it spawned and dissapears
    return first(exitMaze, x, y);
}


bool StudentWorld::correctMove(Actor* i, int x, int y) const
{
    //checks if the spot is a correct spot to dig/move.
    //checks if theres a boulder, if theres earth etc.
    if(!(*i).canDig()&&!noEarth(x, y))
       {
           return false;
       }
       vector<Actor*>:: const_iterator it;
       it = actors.begin();
       while(it!=actors.end())
       {
           if((*it != i && (*it)->canBlock() && nearEnough(x, y, (*it)->getX(), (*it)->getY(), 3.0)) || (x < 0 || x>VIEW_WIDTH - 4||y>VIEW_HEIGHT -4 || y< 0))
           {

               return false;
           }
           it++;

       }
       return true;

}

    GraphObject::Direction StudentWorld::MoveToLocation(int x, int y, int num)
    {

        upgrade(m_mazeForTunnelMan, m_player->getX(), m_player->getY());
        return first(m_mazeForTunnelMan, x, y, num);
    }
bool StudentWorld::AllowedDirection(Actor* i, GraphObject::Direction dir, int x) const
{
    //implements the correct move by utilizing the directional keys
    int a = i->getX();  int y = i->getY();
      if(dir == GraphObject::right)
      {
          a++;
          for(int j = 0; j < x; a++,j++)
          {
              if(!correctMove(i, a, y))
              {
                  return false;
              }

          }

      }
      if(dir == GraphObject::left)
      {
          a--;
          for(int j = 0; j<x; j++,a--)
          {
              if(!correctMove(i, a, y))
              {
                  return false;
              }
          }
      }
      if(dir == GraphObject::down)
      {
          y--;
          for(int j=0; j<x; j++,y--)
          {
              if(!correctMove(i, a, y))
              {
                  return false;
              }
          }

      }
      if(dir == GraphObject::up)
      {
          y++;
          for(int j = 0; j<x; j++,y++)
          {
              if(!correctMove(i, a, y))
              {
                  return false;
              }
          }
      }
      return true;

  }


bool StudentWorld::nearTo(Actor* i, double k) const
{
    int x =nearEnough(m_player->getX(), m_player->getY(), i->getX(), i->getY(), k); //if the player is within radius of protester or vice verssa
    return x;
}

//checks if the object is looking at tunnelman
bool StudentWorld::looking(Actor* i) const
{
    return((*i).getX()< (*m_player).getY() && (*i).getDirection() == GraphObject::right) || ((*i).getX() > (*m_player).getX() && (*i).getDirection() == GraphObject::left) || ((*i).getY() > (*m_player).getY() && (*i).getDirection() == GraphObject::down) ||((*i).getY() < (*m_player).getY() &&  (*i).getDirection() == GraphObject::up);
}


//checks the line of sight requirement
GraphObject::Direction StudentWorld::inFrontOf(Actor* a) const
{
    int x = (*a).getX(); int y = (*a).getY();
      const int player1 = (*m_player).getX(); const int player2 = (*m_player).getY();

      y = (*a).getY();
      if(y == player2)
      {
          GraphObject::Direction dir = GraphObject::left;
          for(;correctMove(a, x, y);x--)
          {
              if(x == player1)
              {
                  return dir;
              }

          }
          dir = GraphObject::right;
          x = (*a).getX();
          for(;correctMove(a, x, y);x++)
          {
              if(x == player1)
              {
                  return dir;
              }
          }
      }

      if(x == player1)
      {
          GraphObject::Direction dir = GraphObject::up;
          for(;correctMove(a, x, y);y++)
          {
              if(y == player2)
              {
                  return dir;

              }

          }
          dir = GraphObject::down;
          y = (*a).getY();
          for(;correctMove(a, x, y);y--)
          {
              if(y == player2)
              {
                  return dir;

              }

          }
      }
      return GraphObject::none;
  }

//display of the scores, health etc
string StudentWorld::scores(int level, int lives, int health, int water, int gold, int barrels, int sonar, int score) const
{
    string lev = statistics(level, 2);
    string live = statistics(lives, 1);
    string hlth = statistics(health, 2) + "0%";
    string wter = statistics(water, 2);
    string gld = statistics(gold, 2);
    string barl = statistics(barrels, 2);
    string son = statistics(sonar, 2);
    string scr = statistics(score, 6, '0');
    return "Lvl: " + lev + " Lives: " + live + " hlth: " + hlth + " Wtr: " + wter + " Gld: " + gld
    +  " Oil Left: " +  barl   + " Sonar: " +  son  + " Scr: " + scr;
}

//calculates how the statisitics should be formatted
string StudentWorld::statistics(int stat, int numbers, char ch) const
{
    char c = ' ';
    string str = "";

        int div = 1;
        bool start = true;
        int i =1;
        while( i < numbers)
        {
            div *= 10;
            i++;
        }
    for(;div != 0;)
        {
            int digit = stat / div;
            if (start && digit == 0 && div != 1)
                str = str + ch;
            else
            {
                start = false;
                c = '0' + digit;
                str += c;
                if (digit != 0)
                    stat %= (digit * div);
            }
            div = div / 10;
        }
        return str;
    }

//gets all of the objects numbers
void StudentWorld::setDisplayText()
{


    int water = m_player->getWater();
    int health = m_player->getHealth();
    int gold = m_player->getGold();
    int lives = getLives();
    int sonar = m_player->getSonar();
    int score = getScore();
    int barrels = numBarrels;
    int level = getLevel();

    string str= scores(level, lives, health, water, gold, barrels, sonar, score);
    setGameStatText(str);
}

//checks to see if objects are within radius
bool StudentWorld::nearEnough(int xa, int ya, int xb, int yb, double k) const
{
    int x = length(xa, ya, xb, yb);
    return x <= k;
}

//returns false if there is earht, reeturns true if there is no earth
bool StudentWorld::noEarth(int x, int y) const
{
    for (int i = y; i < VIEW_HEIGHT - 4 && i <= y + 3; i++)
    {
        for (int j = x; j < VIEW_WIDTH && j <= x + 3; j++)
        {
            if (m_earth[i][j] != nullptr)
            {
                return false;
            }
        }
    }
    return true;
}

//if all barrels are collected, advance level
    bool StudentWorld::completed() const
    {
        return numBarrels == 0;
    }

//checks to see if object is on the earht
bool StudentWorld::positionOnEarth(int x, int y) const
{
    for (int i = y; i < VIEW_HEIGHT - 4 && i <= y + 3; i++)
    {

        for (int j = x; j < VIEW_WIDTH && j <= x + 3; j++)
        {
            if (m_earth[i][j] == nullptr)
            {
                return false;
            }
        }
    }
    return true;
}
//utilized stacks and queues to update the maze 
    void StudentWorld::upgrade(int maze[][VIEW_WIDTH - 3], int x, int y)
    {

        for (int i = 0; i < VIEW_HEIGHT - 3; i++)
            {
                for (int x = 0; x < VIEW_WIDTH - 3; x++)
                {
                    if (noEarth(x, i) && correctMove(m_player, x, i))
                    {
                        maze[i][x] = -1;
                    }
                    else{
                        maze[i][x] = -2;
                    }
                }
            }
            queue<area> pos;
                area spot(y, x);
            int counter = 1;
            int step = 1;
            maze[y][x] = 0;
           pos.push(spot);
            while (!pos.empty())
            {
                
                if (counter == 0)
                {
                    step++;
                    counter = pos.size();
                }
                area location = pos.front();
                int row = location.row();
                int col = location.col();   
                pos.pop();
                counter--;
                if (row + 1 < VIEW_HEIGHT - 3 && maze[row + 1][col] == -1)
                {
                    area pos1(row + 1, col);
                    pos.push(pos1);
                    maze[row + 1][col] = step;
                }
                if (col - 1 >= 0 && maze[row][col - 1] == -1)
                {
                  area pos1(row, col - 1);
                    pos.push(pos1);
                    maze[row][col - 1] = step;
                }
                if (row - 1 >= 0 && maze[row - 1][col] == -1)
                {
                 area pos1(row - 1, col);
                    pos.push(pos1);
                    maze[row - 1][col] = step;
                }
                if (col + 1 < VIEW_WIDTH - 3 && maze[row][col + 1] == -1)
                {
                  area pos1(row, col + 1);
                    pos.push(pos1);
                    maze[row][col + 1] = step;
                }

            
            }
        }
void StudentWorld::choose(int& a, int& b, int xa, int xb, int  ya, int yb)
{

    a = randomNumberGenerator(xa, xb); b= randomNumberGenerator(ya, yb);
}

//checks if the location of objects are in appropraite locations
void StudentWorld::valid(int&x, int &y)
{
    bool valid;
    do
    {
        valid = true;
        choose(x, y);
        if (!positionOnEarth(x, y))
        {
            valid = false;
            continue;
        }

        vector<Actor*>::iterator ptr;
        ptr = actors.begin();
        while(ptr != actors.end()) //checks the actors in thge vector stack
        {
            if(length(x, y, (*ptr)->getX(), (*ptr)->getY())<=6)
            {
                valid = false;
                break;
            }
            ptr++;
        }

    } while (!valid);
}

//checks to see if protesters are near tunnnelman
void StudentWorld::nearDigger(Actor* i, int j, int& a, int& b)
{
    if((*i).getDirection() == GraphObject::right)
       {
           b = (*i).getY();
           a = (*i).getX() + j;
       }
       if((*i).getDirection() == GraphObject::up)
       {
           a =(*i).getX();
           b = (*i).getY() + j;
       }
       if((*i).getDirection() == GraphObject::down)
       {
           b = (*i).getY() - j;
           a = (*i).getX();
       }
       if((*i).getDirection() == GraphObject::left)
       {
           a = (*i).getX() - j;
           b = (*i).getY();
       }

   }

//checks the distance between objects. uses algebraic formula
double StudentWorld::length(int xa, int ya, int xb, int yb) const
{
        double ansToX = xa - xb; double ansToY = ya-yb;
        double total = sqrt(ansToX*ansToX + ansToY*ansToY);
        return total;
}


GraphObject::Direction StudentWorld::first(int maze[][VIEW_WIDTH - 3], int x, int y, int max)
{



    int current = maze[y][x];
    if(current > max)
    {
        return GraphObject::none;
    }
    if(y <VIEW_HEIGHT - 4)
    {
        int up  = maze [y + 1][x];
        if(up < current && up > -1)
        {
            return GraphObject::up;
        }
    }
    if(y > 0)
    {
        int down = maze [y-1][x];
        if(down < current && down > -1)
        {
            return GraphObject::down;
        }
    }
    if(x < VIEW_WIDTH - 4)
    {
        int right = maze[y][x+1];
        if(right < current&& right> -1)
        {
            return GraphObject::right;
        }
    }
    if(x > 0)
    {
        int left = maze[y][x-1];
        if(left < current && left > -1)
        {
            return GraphObject::left;
        }
    }

    return GraphObject::none;
}




