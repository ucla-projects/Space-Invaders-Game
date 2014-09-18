#ifndef _STUDENTWORLD_H_
#define _STUDENTWORLD_H_

#include "GameWorld.h"
#include <list>
#include <string>

class Actor;
class Player;
class Alien;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(): m_numStars(0), m_round(1), m_numOfAliensKilled(0) {}
    virtual ~StudentWorld();
	virtual void init();
	virtual int move();
    
    void addAliensOrStars(); // Add aliens or stars to space field
    void updateDisplayText(); // Update the score/lives/round at screen top
    std::string properDisplayText(int mScore, int mRound, double mEnergyPercent, int mTorpedoes, int mShipsLeft);
    void doSomethingForEverything(); // Calls doSomething() for all actors
    void removeDeadGameObjects(); // Removes any objects that are dead
    
	virtual void cleanUp();
    
    void addActor(Actor* a); // Add an actor to the space field
    
    int getRound(); // Get the round number
    int getRoundFactor(); // Get the round factor
    int getNumAliensToKill(); // Get the number of aliens needed to kill
    int IncAliensKilled(); // Increment number of aliens killed
    
    int playerX() const; // Get player's x-coordinate
    int playerY() const; // Get player's y-coordinate
    
    int getNumAlienFiredProjectiles(); // Get # of alien projectiles in space field
    int numOfAliens(); // Get number of aliens in space field
    
    Player* getPlayer(); // Get pointer to player
    std::list<Alien*> getCollidingAliens(Actor* a); // Get list of aliens at same location as actor
    Player* getCollidingPlayer(Actor* a); // Get pointer to player if at same location as actor, otherwise NULL
    
private:
    int                 m_numStars;
    int                 m_round, m_roundFactor;
    int                 m_numOfAliensKilled;
    Player*             m_ship;
    std::list<Actor*>   m_actors;
};

#endif // _STUDENTWORLD_H_
