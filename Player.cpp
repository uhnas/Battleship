#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <stack>


using namespace std;

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
  public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
      // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
      // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
      // AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}

class HumanPlayer : public Player
{
  public:
    HumanPlayer(string nm, const Game& g) : Player(nm, g) {}
    virtual ~HumanPlayer() {}
    virtual bool isHuman() const;
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    Point m_lastCellAttacked;
};

bool HumanPlayer :: isHuman() const
{
    return true;
}

bool HumanPlayer :: placeShips(Board& b)
{
    Direction direct;
    string dir;
    int rows;
    int cols;

    int next = 0;
    
    cout<<name()<<" must place "<<game().nShips() <<" ships."<<endl;
    
    for( int i = 0; i < game().nShips(); i++)
    {
        b.display(false);
        while(next == 0)
        {
            cout << "Enter h or v for direction of " << game().shipName(i) << " (length " << game().shipLength(i) << "):";
            cin >> dir;
            if(dir != "h" && dir != "v")
            {
                cout << "Direction must be h or v." << endl;
            }
            else
            {
                next = 1;
            }
        }
        
        if(dir == "h")
        {
            direct = HORIZONTAL;
        }
        else
        {
            direct = VERTICAL;
        }
        
        while(next == 1)
        {
            if(direct == HORIZONTAL)
            {
                cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
                bool check = getLineWithTwoIntegers(rows, cols);
                if(!check)
                {
                    cout << "You must enter two integers." << endl;
                    continue;
                }
                else
                {
                    Point left = Point(rows, cols);
                    if(!b.placeShip(left, i, direct))
                    {
                        
                        cout << "The ship can not be placed there." << endl;
                        continue;
                    }
                    else
                    {
                        next = 2;
                        break;
                    }
                }
                
            }
            else
            {
                cout << "Enter row and column of topmost cell (e.g., 3 5): ";
                bool check = getLineWithTwoIntegers(rows, cols);
                if(!check)
                {
                    cout << "You must enter two integers." << endl;
                    continue;
                }
                else
                {
                    Point top = Point(rows, cols);
                    if(!b.placeShip(top, i, direct))
                    {
                        cout << "The ship can not be placed there." << endl;
                        continue;
                    }
                    else
                    {
                        next = 2;
                        break;
                    }
                }
            }
        }
        next = 0;
    }
    
    return true;
}

Point HumanPlayer :: recommendAttack()
{
    Point attacked;
    int rows;
    int cols;
    int next = 0;
    while(next == 0)
    {

        cout << "Enter the row and column to attack (e.g., 3 5): ";
        bool check = getLineWithTwoIntegers(rows, cols);
        if(!check)
        {
            cout << "You must enter two integers." << endl;
            continue;
        }
        else
        {
            next = 1;
            break;
        }
    }
    attacked = Point(rows, cols);
    return attacked;
}

void HumanPlayer :: recordAttackResult(Point p, bool validShot, bool shotHit,
                                            bool shipDestroyed, int shipId)
{}

void HumanPlayer :: recordAttackByOpponent(Point p)

{}



//*********************************************************************
//  MediocrePlayer
//*********************************************************************



class MediocrePlayer : public Player
{
  public:
    MediocrePlayer(string nm, const Game& g);
    virtual ~MediocrePlayer() {}
    virtual bool isHuman() const {return false;}
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    bool placeShipsHelper(int shipId, Board& b);
    int findPointPos(const vector<Point>& points, const Point& p);
    Point m_lastCellAttacked;
    Point cross;
    vector<Point> hitpoints; // all points that we can hit
    




    int m_state;
    int mRows;
    int mCols;
};

MediocrePlayer::MediocrePlayer(string nm, const Game& g)
: Player(nm, g), m_state(1)
{
    mRows = game().rows();
    mCols = game().cols();
    for(int i = 0; i < mRows; i++)
    {
        for(int j = 0; j < mCols; j++)
        {
            hitpoints.push_back(Point(i,j));
        }
    }
}

bool MediocrePlayer :: placeShips(Board& b)
{
    int fifty = 0;
    bool canplace = false;
    while(fifty < 50 && canplace == false)
    {
        b.block();
        canplace = placeShipsHelper(game().nShips()-1, b);
        b.unblock();
        fifty++;
    }
    
    return canplace;
}

