#ifndef _ACTOR_H_
#define _ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"  
#include "StudentWorld.h"

class Actor : public GraphObject
{
public:
    Actor(StudentWorld* world, int imageID, int startX, int startY): GraphObject(imageID, startX, startY), m_isDead(false)
    {
        m_world = world;
        setVisible(true);
    }
    virtual ~Actor() {}
    virtual void doSomething() = 0; // Action to perform each tick
    void moveBy(int deltaX, int deltaY); // Move the actor by -1, 0, or +1 in each coordinate, marking as dead if off screen. If actor is off the screen, set it to dead
    bool isDead() const; // Is this actor dead?
    void setDead(); // Mark this actor as dead
    StudentWorld* getWorld(); // Get the actor's world
private:
    bool          m_isDead;
    int           m_health;
    StudentWorld* m_world;
};

class Star : public Actor
{
public:
    Star(StudentWorld* world): Actor(world, IID_STAR, rand() %VIEW_WIDTH, VIEW_HEIGHT-1) {}
    virtual ~Star() {}
    virtual void doSomething();
};

class Projectile : public Actor
{
public:
    Projectile(StudentWorld* world, int imageID, int startX, int startY, bool playerFired, int damagePoints): Actor(world, imageID, startX, startY)
    {
        m_playerFired = playerFired;
        m_damagePoints = damagePoints;
    }
    virtual ~Projectile() {}
    virtual void doSomething();
    bool playerFired() const; // Was this projectile fired by the player (as opposed to an alien)?
    
private:
    bool m_playerFired;
    int  m_damagePoints;
};

class Bullet : public Projectile
{
public:
    Bullet(StudentWorld* world, int startX, int startY, bool playerFired): Projectile(world, IID_BULLET, startX, startY, playerFired, 2) {}
    virtual~ Bullet() {}
};

class Torpedo : public Projectile
{
public:
    Torpedo(StudentWorld* world, int startX, int startY, bool playerFired): Projectile(world, IID_TORPEDO, startX, startY, playerFired, 8) {}
    virtual ~Torpedo() {}
};

class Goodie : public Actor
{
public:
    Goodie(StudentWorld* world, int imageID, int startX, int startY): Actor(world, imageID, startX, startY), goodieTickLifetime(int(100.0/world->getRound())+30), m_ticksLeftToLive(goodieTickLifetime), m_ticksPassed(0) {}
    virtual ~Goodie() {}
    virtual void doSomething();
    int getGoodieTickLifetime() const;  // Return goodie tick lifetime
    int getTicksLeftToLive() const;     // Return ticks left to live
    bool threeTicksPassed();            // Return true if three ticks have passed
    virtual void doSpecialAction(Player* p) = 0; // Perform the action specific to the kind of goodie when the
    // player retrieves it
private:
    int goodieTickLifetime, m_ticksLeftToLive, m_ticksPassed;
};

class FreeShipGoodie : public Goodie
{
public:
    FreeShipGoodie(StudentWorld* world, int startX, int startY): Goodie(world, IID_FREE_SHIP_GOODIE, startX, startY) {}
    virtual~ FreeShipGoodie() {}
    virtual void doSpecialAction(Player* p);
};

class EnergyGoodie : public Goodie
{
public:
    EnergyGoodie(StudentWorld* world, int startX, int startY): Goodie(world, IID_ENERGY_GOODIE, startX, startY) {}
    virtual~ EnergyGoodie() {}
    virtual void doSpecialAction(Player* p);
};

class TorpedoGoodie : public Goodie
{
public:
    TorpedoGoodie(StudentWorld* world, int startX, int startY): Goodie(world, IID_TORPEDO_GOODIE, startX, startY) {}
    virtual~ TorpedoGoodie() {}
    virtual void doSpecialAction(Player* p);
};

class Ship : public Actor
{
public:
    enum ProjectileType { BULLET, TORPEDO };
    
