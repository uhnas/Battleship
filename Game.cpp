#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <cctype>

using namespace std;

class GameImpl
{
  public:
    GameImpl(int nRows, int nCols);
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char symbol, string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
private:
    int mRows;
    int mCols;
    struct Ship
    {
        int shipid;
        string shipname;
        int shiplen;
        char shipsymbol;
    };
    vector <Ship> m_ships;
};

void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols)
{
    // This compiles but may not be correct
    mRows = nRows;
    mCols = nCols;
}

int GameImpl::rows() const
{
    return mRows;  // This compiles but may not be correct
}

int GameImpl::cols() const
{
    return mCols;  // This compiles but may not be correct
}

bool GameImpl::isValid(Point p) const
{
    return p.r >= 0  &&  p.r < rows()  &&  p.c >= 0  &&  p.c < cols();
}

Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name)
{
    if( length <= 0 || (length > mRows && length > mCols))
    {
        return false;
    }
    if(!isprint(symbol))
    {
        return false;
    }
    if(symbol == 'X' || symbol == '.' || symbol == 'o')
    {
        return false;
    }
    else
    {
        for(int i = 0; i < m_ships.size(); i++)
        {
            if( m_ships[i].shipname == name)
            {
                return false;
            }
        }
    Ship nship;
    nship.shipid = m_ships.size();
    nship.shiplen = length;
    nship.shipsymbol = symbol;
    nship.shipname = name;
    m_ships.push_back(nship);
    return true;  // This compiles but may not be correct
    }
    return false;
}

int GameImpl::nShips() const
{
    int total = m_ships.size();
    
    return total;  // This compiles but may not be correct
}

int GameImpl::shipLength(int shipId) const
{
    int len = m_ships[shipId].shiplen;
    return len;  // This compiles but may not be correct
}

char GameImpl::shipSymbol(int shipId) const
{
    char sym = m_ships[shipId].shipsymbol;
    return sym;  // This compiles but may not be correct
}

string GameImpl::shipName(int shipId) const
{
    string nam = m_ships[shipId].shipname;
    return nam;  // This compiles but may not be correct
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)
{
    if(!p1->placeShips(b1) || !p2->placeShips(b2))
    {
        return nullptr;
    }
    
    int turn = 1;
    bool checkshot = false;
    bool shotHit = false;
    bool shipDestroyed = false;
    int attackId = -1;

    while(!b1.allShipsDestroyed() && !b2.allShipsDestroyed())
    {
        if(turn % 2 == 0)
        {
            cout << p2->name() << "'s turn. Board for " << p1->name() << ":" << endl;
            b1.display(p2->isHuman());
            Point coordattack = p2->recommendAttack();
            checkshot = b1.attack(coordattack, shotHit, shipDestroyed, attackId);
            p2->recordAttackResult(coordattack, checkshot, shotHit, shipDestroyed, attackId);
            if(p2->isHuman() && attackId == -1)
            {
                cout << p2->name() << " wasted a shot at (" << coordattack.r << "," << coordattack.c << ")." << endl;
            }
            else
            {
                cout << p2->name() << " attacked (" << coordattack.r << "," << coordattack.c << ") and ";
                
                if(shotHit == true && shipDestroyed == true)
                {
                    cout << "destroyed the " << this->shipName(attackId);
                
                }
                else if(shotHit == true)
                {
                    cout << "hit something";
                }
                else
                {
                    cout << "missed";
                }
                
                cout << ", resulting in:" << endl;
                b1.display(p2->isHuman());

            }
        }
        else
        {
            cout << p1->name() << "'s turn. Board for " << p2->name() << ":" << endl;
            b2.display(p1->isHuman());
            Point coordattack = p1->recommendAttack();
            checkshot = b2.attack(coordattack, shotHit, shipDestroyed, attackId);
            p1->recordAttackResult(coordattack, checkshot, shotHit, shipDestroyed, attackId);
            if(p1->isHuman() && attackId == -1)
            {
                cout << p1->name() << " wasted a shot at (" << coordattack.r << "," << coordattack.c << ")." << endl;
            }
            else
            {
                cout << p1->name() << " attacked (" << coordattack.r << "," << coordattack.c << ") and ";
                
                if(shotHit == true && shipDestroyed == true)
                {
                    cout << "destroyed the " << this->shipName(attackId);
                
                }
                else if(shotHit == true)
                {
                    cout << "hit something";
                }
                else
                {
                    cout << "missed";
                }
                
                cout << ", resulting in:" << endl;
                b2.display(p1->isHuman());
            }
        }
        turn++;
        if(b1.allShipsDestroyed() || b2.allShipsDestroyed())
        {
            break;
        }
        if (shouldPause == true)
        {
            waitForEnter();
        }
    }
    if(b1.allShipsDestroyed())
    {
        cout << p2->name() << " wins!" << endl;
        return p2;
    }
    if(b2.allShipsDestroyed())
    {
        cout << p1->name() << " wins!" << endl;
        return p1;
    }
    return nullptr;  // This compiles but may not be correct
}



//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1  ||  nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows()  &&  length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
             << endl;
        return false;
    }
    if (!isascii(symbol)  ||  !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
             << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X'  ||  symbol == '.'  ||  symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
             << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                 << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0  && shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr  ||  p2 == nullptr  ||  nShips() == 0)
        return nullptr;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}

