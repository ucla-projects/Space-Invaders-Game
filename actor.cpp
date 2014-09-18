#include "actor.h"
#include "StudentWorld.h"
#include <list>
#include <iostream>

// Implementations of ACTOR class

void Actor::moveBy(int deltaX, int deltaY)
{
    moveTo(getX()+deltaX, getY()+deltaY);
    if (getY() < 0 || getY() >= VIEW_HEIGHT) // if actor is off the screen, set it to dead
        setDead();
}

bool Actor::isDead() const // Return true if the Actor is dead, false otherwise
{
    if (m_isDead == true)
        return true;
    return false;
}

void Actor::setDead() // Set Actor to dead
{
    m_isDead = true;
}

StudentWorld* Actor::getWorld() // Get the actor's world
{
    return m_world;
}

// Implementations of STAR class

void Star::doSomething()
{
    moveBy(0, -1);
}

// Implementations of the PROJECTILE class
void Projectile::doSomething()
{
    if (m_playerFired == true)      // player fired
    {
        std::list<Alien*> A = getWorld()->getCollidingAliens(this);
        if (A.size() > 0)
        {
            std::list<Alien*>::iterator it;
            for (it = A.begin(); it != A.end(); it++)
            {
                (*it)->damage(m_damagePoints, true);
            }
            setDead();
            return;
        }
        
        moveBy(0, 1);           // didn't collide with any Aliens, so move
        A = getWorld()->getCollidingAliens(this);       // double check for collision after movement
        if (A.size() > 0)
        {
            std::list<Alien*>::iterator it;
            for (it = A.begin(); it != A.end(); it++)
            {
                (*it)->damage(m_damagePoints, true);
            }
            setDead();
            return;
        }

    }
    
    else                            // alien fired
    {
        Player* A = getWorld()->getCollidingPlayer(this);
        if (A != nullptr)           // if the projectile hit the player
        {
            getWorld()->getPlayer()->damage(m_damagePoints, true);
            setDead();
            return;
        }

        moveBy(0, -1);             // didn't collide with Player, so move
        A = getWorld()->getCollidingPlayer(this);      // double check for collision after movement
        if (A != nullptr)           // if the projectile hit the player
        {
            getWorld()->getPlayer()->damage(m_damagePoints, true);
            setDead();
            return;
        }
    }
    
    if (getY() < 0 || getY() >= VIEW_HEIGHT)
        setDead();
}

bool Projectile::playerFired() const // Was this projectile fired by the player (as opposed to an alien)?
{
    return m_playerFired;
}

// Implementations of GOODIE class
void Goodie::doSomething()
{
    moveBy(0, -1);
    doSpecialAction(getWorld()->getPlayer());
}

int Goodie::getGoodieTickLifetime() const
{
    return goodieTickLifetime;
}

int Goodie::getTicksLeftToLive() const
{
    return m_ticksLeftToLive;
}

bool Goodie::threeTicksPassed()
{
    if (m_ticksPassed == 2)
    {
        m_ticksPassed = 0;
        return true;
    }
    else
    {
        m_ticksPassed--;
        return false;
    }
}

// Implementations of FREE SHIP GOODIE class

void FreeShipGoodie::doSpecialAction(Player* p)
{
    // if the free ship goodie runs into the player ship, it will increase the player's number of lives by 1
    if (getX() == getWorld()->playerX() && getY() == getWorld()->playerY())
    {
        getWorld()->incLives();
        getWorld()->increaseScore(5000);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        setDead();
        return;
    }
    else                                        // the free ship goodie has not collided with the player ship
    {
        int Brightness = getTicksLeftToLive()/getGoodieTickLifetime();
        setBrightness(Brightness);
    }
    if (getTicksLeftToLive() == 0)
    {
        setDead();
        return;
    }
    else
    {
        if (threeTicksPassed() == true)        // the free ship goodie moves every 3 ticks
            moveBy(0, -1);
        if (getY() < 0)                        // delete goodie if it's off the field
            setDead();
        if (getX() == getWorld()->playerX() && getY() == getWorld()->playerY())
        {
            getWorld()->increaseScore(5000);
            getWorld()->playSound(SOUND_GOT_GOODIE);
            getWorld()->incLives();
            setDead();
            return;
        }
    }
}