bool MediocrePlayer :: placeShipsHelper(int shipId, Board& b)
{
    bool returnValue = false;

    if(shipId == -1)
    {
        return true;
    }
    for(int i = 0; i < game().rows(); i++)
    {
        for(int j = 0; j < game().cols(); j++)
        {
            if(b.placeShip(Point(i,j), shipId, HORIZONTAL))
            {
                returnValue = placeShipsHelper(shipId-1, b);
                
                if(returnValue == true)
                {
                    return true;
                }
                
                else
                {
                    b.unplaceShip(Point(i,j), shipId, HORIZONTAL);
                }
            }
            // try placing vertically
            if(b.placeShip(Point(i,j), shipId, VERTICAL))
            {
               returnValue = placeShipsHelper(shipId-1, b);
               
               if(returnValue == true)
               {
                   return true;
               }
               else
               {
                   b.unplaceShip(Point(i,j), shipId, VERTICAL);
               }
            }
            // verical placing ends
        }
    }
    return false;
}

Point MediocrePlayer::recommendAttack()
{
    int pos = -1;
    Point attack;
    
    if (hitpoints.empty() == true)
    {
        attack = Point(0,0);
        return attack;

    }
    
    if ( m_state == 1 )
    {
        pos = randInt(hitpoints.size());
        attack = hitpoints.at(pos);
    }
    else
    {
        
        bool cantattack = true;
        int i = 0;
        while( i != 9)
        {
            if ( findPointPos(hitpoints, Point(cross.r - 4 + i, cross.c)) != -1 || findPointPos(hitpoints, Point(cross.r, cross.c - 4 + i)) != -1 )
            {
                cantattack = false;
                break;
            }
            i++;
        }
        
        if ( cantattack == true)
        {
            m_state = 1;
            return recommendAttack();
        }
        
        while(pos == -1)
        {
            int j = randInt(9);
            if ( randInt(2) == 1)
            {
                attack = Point(cross.r, cross.c - 4 + j);
            }
            else
            {
                attack = Point(cross.r - 4 + j, cross.c);

            }
            pos = findPointPos(hitpoints, attack);
        }
    }
    
    hitpoints.erase(hitpoints.begin() + pos);
    return attack;
}


int MediocrePlayer :: findPointPos(const vector<Point>& points, const Point& p)
{
    for(int i = 0 ; i < points.size(); i++)
    {
        if(points[i].r == p.r && points[i].c == p.c)
        {
            return i;
        }
        else
        {
            continue;
        }
    }

    return -1;
}

void MediocrePlayer :: recordAttackResult(Point p, bool validShot, bool shotHit,
                                            bool shipDestroyed, int shipId)
{

    if(validShot == false)
    {
        m_state = 1;
        return;
    }
    
    if(m_state == 1)
    {
        if(shotHit == false)
        {
            m_state = 1;
            return;
        }
        if(shotHit == true && shipDestroyed == true)
        {
            m_state = 1;
            return;
        }
        if(shotHit == true && shipDestroyed == false)
        {
            m_state = 2;
            cross = p;

            return;
        }
    }
    
    if(m_state != 1)
    {
        if(shotHit == false)
        {
            m_state = 2;
            return;
        }
        if(shipDestroyed == true)
        {
            m_state = 1;
            return;
        }
        else if(shotHit == true && shipDestroyed == false)
        {
            m_state = 2;
            return;
        }
    }
}


 void MediocrePlayer :: recordAttackByOpponent(Point p)
{ }

// Remember that Mediocre::placeShips(Board& b) must start by calling
// b.block(), and must call b.unblock() just before returning.

//*********************************************************************
//  GoodPlayer
//*********************************************************************

class GoodPlayer : public Player
{
  public:
    GoodPlayer(string nm, const Game& g);
    virtual ~GoodPlayer() {}
    virtual bool isHuman() const {return false;}
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    bool placeShipsHelper(int shipId, Board& b);
    int findPointPos(const vector<Point>& points, const Point& p);
    Point m_lastCellAttacked;
    Point middle;
    Point ogmid;
    vector<Point> hitpoints; // all points that we can hit
    stack<Point> aroundpoints; // all points around a hit
    int m_state;
    int ogshipid;
    int mRows;
    int mCols;
};

GoodPlayer::GoodPlayer(string nm, const Game& g)
: Player(nm, g), m_state(1)
{
    mRows = game().rows();
    mCols = game().cols();
    for(int i = 0; i < mRows; i++)
    {
        for(int j = 0; j < mCols; j++)
        {
            hitpoints.push_back(Point(i,j));
        }
    }
}

