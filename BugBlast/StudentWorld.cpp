#include "StudentWorld.h"
#include "GameConstants.h"
#include "GameWorld.h"
#include "Actor.h"
#include "Level.h"
#include <vector>
#include <sstream>
#include <string>
using namespace std;

GameWorld* createStudentWorld()
{
	return new StudentWorld();
}

StudentWorld::StudentWorld()
{
	m_avatar = nullptr;
	for (int i = 0; i < VIEW_HEIGHT; i++)
		for (int j = 0; j < VIEW_WIDTH; j++)
			m_maze[i][j] = -1; // Indicate empty
	m_hasCompletedLevel = false;
}

int StudentWorld::init()
{
	m_hasCompletedLevel = false;
	int currentLevel = getLevel();
	// Convert next level's file name to a string:
	ostringstream fileName;
	if (currentLevel < 10)
		fileName << "level0" << currentLevel << ".dat";
	else
		fileName << "level" << getLevel() << ".dat";
	string file = fileName.str();

	// Load the next level with actors if it exists:
	m_lvl = new Level;
	Level::LoadResult result;
	result = m_lvl->loadLevel(file);
	if (result == Level::load_fail_bad_format)
		return GWSTATUS_LEVEL_ERROR;
	else if (result == Level::load_fail_file_not_found && currentLevel == 0)
		return GWSTATUS_NO_FIRST_LEVEL;
	else if (result == Level::load_fail_file_not_found)
		return GWSTATUS_PLAYER_WON;
	else if (result == Level::load_success)
	{
		for (int i = 0; i < VIEW_HEIGHT; i++)
		{
			for (int j = 0; j < VIEW_WIDTH; j++)
			{
				Level::MazeEntry nextActor = m_lvl->getContentsOf(i, j);
				switch(nextActor)
				{
				case Level::player:
					{
						m_avatar = new Player(i, j, this);
						m_maze[i][j] = IID_PLAYER;
						break;
					}
				case Level::simple_zumi:
					{
						m_actors.push_back(new SimpleZumi(i, j, this));
						m_maze[i][j] = IID_SIMPLE_ZUMI;
						break;
					}
				case Level::complex_zumi:
					{
						m_actors.push_back(new ComplexZumi(i, j, this));
						m_maze[i][j] = IID_COMPLEX_ZUMI;
						break;
					}
				case Level::perma_brick:
					{
						m_actors.push_back(new PermanentBrick(i, j, this));
						m_maze[i][j] = IID_PERMA_BRICK;
						break;
					}
				case Level::destroyable_brick:
					{
						m_actors.push_back(new DestroyableBrick(i, j, this));
						m_maze[i][j] = IID_DESTROYABLE_BRICK;
						break;
					}
				case Level::exit:
					{
						m_actors.push_back(new Exit(i, j, this));
						m_maze[i][j] = IID_EXIT;
						break;
					}
				default: // Location is empty.
					break;
				}
			}
		}
	}
	m_bonus = getLvl()->getOptionValue(optionLevelBonus);
	
	return GWSTATUS_CONTINUE_GAME;
}


int StudentWorld::move()
{
	// Update the game status line:
	setDisplayText();

	// Do something with the player:
	m_avatar->doSomething();
	if (m_bonus != 0)
		decrementBonus();

	// Do something with each actor:
	int n_Actors = m_actors.size();
	for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); )
	{
		if ((*it)->isDead() == false)
		{
			Actor* temp = *it;
			(*it)->doSomething();
			it = find(m_actors.begin(), m_actors.end(), temp);
			it++;
		}
		else
		{
			Actor* temp = *it;
			it = m_actors.erase(it);
			delete temp;
		}
		if (m_avatar->isDead())
		{
			decLives();
			return GWSTATUS_PLAYER_DIED;
		}
		if (allZumiDead())
		{
			getActor(IID_EXIT)->setVisible(true);
			playSound(SOUND_REVEAL_EXIT);
		}
		if (m_hasCompletedLevel)
			return GWSTATUS_FINISHED_LEVEL;
	}
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	delete m_avatar;
	
	int size = m_actors.size();
	for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); )
	{
		Actor* temp = *it;
		it = m_actors.erase(it);
		delete temp;
	}

	for (int i = 0; i < VIEW_HEIGHT; i++)
		for (int j = 0; j < VIEW_WIDTH; j++)
			m_maze[i][j] = -1; // Indicate empty
}