// Implementations of ENERGY GOODIE class
void EnergyGoodie::doSpecialAction(Player* p)
{
    // if the energy goodie runs into the player ship, it will restore the player's energy to full
    if (getX() == getWorld()->playerX() && getY() == getWorld()->playerY())
    {
        getWorld()->increaseScore(5000);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->getPlayer()->restoreFullEnergy();
        setDead();
        return;
    }
    else                                        // the energy goodie has not collided with the player ship
    {
        int Brightness = getTicksLeftToLive()/getGoodieTickLifetime();
        setBrightness(Brightness);
    }
    if (getTicksLeftToLive() == 0)
    {
        setDead();
        return;
    }
    else
    {
        if (threeTicksPassed() == true)        // the energy goodie moves every 3 ticks
            moveBy(0, -1);
        if (getY() < 0)                        // delete goodie if it's off the field
            setDead();
        if (getX() == getWorld()->playerX() && getY() == getWorld()->playerY())
        {
            getWorld()->increaseScore(5000);
            getWorld()->playSound(SOUND_GOT_GOODIE);
            getWorld()->getPlayer()->restoreFullEnergy();
            setDead();
            return;
        }
    }
}

// Implementations of TORPEDO GOODIE class
void TorpedoGoodie::doSpecialAction(Player* p)
{
    // if the free ship goodie runs into the player ship, it will increase the player's number of lives by 1
    if (getX() == getWorld()->playerX() && getY() == getWorld()->playerY())
    {
        getWorld()->increaseScore(5000);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->getPlayer()->addTorpedoes(5);
        setDead();
        return;
    }
    else                                        // the free ship goodie has not collided with the player ship
    {
        int Brightness = getTicksLeftToLive()/getGoodieTickLifetime();
        setBrightness(Brightness);
    }
    if (getTicksLeftToLive() == 0)
    {
        setDead();
        return;
    }
    else
    {
        if (threeTicksPassed() == true)        // the free ship goodie moves every 3 ticks
            moveBy(0, -1);
        if (getY() < 0)                        // delete goodie if it's off the field
            setDead();
        if (getX() == getWorld()->playerX() && getY() == getWorld()->playerY())
        {
            getWorld()->increaseScore(5000);
            getWorld()->playSound(SOUND_GOT_GOODIE);
            getWorld()->getPlayer()->addTorpedoes(5);
            setDead();
            return;
        }
    }
}

// Implementations of SHIP class

int Ship::getEnergy() const // How much life energy does the ship have?
{
    return m_energy;
}

double Ship::getEnergyPct() const // What percentage of the starting life energy does the ship have?
{
    return (double(getEnergy())/m_startEnergy) * 100.0;
}

void Ship::decreaseEnergy(int points) // Decrease the life energy by the number of points
{
    m_energy -= points;
}

void Ship::restoreFullEnergy() // Restore the life energy level to the starting level
{
    m_energy = m_startEnergy;
}

void Ship::launchProjectile(ProjectileType pt, bool playerFired)
{
    if (pt == BULLET && playerFired == false)                           // alien fired bullet
        getWorld()->addActor(new Bullet(getWorld(), getX(), getY()-1, false));
    else if (pt == TORPEDO && playerFired == false)                     // alien fired torpedo
        getWorld()->addActor(new Torpedo(getWorld(), getX(), getY()-1, false));
    else if (pt == BULLET && playerFired == true)                       // player fired bullet
        getWorld()->addActor(new Bullet(getWorld(), getX(), getY()+1, true));
    else                                                                // player fired torpedo
        getWorld()->addActor(new Torpedo(getWorld(), getX(), getY()+1, true));
}

// Implementations of PLAYER class

void Player::doSomething()
{    
    std::list<Alien*> m_collidedAliens = getWorld()->getCollidingAliens(this);  // check if Player has hit any Aliens
    if (m_collidedAliens.size() > 0)
    {
        damage(15*m_collidedAliens.size(), false);
        std::list<Alien*>::iterator it;
        for (it = m_collidedAliens.begin(); it != m_collidedAliens.end(); it++)
        {
            (*it)->setDead();
        }
    }
    
    int key = 0;
    if (getWorld()->getKey(key))
    {
        switch(key)
        {
            case(KEY_PRESS_DOWN):
                if (getY() > 0)
                    moveBy(0, -1);
                break;
            case(KEY_PRESS_UP):
                if (getY() < VIEW_HEIGHT-1)
                    moveBy(0, 1);
                break;
            case(KEY_PRESS_LEFT):
                if (getX() > 0)
                    moveBy(-1, 0);
                break;
            case(KEY_PRESS_RIGHT):
                if (getX() < VIEW_WIDTH-1)
                    moveBy(1, 0);
                break;
            case(KEY_PRESS_SPACE):      // player fired bullet
                if (m_ticksSinceFired == 1)
                {
                    launchProjectile(BULLET, true);
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                    m_ticksSinceFired = 0;
                    break;
                }
                if (m_ticksSinceFired == 0)
                    m_ticksSinceFired = 1;
                break;
            case(KEY_PRESS_TAB):
                if (getNumTorpedoes() == 0)
                    break;
                if (m_ticksSinceFired == 1)
                {
                    launchProjectile(TORPEDO, true);
                    getWorld()->playSound(SOUND_PLAYER_TORPEDO);
                    decNumTorpedoes();
                    m_ticksSinceFired = 0;
                    break;
                }
                if (m_ticksSinceFired == 0)
                    m_ticksSinceFired = 1;
                break;
        }
    }
    if (key != KEY_PRESS_SPACE && key != KEY_PRESS_TAB)
        m_ticksSinceFired = 1;
    
    m_collidedAliens = getWorld()->getCollidingAliens(this);  // double check if Player has collided with Aliens
    if (m_collidedAliens.size() > 0)
    {
        damage(15*m_collidedAliens.size(), false);
        std::list<Alien*>::iterator it;
        for (it = m_collidedAliens.begin(); it != m_collidedAliens.end(); it++)
        {
            (*it)->setDead();
        }
    }
}

