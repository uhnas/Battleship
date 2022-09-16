#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>

using namespace std;

class BoardImpl
{
  public:
    BoardImpl(const Game& g);
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;

  private:
      // TODO:  Decide what private members you need.  Here's one that's likely
      //        to be useful:
    int mRows;
    int mCols;
    char m_display[MAXROWS][MAXCOLS];
    struct Ship
    {
        int shipid;
        string shipname;
        int shiplen;
        char shipsymbol;
    };
    vector <Ship> m_boardShips {};
    vector <Ship> m_deadShips {};
    const Game& m_game;
};

BoardImpl::BoardImpl(const Game& g)
: m_game(g), m_boardShips({}), m_deadShips({})
{
    mRows = g.rows();
    mCols = g.cols();

    for(int i = 0; i < mRows; i++)
    {
        for(int j = 0; j < mCols; j++)
        {
            m_display[i][j] = '.';
        }
    }
    // This compiles, but may not be correct
}



void BoardImpl::clear()
{
    for(int i = 0; i < mRows; i++)
        for(int j = 0; j < mCols; j++)
        {
            m_display[i][j] = '.';
        }
    
    // This compiles, but may not be correct
}

void BoardImpl::block()
{
    int counter = 0;
    while(counter < ((mRows * mCols) / 2))
    {
        if(m_display[randInt(mRows)][randInt(mCols)] == '#') // if not blocked go to next iteration of loop
        {
            continue;
        }
        else
        {
            m_display[randInt(mRows)][randInt(mCols)] = '#'; // block using #
            counter++;
        }
    }
    // TODO:  Replace this with code to block half of the cells on the board
}

void BoardImpl::unblock()
{
    for(int i = 0; i < mRows; i++)
        for(int j = 0; j < mCols; j++)
        {
            if(m_display[i][j] == '#') 
            {
                m_display[i][j] = '.';
            }
        }
    // TODO:  Replace this with code to unblock all blocked cells
}

//void BoardImpl::block()
//{
//      // Block cells with 50% probability
//    for (int r = 0; r < m_game.rows(); r++)
//        for (int c = 0; c < m_game.cols(); c++)
//            if (randInt(2) == 0)
//            {
//                ; // TODO:  Replace this with code to block cell (r,c)
//            }
//}
//
//void BoardImpl::unblock()
//{
//    for (int r = 0; r < m_game.rows(); r++)
//        for (int c = 0; c < m_game.cols(); c++)
//        {
//            ; // TODO:  Replace this with code to unblock cell (r,c) if blocked
//        }
//}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{

    if(shipId < 0 || shipId > (m_game.nShips() - 1))
    {
        return false;
    }
    
    if(topOrLeft.r < 0 || topOrLeft.r > m_game.rows()-1 || topOrLeft.c < 0 || topOrLeft.c > m_game.cols()-1)
    {
        return false;
    }
    
    for(int i = 0; i < mRows; i++)
    {
        for(int j = 0; j < mCols; j++)
        {
            if(m_display[i][j] == m_game.shipSymbol(shipId))
            {
                return false;
            }
        }
    }
    
    if(dir == VERTICAL)
    {
        Point top = topOrLeft;
        if(top.r < 0 || top.c < 0)
        {
            return false;
        }
        if(top.r + m_game.shipLength(shipId) > m_game.rows())
        {
            return false;
        }
        for(int i = 0; i < m_game.shipLength(shipId); i++)
        {
            if(m_display[top.r+i][top.c] != '.')
            {
                return false;
            }
        }
        for(int i = 0; i < m_game.shipLength(shipId); i++)
        {
            m_display[top.r+i][top.c] = m_game.shipSymbol(shipId);
        }
        Ship nship;
        nship.shiplen = m_game.shipLength(shipId);
        nship.shipsymbol = m_game.shipSymbol(shipId);
        nship.shipname = m_game.shipName(shipId);
        nship.shipid = shipId;
        m_boardShips.push_back(nship);
        return true;
    }
    else if( dir == HORIZONTAL)
    {
        Point left = topOrLeft;
        if(left.r < 0 || left.c < 0)
        {
            return false;
        }
        if(left.c + m_game.shipLength(shipId) > m_game.cols())
        {
            return false;
        }
        for(int i = 0; i < m_game.shipLength(shipId); i++)
        {
            if(m_display[left.r][left.c+i] != '.')
            {
                return false;
            }
        }
        for(int i = 0; i < m_game.shipLength(shipId); i++)
        {
            m_display[left.r][left.c+i] = m_game.shipSymbol(shipId);
        }
        Ship nship;
        nship.shiplen = m_game.shipLength(shipId);
        nship.shipsymbol = m_game.shipSymbol(shipId);
        nship.shipname = m_game.shipName(shipId);
        nship.shipid = shipId;
        m_boardShips.push_back(nship);
        return true;
    }
    else
    {
        return false;
    }
    
    return false;
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    if(shipId < 0 || shipId > (m_game.nShips() - 1))
    {
        return false;
    }
    
    if(topOrLeft.r < 0 || topOrLeft.r > m_game.rows()-1 || topOrLeft.c < 0 || topOrLeft.c > m_game.cols()-1)
    {
        return false;
    }
    
    for(int i = 0; i < mRows; i++)
    {
        for(int j = 0; j < mCols; j++)
        {
            if(m_display[i][j] == m_game.shipSymbol(shipId))
            {
                break;
            }
            
            if(i == mRows - 1 && j == mCols - 1)
            {
                if(m_display[i][j] == m_game.shipSymbol(shipId))
                {
                    break;
                }
                else
                {
                    return false;
                }
            }
        }
    }
    
    if(dir == VERTICAL)
    {
        Point top = topOrLeft;
        if(top.r < 0 || top.c < 0)
        {
            return false;
        }
        
        if(m_display[top.r][top.c] != m_game.shipSymbol(shipId))
        {
            return false;
        }
        
        
        for(int i = 0; i < m_game.shipLength(shipId); i++)
        {
            if(m_display[top.r+i][top.c] == '.')
            {
                return false;
            }
        }
        
        for(int i = 0; i < m_game.shipLength(shipId); i++)
        {
            m_display[top.r+i][top.c] = '.';
        }
        
    }
    
    if(dir == HORIZONTAL)
    {
        Point top = topOrLeft;
        if(top.r < 0 || top.c < 0)
        {
            return false;
        }
        
        if(m_display[top.r][top.c] != m_game.shipSymbol(shipId))
        {
            return false;
        }
        
        
        for(int i = 0; i < m_game.shipLength(shipId); i++)
        {
            if(m_display[top.r][top.c+i] == '.')
            {
                return false;
            }
        }
        
        for(int i = 0; i < m_game.shipLength(shipId); i++)
        {
            m_display[top.r][top.c+i] = '.';
        }
        
    }
    m_boardShips.erase(m_boardShips.begin() + shipId);
    
    return true; // This compiles, but may not be correct
}