void StudentWorld::setDisplayText()
{
	int score = getScore();
	int level = getLevel();
	unsigned int bonus = getBonus();
	int livesLeft = getLives();

	ostringstream SCORE;
	if (score < 10)
		SCORE << "0000000" << score;
	else if (score >= 10 && score < 100)
		SCORE << "000000" << score;
	else if (score >= 100 && score < 1000)
		SCORE << "00000" << score;
	else if (score >=1000 && score < 10000)
		SCORE << "0000" << score;
	else if (score >= 10000 && score < 100000)
		SCORE << "000" << score;
	else if (score >= 100000 && score < 1000000)
		SCORE << "00" << score;
	else if (score >= 1000000 && score < 10000000)
		SCORE << "0" << score;
	else
		SCORE << score;
	ostringstream LEVEL;
	if (level < 10)
		LEVEL << "0" << level;
	else
		LEVEL << level;
	ostringstream LIVES;
	if (livesLeft < 10)
		LIVES << "00" << livesLeft;
	else if (livesLeft >= 10 && livesLeft < 100)
		LIVES << "0" << livesLeft;
	else
		LIVES << livesLeft;
	stringstream BONUS;
	if (bonus < 10)
		BONUS << "     " << bonus;
	else if (bonus >= 10 && bonus < 100)
		BONUS << "    " << bonus;
	else if (bonus >= 100 && bonus < 1000)
		BONUS << "   " << bonus;
	else if (bonus >= 1000 && bonus < 10000)
		BONUS << "  " << bonus;
	else if (bonus >= 10000 && bonus < 100000)
		BONUS << " " << bonus;
	else
		BONUS << bonus;

	string fullLine;
	fullLine += "Score: ";
	fullLine += SCORE.str();
	fullLine += "  Level: ";
	fullLine += LEVEL.str();
	fullLine += "  Lives: ";
	fullLine += LIVES.str();
	fullLine += "  Bonus: ";
	fullLine += BONUS.str();

	setGameStatText(fullLine);
}

int StudentWorld::getLevelData(int x, int y) const
{
	return m_maze[x][y];
}

void StudentWorld::setLevelData(int x, int y, int ID)
{
	m_maze[x][y] = ID;
}

Player* StudentWorld::getAvatar() const
{
	return m_avatar;
}

Actor* StudentWorld::getActor(int ID) const
{
	int size = m_actors.size();
	for (int i = 0; i < size; i++)
	{
		int ID_Actor = getLevelData(m_actors[i]->getX(), m_actors[i]->getY());
		if (ID_Actor == ID)
			return m_actors[i];
	}
	return nullptr; // Actor not found.
}

void StudentWorld::addActor(Actor* newActor)
{
	m_actors.push_back(newActor);
}

void StudentWorld::setActorAsDead(int x, int y)
{
	int size = m_actors.size();
	for (int i = 0; i < size; i++)
	{
		if (m_actors[i]->getX() == x && m_actors[i]->getY() == y)
		{
			m_actors[i]->setDead();
			break;
		}
	}
}

Level* StudentWorld::getLvl() const
{
	return m_lvl;
}

bool StudentWorld::allZumiDead() const // Problem: Exit appears as visible after killing 2 Zumis
{
	int size = m_actors.size();
	for (int i = 0; i < size; i++)
	{
		int ID_OfActor = getLevelData(m_actors[i]->getX(), m_actors[i]->getY());
		if (ID_OfActor == IID_SIMPLE_ZUMI || ID_OfActor == IID_COMPLEX_ZUMI)
			return false;
	}
	return true;
}

void StudentWorld::setLevelFinished()
{
	m_hasCompletedLevel = true;
}

unsigned int StudentWorld::getBonus() const
{
	return m_bonus;
}

void StudentWorld::decrementBonus()
{
	m_bonus--;
}