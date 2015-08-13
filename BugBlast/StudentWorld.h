#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <vector>
#include "Level.h"

class Actor;
class Player;
class Level;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	void setDisplayText(); // pg.18
	int getLevelData(int x, int y) const;
	void setLevelData(int x, int y, int ID);
	Player* getAvatar() const;
	Actor* getActor(int ID) const;
	void addActor(Actor* newActor);
	void setActorAsDead(int x, int y);
	Level* getLvl() const;
	bool allZumiDead() const;
	void setLevelFinished();
	unsigned int getBonus() const;
	void decrementBonus();
	virtual ~StudentWorld() {}
private:
	std::vector<Actor*> m_actors;
	Player* m_avatar;
	int m_maze[VIEW_HEIGHT][VIEW_WIDTH];
	Level* m_lvl;
	bool m_hasCompletedLevel;
	unsigned int m_bonus;
};

#endif