bool GoodPlayer :: placeShips(Board& b)
{
    bool canplace = false;
    b.block();
    canplace = placeShipsHelper(game().nShips()-1, b);
    b.unblock();
    
    return canplace;
}

bool GoodPlayer :: placeShipsHelper(int shipId, Board& b)
{
    bool returnValue = false;

    if(shipId == -1)
    {
        return true;
    }
    for(int i = 0; i < game().rows(); i++)
    {
        for(int j = 0; j < game().cols(); j++)
        {
            if(b.placeShip(Point(i,j), shipId, HORIZONTAL))
            {
                returnValue = placeShipsHelper(shipId-1, b);
                
                if(returnValue == true)
                {
                    return true;
                }
                
                else
                {
                    b.unplaceShip(Point(i,j), shipId, HORIZONTAL);
                }
            }
            // try placing vertically
            if(b.placeShip(Point(i,j), shipId, VERTICAL))
            {
               returnValue = placeShipsHelper(shipId-1, b);
               
               if(returnValue == true)
               {
                   return true;
               }
               else
               {
                   b.unplaceShip(Point(i,j), shipId, VERTICAL);
               }
            }
            // vertical placing ends
        }
    }
    return false;
}

int GoodPlayer :: findPointPos(const vector<Point>& points, const Point& p)
{
    for(int i = 0 ; i < points.size(); i++)
    {
        if(points[i].r == p.r && points[i].c == p.c)
        {
            return i;
        }
        else
        {
            continue;
        }
    }

    return -1;
}

Point GoodPlayer::recommendAttack()
{
    int pos = -1;
    Point attack;

    if (hitpoints.empty() == true)
    {
        attack = Point(0,0);
        return attack;
    }

    if ( m_state == 1 )
    {
        pos = randInt(hitpoints.size());
        attack = hitpoints.at(pos);
    }
    else
    {

        bool cantattack = true;
        while(aroundpoints.size() != 0)
        {

            if (findPointPos(hitpoints, aroundpoints.top()) != -1)
            {
                cantattack = false;
                break;
            }
            aroundpoints.pop();
        }

        if ( cantattack == true)
        {

            m_state = 1;
            return recommendAttack();
        }

        while(pos == -1)
        {
            attack = aroundpoints.top();
            pos = findPointPos(hitpoints, attack);
        }
    }

    hitpoints.erase(hitpoints.begin() + pos);
    return attack;
}