void BoardImpl::display(bool shotsOnly) const
{
    cout << "  ";
    for(int cols = 0; cols < m_game.cols(); cols++)
    {
        cout << cols;
    }
    cout << endl;
    
    for(int rows = 0; rows < m_game.rows(); rows++)
    {
        cout << rows << " ";
        for(int cols = 0; cols < mCols; cols++)
        {
            if(m_display[rows][cols] == 'X' || m_display[rows][cols] == 'o' || m_display[rows][cols] == '.')
            {
                cout << m_display[rows][cols];
            }
            else
            {
                if(shotsOnly == false)
                {
                    cout << m_display[rows][cols];
                }
                else
                {
                    cout << '.';

                }
            }
        }
        cout << endl;
    }
    // This compiles, but may not be correct
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    
    if(p.r < 0 || p.r > m_game.rows()-1 || p.c < 0 || p.c > m_game.cols()-1)
    {
        shipId = -1;
        shotHit = false;
        shipDestroyed = false;
        return false;
    }
    
    if(m_display[p.r][p.c] == 'o' || m_display[p.r][p.c] == 'X')
    {
        shipId = -1;
        shotHit = false;
        shipDestroyed = false;
        return false;
    }
    
    if(m_display[p.r][p.c] == '.')
    {
        m_display[p.r][p.c] = 'o';

        shipId = -1;
        shotHit = false;
        shipDestroyed = false;
    }
    
    else
    {
        shotHit = true;
        char sym = m_display[p.r][p.c];
        m_display[p.r][p.c] = 'X';
        for(int i = 0; i < m_boardShips.size(); i++)
        {
            if(sym == m_boardShips[i].shipsymbol)
            {
                shipId = m_boardShips[i].shipid;
            }
        }
        
        for(int r = 0; r < mRows; r++)
        {
            for(int c = 0; c < mCols; c++)
            {
                if(m_display[r][c] == sym)
                {
                    shipDestroyed = false;
                    return true;
                }
            }
        }
        shipDestroyed = true;
        if(shipId > -1)
        {
            Ship nship;
            nship.shiplen = m_game.shipLength(shipId);
            nship.shipsymbol = m_game.shipSymbol(shipId);
            nship.shipname = m_game.shipName(shipId);
            nship.shipid = shipId;
            m_deadShips.push_back(nship);
        }
        
        return true;
    }
    
    return true; // This compiles, but may not be correct
}

bool BoardImpl::allShipsDestroyed() const
{
    if(m_boardShips.size() == m_deadShips.size())
    {
        
        return true;
    }
    else
    {
        return false;
    }
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
