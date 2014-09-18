#include "StudentWorld.h"
#include "actor.h"
#include <sstream>
using namespace std;

GameWorld* createStudentWorld()
{
    return new StudentWorld();
}

StudentWorld::~StudentWorld()
{
    cleanUp();                                                      // Clears everything
}

void StudentWorld::init()
{
    m_ship = new Player(this);
}

int StudentWorld::move()
{    
    addAliensOrStars();                                             // Calls 4 other functions to decide if Aliens/Stars get added
    updateDisplayText();                                            // Update the display text accordingly
    doSomethingForEverything();                                     // Call every actor's doSomething() function
    removeDeadGameObjects();                                        // If any Actor is marked dead, remove it
    
    if (m_ship->isDead() == true)
    {
        decLives();
        m_numOfAliensKilled = getNumAliensToKill();
        return GWSTATUS_PLAYER_DIED;
    }    
    return GWSTATUS_CONTINUE_GAME;
}


void StudentWorld::cleanUp()
{
    delete m_ship;                                                  // Clears everything on the space field
    
    list<Actor*>::iterator del;
    for (del = m_actors.begin(); del != m_actors.end(); )
    {
        Actor* temp = *del;
        delete temp;
        del = m_actors.erase(del);
    }
}

void StudentWorld::addAliensOrStars()
{
    if (testParamsProvided() == true)                               // add Aliens/Goodies
    {
        if (m_actors.size() != 0)
            return;
        int temp = getTestParam(TEST_PARAM_ACTOR_INDEX);
        if (temp == TEST_PARAM_NACHLING)
            addActor(new Nachling(this, getRound()));
        if (temp == TEST_PARAM_WEALTHY_NACHLING)
            addActor(new WealthyNachling(this, getRound()));
        if (temp == TEST_PARAM_SMALLBOT)
            addActor(new Smallbot(this, getRound()));
        if (temp == TEST_PARAM_GOODIE_ENERGY)
            addActor(new EnergyGoodie(this, 15, 39));
        if (temp == TEST_PARAM_GOODIE_TORPEDO)
            addActor(new TorpedoGoodie(this, 15, 39));
        if (temp == TEST_PARAM_GOODIE_FREE_SHIP)
            addActor(new FreeShipGoodie(this, 15, 39));
        return;
    }
    else
    {
        int numberOfActiveAliens = numOfAliens();
        if (numberOfActiveAliens < int(2+.5*getRound()) && numberOfActiveAliens < getNumAliensToKill())                                // number of active aliens has to be less than maximum allowed number of Aliens in the space field and is less than the number of aliens needed to be destroyed by the Player
        {
            int temp = rand() %100;
            if (temp < 70)                                      // 70% chance to create a NachlingBase class
            {
                int temp2 = rand() %100;
                if (temp2 < 20)                                 // 80% chance to make a Nachling, 20% chance to make a Wealthy Nachling
                    addActor(new WealthyNachling(this, getRound()));
                else
                    addActor(new Nachling(this, getRound()));
            }
            else
                addActor(new Smallbot(this, getRound()));
        }
        int temp = rand()% 100;
        if (temp < 33)
            addActor(new Star(this));
    }
}

void StudentWorld::updateDisplayText() // Update the score/lives/round at screen top
{
    int score = getScore();
    int round = getRound();
    double energyPercent = m_ship->getEnergyPct();
    int torpedoes = m_ship->getNumTorpedoes();
    int shipsLeft = getLives();
    
    string S = properDisplayText(score, round, energyPercent, torpedoes, shipsLeft);
    
    setGameStatText(S);
}