void Player::damage(int points, bool hitByProjectile)
{
    getWorld()->getPlayer()->decreaseEnergy(points);
    if (hitByProjectile == false)
        getWorld()->playSound(SOUND_ENEMY_PLAYER_COLLISION);
    else                                            // player was hit by a projectile
        getWorld()->playSound(SOUND_PLAYER_HIT);
    if (getEnergy() <= 0)
    {
        getWorld()->getPlayer()->setDead();
        getWorld()->playSound(SOUND_PLAYER_DIE);
    }
    return;
}

int Player::getNumTorpedoes() const // How many torpedoes does this player have?
{
    return m_torpedoes;
}

void Player::addTorpedoes(int n) // Add n torpedoes to the players arsenal.
{
    m_torpedoes += n;
}

void Player::decNumTorpedoes()
{
    m_torpedoes--;
}

// Implementations of ALIEN class

void Alien::damage(int points, bool hitByProjectile)
{
    
    decreaseEnergy(points);             // when an Alien is hit
    if (getEnergy() <= 0)
    {
        getWorld()->playSound(SOUND_ENEMY_DIE);
        if (hitByProjectile == true)
        {
            getWorld()->IncAliensKilled();
            getWorld()->increaseScore(m_worth);
            maybeDropGoodie();
        }
        setDead();
    }
    else
    {
        if (m_hitWithinLastTick == true)    // Update member to know if Alien was hit within the last tick
            m_hitWithinLastTick = false;
        else
            m_hitWithinLastTick = true;
        getWorld()->playSound(SOUND_ENEMY_HIT);
    }
}

// Is this a tick during which the alien can do something?
bool Alien::canAct()
{
    if (m_canActNextTick == true)
    {
        m_canActNextTick = false;
        return true;
    }
    else                // (m_canActNextTick == false)
    {
        m_canActNextTick = true;
        return false;
    }
}

void Alien::setCanAct(bool canActNextTurn)
{
    m_canActNextTick = canActNextTurn;
}

// fire a projectile of the indicated type
void Alien::fireProjectile(ProjectileType pt)
{
    launchProjectile(pt, false);
}

bool Alien::wasHitWithinLastTick()
{
    return m_hitWithinLastTick;
}

// Implementations of NACHLINGBASE class