    Ship(StudentWorld* world, int imageID, int startX, int startY, int startEnergy): Actor(world, imageID, startX, startY), m_energy(startEnergy), m_startEnergy(startEnergy) {}
    virtual ~Ship() {}
    virtual void damage(int points, bool hitByProjectile) = 0;
    int getEnergy() const; // How much life energy does the ship have?
    double getEnergyPct() const; // What percentage of the starting life energy does the ship have?
    void decreaseEnergy(int points); // Decrease the life energy by the number of points
    void restoreFullEnergy(); // Restore the life energy level to the starting level
    void launchProjectile(ProjectileType pt, bool playerFired); // Launch a projectile of the indicated type, fired by either the player
private:
    int        m_energy;
    const int  m_startEnergy;
};

class Player : public Ship
{
public:
    Player(StudentWorld* world): Ship(world, IID_PLAYER_SHIP, VIEW_WIDTH/2, 1, 50), m_lives(3), m_torpedoes(0), m_ticksSinceFired(1) {}
    virtual ~Player() {}
    virtual void doSomething();
    virtual void damage(int points, bool hitByProjectile); // Cause the indicated number of points damage to the player, who was either hit by a projectile or collided with an alien
    void addTorpedoes(int n); // Add n torpedoes to the players arsenal
    int getNumTorpedoes() const; // How many torpedoes does this player have?
    void decNumTorpedoes(); // Decrement the number of torpedoes
private:
    int   m_lives, m_torpedoes;
    int   m_ticksSinceFired;
};

class Alien : public Ship
{
public:
    Alien(StudentWorld* world, int imageID, int startEnergy, int worth): Ship(world, imageID, rand() %VIEW_WIDTH, VIEW_HEIGHT-1, startEnergy), m_worth(worth), m_canActNextTick(true), m_hitWithinLastTick(false) {}
    virtual~ Alien() {}
    virtual void damage(int points, bool hitByProjectile); // Cause the indicated number of points damage to the alien, who was either hit by a projectile or collided with the player
    virtual void maybeDropGoodie() = 0; // Possibly drop a goodie
    void fireProjectile(ProjectileType pt); // fire a projectile of the indicated type
    bool wasHitWithinLastTick(); // returns if the Alien was hit within the last tick
    bool canAct(); // Is this a tick during which the alien can do something?
    void setCanAct(bool canActNextTurn); // Allow the Alien to move next tick
    
private:
    int m_worth;
    bool m_canActNextTick;
    bool m_hitWithinLastTick;
};

class NachlingBase : public Alien
{
public:
    NachlingBase(StudentWorld* world, int imageID, int startEnergy, int worth): Alien(world, imageID, startEnergy, worth), m_state(0), m_direction(0), MDB(0), HMD(0), HMR(0) {}
    virtual ~NachlingBase() {}
    virtual void doSomething();
private:
    int      m_state, m_direction;
    int      MDB, HMD, HMR; // MDB(minimum distance to border), HMD(horizontal movement distance), HMR(horizontal movement remaining)
    static int const BASE_CHANCE_OF_FIRING = 10;
};

class Nachling : public NachlingBase
{
public:
    Nachling(StudentWorld* world, int round): NachlingBase(world, IID_NACHLING, world->getRound() * 5, 1000) {}
    virtual~ Nachling() {}
    virtual void maybeDropGoodie();
private:
};

class WealthyNachling : public NachlingBase
{
public:
    WealthyNachling(StudentWorld* world, int round): NachlingBase(world, IID_WEALTHY_NACHLING, world->getRound() * 8, 1200), m_isMalfunctioning(false), m_ticksPassed(0) {}
    virtual ~WealthyNachling() {}
    virtual void doSomething();
    virtual void maybeDropGoodie();
private:
    int     m_ticksPassed;
    bool    m_isMalfunctioning;
};

class Smallbot : public Alien
{
public:
    Smallbot(StudentWorld* world, int round): Alien(world, IID_SMALLBOT, world->getRoundFactor() * 12, 1500) {}
    virtual ~Smallbot() {}
    virtual void doSomething();
    virtual void maybeDropGoodie();
private:
};

#endif // _ACTOR_H_