void GoodPlayer :: recordAttackResult(Point p, bool validShot, bool shotHit,
                                            bool shipDestroyed, int shipId)
{
    if(validShot == false)
    {
        m_state = 1;
        return;
    }

    if(m_state == 1)
    {
        if(shotHit == false)
        {
            m_state = 1;

            return;
        }
        if(shipDestroyed == true)
        {
            m_state = 1;
            return;
        }
        if(shotHit == true && shipDestroyed == false)
        {
            m_state = 2;
            middle = p;
            ogshipid = shipId;

            if(findPointPos(hitpoints, Point(middle.r + 1, middle.c)) != -1)
            {

                aroundpoints.push(Point(middle.r+1, middle.c));

            }
            if(findPointPos(hitpoints, Point(middle.r, middle.c + 1)) != -1)
            {
                aroundpoints.push(Point(middle.r, middle.c+1));

            }
            if(findPointPos(hitpoints, Point(middle.r - 1, middle.c)) != -1)
            {
                aroundpoints.push(Point(middle.r-1, middle.c));

            }
            if(findPointPos(hitpoints, Point(middle.r, middle.c - 1)) != -1)
            {
                aroundpoints.push(Point(middle.r, middle.c - 1));
            }

            return;
        }
    }

    if(m_state == 2)
    {
        if(ogshipid != shipId)
        {
            shotHit = false;
        }
        
        if(shotHit == false)
        {
            
            m_state = 2;
            return;
        }
        if(shipDestroyed == true)
        {
            m_state = 1;
            return;
        }
        if(shotHit == true && shipDestroyed == false)
        {
            
            m_state = 3;
            if(p.r == middle.r-1)
            {
                
                if(findPointPos(hitpoints, Point(middle.r - 2, middle.c)) != -1)
                {
                    aroundpoints.push(Point(middle.r-2, middle.c));
                }
                else
                {
                    aroundpoints.push(Point(ogmid.r + 1, middle.c));

                    middle = ogmid;
                    m_state = 2;
                    return;
                }
            }
            else if(p.r == middle.r+1)
            {
                if(findPointPos(hitpoints, Point(middle.r + 2, middle.c)) != -1)
                {

                    aroundpoints.push(Point(middle.r+2, middle.c));
                }
                else
                {
                    aroundpoints.push(Point(ogmid.r - 1, middle.c));
                    middle = ogmid;
                    m_state = 2;

                    return;
                }
            }
            else if(p.c == middle.c + 1)
            {
    
                if(findPointPos(hitpoints, Point(middle.r, middle.c + 2)) != -1)
                {
                    aroundpoints.push(Point(middle.r, middle.c+2));
                }
                else
                {
                    
                    aroundpoints.push(Point(middle.r, ogmid.c -1 ));
                    middle = ogmid;
                    m_state = 2;

                    return;
                }
            }
            else if(p.c == middle.c - 1)
            {
            
                if(findPointPos(hitpoints, Point(middle.r, middle.c - 2)) != -1)
                {
                    aroundpoints.push(Point(middle.r, middle.c - 2));
                }
                else
                {
                   
                    
                    aroundpoints.push(Point(middle.r, ogmid.c + 1));
                    middle = ogmid;
                    m_state = 2;

                    return;
                }
            }
            ogmid = middle;
            middle = p;

            return;
        }
    }
    if(m_state == 3)
    {
        if(ogshipid != shipId)
        {
            shotHit = false;
        }
        if(shotHit == false)
        {
            if(p.r == middle.r-1)
            {
                
                
                if(findPointPos(hitpoints, Point(ogmid.r + 1, middle.c)) != -1)
                {
                    
                    aroundpoints.push(Point(ogmid.r + 1, middle.c));
                }
            }
            else if(p.r == middle.r+1)
            {
                
                
                if(findPointPos(hitpoints, Point(ogmid.r - 1, middle.c)) != -1)
                {
                    
                    aroundpoints.push(Point(ogmid.r - 1, middle.c));
                }
            }
            else if(p.c == middle.c + 1)
            {
               
                
                if(findPointPos(hitpoints, Point(middle.r, ogmid.c - 1)) != -1)
                {
                    aroundpoints.push(Point(middle.r, ogmid.c - 1));
                }
            }
            else if(p.c == middle.c - 1)
            {
                
                if(findPointPos(hitpoints, Point(middle.r, ogmid.c + 1)) != -1)
                {
                    
                    aroundpoints.push(Point(middle.r, ogmid.c + 1));
                }
                
            }
            m_state = 2;
            middle = ogmid;
            
            return;
        }
        if(shipDestroyed == true)
        {
            m_state = 1;
            return;
        }
        if(shotHit == true && shipDestroyed == false)
        {
            m_state = 3;
            if(p.r == middle.r-1)
            {
                
                if(findPointPos(hitpoints, Point(middle.r - 2, middle.c)) != -1)
                {
                    aroundpoints.pop();

                    aroundpoints.push(Point(middle.r-2, middle.c));
                }
                else
                {
                    middle = ogmid;
                    return;
                }
            }
            else if(p.r == middle.r+1)
            {
                if(findPointPos(hitpoints, Point(middle.r + 2, middle.c)) != -1)
                {
                    aroundpoints.pop();

                    aroundpoints.push(Point(middle.r+2, middle.c));

                }
                else
                {
                    middle = ogmid;
                    return;

                }
            }
            else if(p.c == middle.c + 1)
            {
    
                if(findPointPos(hitpoints, Point(middle.r, middle.c + 2)) != -1)
                {
                    aroundpoints.pop();

                    aroundpoints.push(Point(middle.r, middle.c+2));

                }
                else
                {
                    middle = ogmid;
                    return;
                }
            }
            else if(p.c == middle.c - 1)
            {
            
                if(findPointPos(hitpoints, Point(middle.r, middle.c - 2)) != -1)
                {
                    aroundpoints.pop();

                    aroundpoints.push(Point(middle.r, middle.c - 2));
                }
                else
                {
                    middle = ogmid;
                    return;
                }
            }
            middle = p;
        }
        
    }
}

void GoodPlayer::recordAttackByOpponent(Point /* p */)
{
     // GoodPlayer also ignores what the other opponent does because i couldn't figure out a way this could make the goodplayer better
}
//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };

    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  &&
                                                     type != types[pos]; pos++)
        ;
    switch (pos)
    {
      case 0:  return new HumanPlayer(nm, g);
      case 1:  return new AwfulPlayer(nm, g);
      case 2:  return new MediocrePlayer(nm, g);
      case 3:  return new GoodPlayer(nm, g);
      default: return nullptr;
    }
}