void NachlingBase::doSomething()
{
    if (canAct() == false)
    {
        setCanAct(true);
        return;
    }
    else
    {
        setCanAct(false);
        if (m_state == 0)
        {
            if (getX() != getWorld()->playerX())
            {
                int temp2 = rand() %3;
                if (temp2 == 0)                                     // 1/3 chance
                {
                    if (getWorld()->playerX() > getX())             // player's x-coord is greater than alien's
                        moveBy(1, -1);
                    else                                            // player's x-coord is smaller than alien's
                        moveBy(-1, -1);
                    return;
                }
            }
            else                                                // player's x-coord is same as alien's
            {
                if (getX() != 1 && getX() != VIEW_WIDTH-1)
                {
                    m_state = 1;
                    if (getX() < 15)                            // step 4bii
                        MDB = getX();                           // MDB is closest distance to border
                    else
                        MDB = VIEW_WIDTH-1-getX();
                    if (MDB > 3)                                // step 4biii
                        HMD = (rand() %3) + 1;                  // HMD is between 1 and 3 or equal to MDB
                    else
                        HMD = MDB;
                    
                    m_direction = rand() %2;                    // step 4biv
                    HMR = HMD;                                  // Choose a random horizontal movement direction
                }
            }
            moveBy(0, -1);                                          // move towards bottom of space field
            if (getY()<0)
                setDead();
            return;
        }
        
        if (m_state == 1)
        {
            if (getWorld()->getPlayer()->getY() > getY())
            {
                m_state = 2;
                return;
            }
            if (HMR == 0)
            {
                if (m_direction == 0)
                    m_direction = 1;                                // switch the direction
                else
                    m_direction = 0;
                HMR = 2 * HMD;
            }
            else
                HMR--;
            
            if (m_direction == 0)
                moveBy(-1, 0);                                      // take one step in m_direction
            else
                moveBy(1, 0);
            
            int chancesOfFiring = int(BASE_CHANCE_OF_FIRING/getWorld()->getRound())+1;
            int temp = rand() %chancesOfFiring;
            if (temp == 0)
            {
                int count = getWorld()->getNumAlienFiredProjectiles();
                if (count < 2 * getWorld()->getRound())
                {
                    launchProjectile(BULLET, false);
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                }
            }
            
            int temp2 = rand() %20;                                 // 1 in 20 chance to go to state 2
            if (temp2 == 0)
                m_state = 2;
            return;
        }
        
        if (m_state == 2)
        {
            if (getY() == VIEW_HEIGHT-1)
            {
                m_state = 0;
                return;
            }
            if (getX() == 0)                                        // if Nachling is on the left side, go right
                m_direction = 1;
            else if (getX() == VIEW_WIDTH-1)                        // if Nachling is on the right side, go left
                m_direction = 0;
            else
            {
                int temp2 = rand() %2;
                if (temp2 == 0)
                    m_direction = 0;
                else
                    m_direction = 1;
            }
            if (m_direction == 0)                                   // go one step in the respective direction and one step up
                moveBy(-1, 1);
            else
                moveBy(1, 1);
            return;
        }
    }
}

void Nachling::maybeDropGoodie()
{
    return;
}

// Implementations of WEALTHY NACHLING CLASS

void WealthyNachling::doSomething()
{
    if (m_ticksPassed == 30)                                      // when malfunctioning, 30 ticks pass
    {
        m_ticksPassed = 0;
        m_isMalfunctioning = false;
    }
    if (m_isMalfunctioning == true)
    {
        m_ticksPassed++;
        return;
    }
    int temp2 = rand() %200;                                    // 1/200 chance of malfunctioning
    if (temp2 == 0)
    {
        m_isMalfunctioning = true;
        return;
    }
    NachlingBase::doSomething();
}

void WealthyNachling::maybeDropGoodie()
{
    int temp = rand() %3;
    if (temp == 0)              // 1/3 chance to drop a goodie
    {
        int temp2 = rand() %2;
        if (temp2 == 0)         // 1/2 chance the goodie will be energy / torpedo goodie
            getWorld()->addActor(new EnergyGoodie(getWorld(), getX(), getY()));
        else
            getWorld()->addActor(new TorpedoGoodie(getWorld(), getX(), getY()));
    }
}

// Implementations of SMALLBOT class

void Smallbot::doSomething()
{
    if (canAct() == false)
    {
        
        return;
    }
    int temp = rand() %3;
    if (wasHitWithinLastTick() == true && temp == 0) // If smallbot wasn't within last tick, 1/3 chance to move
    {
        if (getX() == 0)
            moveBy(1, -1);
        else if (getX() == VIEW_WIDTH-1)
            moveBy(-1, -1);
        else
        {
            int temp2 = rand() %2;
            if (temp2 == 0)
                moveBy(-1, -1);
            else
                moveBy(1, -1);
        }
    }
    else
    {
        moveBy(0, -1);
    }
    if (getX() == getWorld()->playerX() && getY() > getWorld()->playerY())
    {
        int q = int(100.0/getWorld()->getRound())+1;
        int temp2 = rand() %q;
        if (temp2 == 0)
        {
            if (getWorld()->getNumAlienFiredProjectiles() < 2*getWorld()->getRound())
            {
                launchProjectile(TORPEDO, false);
                getWorld()->playSound(SOUND_ENEMY_FIRE);
            }
        }
        else
        {
            if (getWorld()->getNumAlienFiredProjectiles() < 2*getWorld()->getRound())
            {
                launchProjectile(BULLET, false);
                getWorld()->playSound(SOUND_ENEMY_FIRE);
            }
        }
    }
    if (getY() < 0)
        setDead();
}

void Smallbot::maybeDropGoodie()
{
    int temp = rand() %3;       // 1/3 chance to drop a Free Ship Goodie
    if (temp== 0)
        getWorld()->addActor(new FreeShipGoodie(getWorld(), getX(), getY()));
}
