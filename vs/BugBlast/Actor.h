#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include <queue>

class StudentWorld;

class Actor: public GraphObject
{
public:
	Actor(int imageID, int startX, int startY, StudentWorld* world): GraphObject(imageID, startX, startY), m_isDead(false), m_world(world) {}
	bool isDead() const;
	virtual void doSomething() = 0;
	StudentWorld* getWorld() const;
	void setDead() { m_isDead = true; }
	virtual ~Actor() {}
private:
	bool m_isDead;
	StudentWorld* m_world;
};

class Player: public Actor
{
public:
	Player(int startX, int startY, StudentWorld* world);
	void addBugSprayers(int n);
	virtual void doSomething();
	void walkThroughWalls(bool able);
	void setMaxBugSprayers(int n);
	int getNumberofBugSprayers() const;
	int getMaximumBugSprayers() const;
	virtual ~Player();
private:
	int m_nBugSprayers;
	bool m_walkThroughBricks;
	int m_timeWalkThroughBricks;
	int m_timeSprayerGoodies;
	int m_maxBugSprayers;
};

class Zumi: public Actor
{
public:
	Zumi(int imageID, int startX, int startY, StudentWorld* world);
	int getSpeed() const;
	void setSleepTime(int amount);
	int getDirection() const;
	void setDirection(int dir);
	virtual ~Zumi();
private:
	int m_speed;
	int m_dir;
};

class SimpleZumi: public Zumi
{
public:
	SimpleZumi(int startX, int startY, StudentWorld* world): Zumi(IID_SIMPLE_ZUMI, startX, startY, world) {}
	virtual void doSomething();
	virtual ~SimpleZumi();
private:
};

class ComplexZumi: public Zumi
{
public:
	ComplexZumi(int startX, int startY, StudentWorld* world): Zumi(IID_COMPLEX_ZUMI, startX, startY, world) {}
	virtual void doSomething();
	virtual ~ComplexZumi();
private:
	std::queue<int> m_route;
	struct Marked
	{
		bool visited;
		int color;
	};
};

class Brick: public Actor
{
public:
	Brick(int imageID, int startX, int startY, StudentWorld* world);
	virtual void doSomething();
	virtual ~Brick() {}
private:
};

class DestroyableBrick: public Brick
{
public:
	DestroyableBrick(int startX, int startY, StudentWorld* world): Brick(IID_DESTROYABLE_BRICK, startX, startY, world) {}
	virtual ~DestroyableBrick();
private:
};

class PermanentBrick: public Brick
{
public:
	PermanentBrick(int startX, int startY, StudentWorld* world): Brick(IID_PERMA_BRICK, startX, startY, world) {}
	virtual ~PermanentBrick() {}
private:
};

class Exit: public Actor
{
public:
	Exit(int startX, int startY, StudentWorld* world);
	virtual void doSomething();
	virtual ~Exit() {}
private:
};

class BugSprayer: public Actor
{
public:
	BugSprayer(int startX, int startY, StudentWorld* world);
	virtual void doSomething(); 
	virtual ~BugSprayer();
private:
	int m_lifeTime;
};

class BugSpray: public Actor
{
public:
	BugSpray(int startX, int startY, StudentWorld* world);
	virtual void doSomething();
	virtual ~BugSpray() {}
private:
	int m_lifeTime; // 3 ticks
};

class Goodie: public Actor
{
public:
	Goodie(int imageID, int startX, int startY, StudentWorld* world);
	int getLifeTime() const;
	void decrementLifeTime();
	virtual ~Goodie();
private:
	int m_lifeTime;
};

class ExtraLifeGoodie: public Goodie
{
public:
	ExtraLifeGoodie(int startX, int startY, StudentWorld* world): Goodie(IID_EXTRA_LIFE_GOODIE, startX, startY, world) {}
	virtual void doSomething();
	virtual ~ExtraLifeGoodie() {}
private:
};

class WalkThroughWallsGoodie: public Goodie
{
public:
	WalkThroughWallsGoodie(int startX, int startY, StudentWorld* world): Goodie(IID_WALK_THRU_GOODIE, startX, startY, world) {}
	virtual void doSomething();
	virtual ~WalkThroughWallsGoodie() {}
private:
};

class IncreaseSimultaneousBugSprayer: public Goodie
{
public:
	IncreaseSimultaneousBugSprayer(int startX, int startY, StudentWorld* world)
		: Goodie(IID_INCREASE_SIMULTANEOUS_SPRAYER_GOODIE, startX, startY, world) {}
	virtual void doSomething();
	virtual ~IncreaseSimultaneousBugSprayer() {}
private:
};

#endif