std::string StudentWorld::properDisplayText(int mScore, int mRound, double mEnergyPercent, int mTorpedoes, int mShipsLeft) // Properly display the text
{
    string newScore, newRound, newTorpedoes, newShipsLeft, newEnergyPercent;
    
    // Convert score
    if (mScore < 10)
    {
        stringstream convertScore;
        convertScore << mScore;
        newScore = "000000" + convertScore.str();
    }
    else if (mScore < 100)
    {
        stringstream convertScore;
        convertScore << mScore;
        newScore = "00000" + convertScore.str();
    }
    else if (mScore < 1000)
    {
        stringstream convertScore;
        convertScore << mScore;
        newScore = "0000" + convertScore.str();
    }
    else if (mScore < 10000)
    {
        stringstream convertScore;
        convertScore << mScore;
        newScore = "000" + convertScore.str();
    }
    else if (mScore < 100000)
    {
        stringstream convertScore;
        convertScore << mScore;
        newScore = "00" + convertScore.str();
    }
    else if (mScore < 1000000)
    {
        stringstream convertScore;
        convertScore << mScore;
        newScore = "0" + convertScore.str();
    }
    else
    {
        stringstream convertScore;
        convertScore << mScore;
        newScore = convertScore.str();
    }
    
    // Convert round
    if (mRound < 10)
    {
        stringstream convertRound;
        convertRound << mRound;
        newRound = "0" + convertRound.str();
    }
    else
    {
        stringstream convertRound;
        convertRound << mRound;
        newRound = convertRound.str();
    }
    
    // Convert energy percent
    stringstream convertEnergy;
    convertEnergy << mEnergyPercent;
    newEnergyPercent = convertEnergy.str();
    
    // Convert torpedoes
    stringstream convertTorpedo;
    convertTorpedo << mTorpedoes;
    newTorpedoes = convertTorpedo.str();
    
    // Convert ships
    if (mShipsLeft < 10)
    {
        stringstream convertShip;
        convertShip << mShipsLeft;
        newShipsLeft = "0" + convertShip.str();
    }
    else
    {
        stringstream convertShip;
        convertShip << mShipsLeft;
        newShipsLeft = convertShip.str();
    }
    
    string X = "Score: " + newScore + "  Round: " + newRound + "  Energy: " + newEnergyPercent + "%  Torpedoes: " + newTorpedoes + "  Ships: " + newShipsLeft;
    
    return X;
}

void StudentWorld::doSomethingForEverything() // Calls doSomething() for all actors
{
    m_ship->doSomething();
    list<Actor*>::iterator ptr = m_actors.begin();
    while (ptr != m_actors.end())
    {
        Actor* S = *ptr;
        S->doSomething();
        ptr++;
    }
}

void StudentWorld::removeDeadGameObjects() // Removes any objects that are dead
{
    if (m_actors.size() == 0)
        return;
    for (list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); )
    {
        if ((*it)->isDead() == true)
        {
            delete (*it);
            it = m_actors.erase(it);
        }
        else
            it++;
    }
}

void StudentWorld::addActor(Actor* a) // Add an Actor to the list
{
    m_actors.push_back(a);
}

int StudentWorld::getRound() // Get the round number
{
    return m_round;
}

int StudentWorld::getRoundFactor() // Get the round factor number
{
    return getRound()*0.1 + 0.9;
}

int StudentWorld::getNumAliensToKill()  // Return the number of aliens that the player needs to kill to advance round
{
    return (4*getRound());
}

int StudentWorld::IncAliensKilled() // Increases the number of aliens killed
{
    m_numOfAliensKilled++;
    if (m_numOfAliensKilled == getNumAliensToKill())
    {
        m_round++;
        m_numOfAliensKilled = 0;
    }
}

int StudentWorld::playerX() const                   // Return the player ship's X-coordinate
{
    return m_ship->getX();
}

int StudentWorld::playerY() const                   // Return the player ship's Y-coordinate
{
    return m_ship->getY();
}

int StudentWorld::getNumAlienFiredProjectiles() // Get the number of alien-fired projectiles in the space field
{
    int count = 0;
    list<Actor*>::iterator ptr;
    ptr = m_actors.begin();
    for (ptr = m_actors.begin(); ptr != m_actors.end(); ptr++)
    {
        Projectile* X = dynamic_cast<Projectile*>(*ptr);
        if (X != NULL && X->playerFired() == false)
        {
            count++;
        }
    }
    return count;
}

int StudentWorld::numOfAliens()
{                                                                   // returns number of Aliens in the space field
    int count = 0;
    list<Actor*>::iterator ptr;
    for (ptr = m_actors.begin(); ptr != m_actors.end(); ptr++)
    {
        Alien* X = dynamic_cast<Alien*>(*ptr);
        if (X != nullptr)
            count++;
    }
    return count;
}


Player* StudentWorld::getPlayer()
{
    return m_ship;
}

std::list<Alien*> StudentWorld::getCollidingAliens(Actor* a) // Get a list of aliens at the same location as the actor
{
    list<Actor*>::iterator ptr;
    list<Alien*> m_aliens;
    for (ptr = m_actors.begin(); ptr != m_actors.end(); ptr++)
    {
        Alien* X = dynamic_cast<Alien*>(*ptr);                          // if the Actor is an Alien and coordinates
        if ((X != nullptr) && (a->getX() == X->getX()) && (a->getY() == X->getY())) // are the same, add it to the Alien list
            m_aliens.push_back(X);
    }
    return m_aliens;                                                    // Return list of aliens at same location
}

Player* StudentWorld::getCollidingPlayer(Actor* a)
{
    // Get a pointer to the player if at the same location as the actor, otherwise NULL
    if (a->getX() == playerX() && a->getY() == playerY())
        return m_ship;
    return nullptr;
}