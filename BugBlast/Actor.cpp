#include "Actor.h"
#include "StudentWorld.h"
#include "GameWorld.h"
#include "Level.h"
#include <cstdlib>
#include <cmath>

bool Actor::isDead() const
{
	return m_isDead;
}

StudentWorld* Actor::getWorld() const
{
	return m_world;
}

Player::Player(int startX, int startY, StudentWorld* world): Actor(IID_PLAYER, startX, startY, world),
	m_nBugSprayers(2), m_walkThroughBricks(false), m_maxBugSprayers(2)
{
	setVisible(true);
	m_timeWalkThroughBricks = getWorld()->getLvl()->getOptionValue(optionWalkThruLifetimeTicks);
	m_timeSprayerGoodies = getWorld()->getLvl()->getOptionValue(optionBoostedSprayerLifetimeTicks);
}

void Player::doSomething()
{
	int move;
	if (getWorld()->getKey(move))
	{
		switch(move)
		{
		case KEY_PRESS_LEFT:
			{
				if (getWorld()->getLevelData(getX() - 1, getY()) == IID_DESTROYABLE_BRICK && m_walkThroughBricks)
					moveTo(getX() - 1, getY());
				else if (getWorld()->getLevelData(getX() - 1, getY()) != IID_PERMA_BRICK && getWorld()->getLevelData(getX() - 1, getY()) != IID_DESTROYABLE_BRICK)
					moveTo(getX() - 1, getY());
				break;
			}
		case KEY_PRESS_RIGHT:
			{
				if (getWorld()->getLevelData(getX() + 1, getY()) == IID_DESTROYABLE_BRICK && m_walkThroughBricks)
					moveTo(getX() + 1, getY());
				else if (getWorld()->getLevelData(getX() + 1, getY()) != IID_PERMA_BRICK && getWorld()->getLevelData(getX() + 1, getY()) != IID_DESTROYABLE_BRICK)
					moveTo(getX() + 1, getY());
				break;
			}
		case KEY_PRESS_UP:
			{
				if (getWorld()->getLevelData(getX(), getY() + 1) == IID_DESTROYABLE_BRICK && m_walkThroughBricks)
					moveTo(getX(), getY() + 1);
				else if (getWorld()->getLevelData(getX(), getY() + 1) != IID_PERMA_BRICK && getWorld()->getLevelData(getX(), getY() + 1) != IID_DESTROYABLE_BRICK)
					moveTo(getX(), getY() + 1);
				break;
			}
		case KEY_PRESS_DOWN:
			{
				if (getWorld()->getLevelData(getX(), getY() - 1) == IID_DESTROYABLE_BRICK && m_walkThroughBricks)
					moveTo(getX(), getY() - 1);
				else if (getWorld()->getLevelData(getX(), getY() - 1) != IID_PERMA_BRICK && getWorld()->getLevelData(getX(), getY() - 1) != IID_DESTROYABLE_BRICK)
					moveTo(getX(), getY() - 1);
				break;
			}
		case KEY_PRESS_SPACE:
			{
				if(getWorld()->getLevelData(getX(), getY()) != IID_BUGSPRAYER && m_nBugSprayers != 0)
				{
					getWorld()->addActor(new BugSprayer(getX(), getY(), getWorld())); // Put bugsprayer at location.
					getWorld()->setLevelData(getX(), getY(), IID_BUGSPRAYER);
					m_nBugSprayers--;
				}
				break;
			}
		}
	}
	if (m_walkThroughBricks && m_timeWalkThroughBricks != 0)
		m_timeWalkThroughBricks--;
	if (m_walkThroughBricks && m_timeWalkThroughBricks == 0)
	{
		if (getWorld()->getLevelData(getX(), getY()) == IID_DESTROYABLE_BRICK)
			setDead();
		m_walkThroughBricks = false;
		m_timeWalkThroughBricks = getWorld()->getLvl()->getOptionValue(optionWalkThruLifetimeTicks);
	}

	if (m_timeSprayerGoodies != 0 && m_maxBugSprayers > 2)
		m_timeSprayerGoodies--;
	if (m_timeSprayerGoodies == 0 && m_maxBugSprayers > 2)
	{
		setMaxBugSprayers(2);
	}
}

void Player::addBugSprayers(int n)
{
	m_nBugSprayers += n;
}

void Player::walkThroughWalls(bool able)
{
	m_walkThroughBricks = able;
}

void Player::setMaxBugSprayers(int n)
{
	m_maxBugSprayers = n;
}

int Player::getNumberofBugSprayers() const
{
	return m_nBugSprayers;
}

int Player::getMaximumBugSprayers() const
{
	return m_maxBugSprayers;
}

Player::~Player()
{
	setVisible(false);
	getWorld()->playSound(SOUND_PLAYER_DIE);
}

Zumi::Zumi(int imageID, int startX, int startY, StudentWorld* world): Actor(imageID, startX, startY, world), m_dir(-1)
{
	setVisible(true);
	if (imageID == IID_COMPLEX_ZUMI)
		m_speed = getWorld()->getLvl()->getOptionValue(optionTicksPerComplexZumiMove);
	else
		m_speed = getWorld()->getLvl()->getOptionValue(optionTicksPerSimpleZumiMove);
}

int Zumi::getSpeed() const
{
	return m_speed;
}

void Zumi::setSleepTime(int amount)
{
	m_speed += amount;
}

int Zumi::getDirection() const
{
	return m_dir;
}

void Zumi::setDirection(int dir)
{
	m_dir = dir;
}

Zumi::~Zumi()
{
	setVisible(false);
	getWorld()->playSound(SOUND_ENEMY_DIE);
	getWorld()->setLevelData(getX(), getY(), -1);

	int goodieProb = getWorld()->getLvl()->getOptionValue(optionProbOfGoodieOverall);
	int prob = rand() % 99;
	if (prob < goodieProb)
	{
		int prob2 = rand() % 99; // Decide which goodie to drop.
		int prob_IncreaseBugSprayers = getWorld()->getLvl()->getOptionValue(optionProbOfMoreSprayersGoodie);
		int prob_WalkThroughWalls = getWorld()->getLvl()->getOptionValue(optionProbOfWalkThruGoodie);
		int prob_ExtraLife = getWorld()->getLvl()->getOptionValue(optionProbOfExtraLifeGoodie);
		if (prob2 < prob_IncreaseBugSprayers)
		{
			getWorld()->addActor(new IncreaseSimultaneousBugSprayer(getX(), getY(), getWorld()));
			getWorld()->setLevelData(getX(), getY(), IID_INCREASE_SIMULTANEOUS_SPRAYER_GOODIE);
		}
		else if (prob2 >= prob_IncreaseBugSprayers && prob2 < prob_WalkThroughWalls + prob_IncreaseBugSprayers)
		{
			getWorld()->addActor(new WalkThroughWallsGoodie(getX(), getY(), getWorld()));
			getWorld()->setLevelData(getX(), getY(), IID_WALK_THRU_GOODIE);
		}
		else
		{
			getWorld()->addActor(new ExtraLifeGoodie(getX(), getY(), getWorld()));
			getWorld()->setLevelData(getX(), getY(), IID_EXTRA_LIFE_GOODIE);
		}
	}
}


void SimpleZumi::doSomething()
{
	if (getX() == getWorld()->getAvatar()->getX() && getY() == getWorld()->getAvatar()->getY()) // Zumi is on the same square as the Player's avatar.
		getWorld()->getAvatar()->setDead();

	int sleepTime = getSpeed();
	if (sleepTime - 1 != 0)
	{
		setSleepTime(-1); // Decrement sleep time.
		return;
	}
	else
	{
		if (getDirection() == -1)
		{
			// Pick a random direction to move towards:
			int dir = rand() % 4; 
			setDirection(dir);
		}
		
		switch(getDirection())
		{
		case 0: // Move left.
			{
				if (getWorld()->getLevelData(getX() - 1, getY()) != IID_PERMA_BRICK && getWorld()->getLevelData(getX() - 1, getY()) != IID_DESTROYABLE_BRICK &&
					getWorld()->getLevelData(getX() - 1, getY()) != IID_BUGSPRAYER)
				{
					getWorld()->setLevelData(getX(), getY(), -1);
					getWorld()->setLevelData(getX() - 1, getY(), IID_SIMPLE_ZUMI);
					moveTo(getX() - 1, getY());
				}
				else
					setDirection(-1);
				break;
			
			}
		case 1: // Move up.
			{
				if (getWorld()->getLevelData(getX(), getY() + 1) != IID_PERMA_BRICK && getWorld()->getLevelData(getX(), getY() + 1) != IID_DESTROYABLE_BRICK &&
					getWorld()->getLevelData(getX(), getY() + 1) != IID_BUGSPRAYER)
				{
					getWorld()->setLevelData(getX(), getY(), -1);
					getWorld()->setLevelData(getX(), getY() + 1, IID_SIMPLE_ZUMI);
					moveTo(getX(), getY() + 1);
				}
				else
					setDirection(-1);
				break;
			}
		case 2: // Move right.
			{
				if (getWorld()->getLevelData(getX() + 1, getY()) != IID_PERMA_BRICK && getWorld()->getLevelData(getX() + 1, getY()) != IID_DESTROYABLE_BRICK &&
					getWorld()->getLevelData(getX() + 1, getY()) != IID_BUGSPRAYER)
				{
					getWorld()->setLevelData(getX(), getY(), -1);
					getWorld()->setLevelData(getX() + 1, getY(), IID_SIMPLE_ZUMI);
					moveTo(getX() + 1, getY());
				}
				else
					setDirection(-1);
				break;
			}
		case 3: // Move down.
			{
				if (getWorld()->getLevelData(getX(), getY() - 1) != IID_PERMA_BRICK && getWorld()->getLevelData(getX(), getY() - 1) != IID_DESTROYABLE_BRICK &&
					getWorld()->getLevelData(getX(), getY() - 1) != IID_BUGSPRAYER)
				{
					getWorld()->setLevelData(getX(), getY(), -1);
					getWorld()->setLevelData(getX(), getY() - 1, IID_SIMPLE_ZUMI);
					moveTo(getX(), getY() - 1);
				}
				else
					setDirection(-1);
				break;
			}
		}
		setSleepTime(getWorld()->getLvl()->getOptionValue(optionTicksPerSimpleZumiMove) - 1); // Reset sleep time.
	}
}

SimpleZumi::~SimpleZumi()
{
	getWorld()->increaseScore(100);
}

void ComplexZumi::doSomething()
{
	if (getX() == getWorld()->getAvatar()->getX() && getY() == getWorld()->getAvatar()->getY()) // Zumi is on the same square as the Player's avatar.
		getWorld()->getAvatar()->setDead();

	int sleepTime = getSpeed();
	if (sleepTime - 1 != 0)
	{
		setSleepTime(-1); // Decrement sleep time.
		return;
	}
	else
	{
		// Determine smell distance:
		int xDistanceFromPlayer = abs(getWorld()->getAvatar()->getX() - getX());
		int yDistanceFromPlayer = abs(getWorld()->getAvatar()->getY() - getY());
		int smellDistance = getWorld()->getLvl()->getOptionValue(optionComplexZumiSearchDistance);
		if (smellDistance >= xDistanceFromPlayer && smellDistance >= yDistanceFromPlayer) // Perform breadth-first search of route to the player.
		{
			Marked m[VIEW_HEIGHT][VIEW_WIDTH];
			for (int i = 0; i < VIEW_HEIGHT; i++)
				for (int j = 0; j < VIEW_WIDTH; j++)
				{
					m[i][j].visited = false;
					m[i][j].color = -1;
				}
			int curX = getX();
			int curY = getY();

			m_route.push(getWorld()->getLevelData(curX, curY)); // Push current location onto queue.
			m[curX][curY].visited = true; // Mark location as visited.
			m[curX - 1][curY].color = 0;
			m[curX][curY + 1].color = 1;
			m[curX + 1][curY].color = 2;
			m[curX][curY - 1].color = 3;
			while (m_route.empty())
			{
				int front = m_route.front();
				m_route.pop();
				if (front == IID_PLAYER) // There is a route to the player.
					break;
				if (getWorld()->getLevelData(curX - 1, curY) != IID_DESTROYABLE_BRICK && getWorld()->getLevelData(curX - 1, curY) != IID_PERMA_BRICK &&
					getWorld()->getLevelData(curX - 1, curY) != IID_BUGSPRAYER)
				{
					curX--;
					m[curX][curY].visited = true;
					if (m[curX][curY].color == -1)
						m[curX][curY].color = m[curX + 1][curY].color;
					m_route.push(getWorld()->getLevelData(curX, curY));
				}
				if (getWorld()->getLevelData(curX + 1, curY) != IID_DESTROYABLE_BRICK && getWorld()->getLevelData(curX + 1, curY) != IID_PERMA_BRICK &&
					getWorld()->getLevelData(curX + 1, curY) != IID_BUGSPRAYER)
				{
					curX++;
					m[curX][curY].visited = true;
					if (m[curX][curY].color == -1)
						m[curX][curY].color = m[curX - 1][curY].color;
					m_route.push(getWorld()->getLevelData(curX, curY));
				}
				if (getWorld()->getLevelData(curX , curY - 1) != IID_DESTROYABLE_BRICK && getWorld()->getLevelData(curX, curY - 1) != IID_PERMA_BRICK &&
					getWorld()->getLevelData(curX, curY - 1) != IID_BUGSPRAYER)
				{
					curY--;
					m[curX][curY].visited = true;
					if (m[curX][curY].color == -1)
						m[curX][curY].color = m[curX][curY + 1].color;
					m_route.push(getWorld()->getLevelData(curX, curY));
				}
				if (getWorld()->getLevelData(curX , curY + 1) != IID_DESTROYABLE_BRICK && getWorld()->getLevelData(curX, curY + 1) != IID_PERMA_BRICK &&
					getWorld()->getLevelData(curX, curY + 1) != IID_BUGSPRAYER)
				{
					curY++;
					m[curX][curY].visited = true;
					if (m[curX][curY].color == -1)
						m[curX][curY].color = m[curX][curY - 1].color;
					m_route.push(getWorld()->getLevelData(curX, curY));
				}
			}
			int colorOfPlayer = m[getWorld()->getAvatar()->getX()][getWorld()->getAvatar()->getY()].color;
			switch(colorOfPlayer)
			{
			case 0:
				{
					getWorld()->setLevelData(getX(), getY(), -1);
					getWorld()->setLevelData(getX() - 1, getY(), IID_COMPLEX_ZUMI);
					moveTo(getX() - 1, getY());
					break;
				}
			case 1:
				{
					getWorld()->setLevelData(getX(), getY(), -1);
					getWorld()->setLevelData(getX(), getY() + 1, IID_COMPLEX_ZUMI);
					moveTo(getX(), getY() + 1);
					break;
				}
			case 2:
				{
					getWorld()->setLevelData(getX(), getY(), -1);
					getWorld()->setLevelData(getX() + 1, getY(), IID_COMPLEX_ZUMI);
					moveTo(getX() + 1, getY());
					break;
				}
			case 3:
				{
					getWorld()->setLevelData(getX(), getY(), -1);
					getWorld()->setLevelData(getX(), getY() - 1, IID_COMPLEX_ZUMI);
					moveTo(getX(), getY() - 1);
					break;
				}
			default:
				{
					// Pick a random direction to move towards:
					int dir = rand() % 4; 
					setDirection(dir);
							
					switch(getDirection())
					{
					case 0: // Move left.
						{
							if (getWorld()->getLevelData(getX() - 1, getY()) != IID_PERMA_BRICK && getWorld()->getLevelData(getX() - 1, getY()) != IID_DESTROYABLE_BRICK &&
								getWorld()->getLevelData(getX() - 1, getY()) != IID_BUGSPRAYER)
							{
								getWorld()->setLevelData(getX(), getY(), -1);
								getWorld()->setLevelData(getX() - 1, getY(), IID_COMPLEX_ZUMI);
								moveTo(getX() - 1, getY());
							}
							else
								setDirection(-1);
							break;
						}
					case 1: // Move up.
						{
							if (getWorld()->getLevelData(getX(), getY() + 1) != IID_PERMA_BRICK && getWorld()->getLevelData(getX(), getY() + 1) != IID_DESTROYABLE_BRICK &&
								getWorld()->getLevelData(getX(), getY() + 1) != IID_BUGSPRAYER)
							{
								getWorld()->setLevelData(getX(), getY(), -1);
								getWorld()->setLevelData(getX(), getY() + 1, IID_COMPLEX_ZUMI);
								moveTo(getX(), getY() + 1);
							}
							else
								setDirection(-1);
							break;
						}
					case 2: // Move right.
						{
							if (getWorld()->getLevelData(getX() + 1, getY()) != IID_PERMA_BRICK && getWorld()->getLevelData(getX() + 1, getY()) != IID_DESTROYABLE_BRICK &&
								getWorld()->getLevelData(getX() + 1, getY()) != IID_BUGSPRAYER)
							{
								getWorld()->setLevelData(getX(), getY(), -1);
								getWorld()->setLevelData(getX() + 1, getY(), IID_COMPLEX_ZUMI);
								moveTo(getX() + 1, getY());
							}
							else
								setDirection(-1);
							break;
						}
					case 3: // Move down.
						{
							if (getWorld()->getLevelData(getX(), getY() - 1) != IID_PERMA_BRICK && getWorld()->getLevelData(getX(), getY() - 1) != IID_DESTROYABLE_BRICK &&
								getWorld()->getLevelData(getX(), getY() - 1) != IID_BUGSPRAYER)
							{
								getWorld()->setLevelData(getX(), getY(), -1);
								getWorld()->setLevelData(getX(), getY() - 1, IID_COMPLEX_ZUMI);
								moveTo(getX(), getY() - 1);
							}
							else
								setDirection(-1);
							break;
						}
					}
	
				}
			}
			setSleepTime(getWorld()->getLvl()->getOptionValue(optionTicksPerSimpleZumiMove) - 1); // Reset sleep time.
		}
		else // Not within smelling distance.
		{
			if (getDirection() == -1)
			{
				// Pick a random direction to move towards:
				int dir = rand() % 4; 
				setDirection(dir);
			}
		
			switch(getDirection())
			{
			case 0: // Move left.
				{
					if (getWorld()->getLevelData(getX() - 1, getY()) != IID_PERMA_BRICK && getWorld()->getLevelData(getX() - 1, getY()) != IID_DESTROYABLE_BRICK &&
						getWorld()->getLevelData(getX() - 1, getY()) != IID_BUGSPRAYER)
					{
						getWorld()->setLevelData(getX(), getY(), -1);
						getWorld()->setLevelData(getX() - 1, getY(), IID_COMPLEX_ZUMI);
						moveTo(getX() - 1, getY());
					}
					else
						setDirection(-1);
					break;
			
				}
			case 1: // Move up.
				{
					if (getWorld()->getLevelData(getX(), getY() + 1) != IID_PERMA_BRICK && getWorld()->getLevelData(getX(), getY() + 1) != IID_DESTROYABLE_BRICK &&
						getWorld()->getLevelData(getX(), getY() + 1) != IID_BUGSPRAYER)
					{
						getWorld()->setLevelData(getX(), getY(), -1);
						getWorld()->setLevelData(getX(), getY() + 1, IID_COMPLEX_ZUMI);
						moveTo(getX(), getY() + 1);
					}
					else
						setDirection(-1);
					break;
				}
			case 2: // Move right.
				{
					if (getWorld()->getLevelData(getX() + 1, getY()) != IID_PERMA_BRICK && getWorld()->getLevelData(getX() + 1, getY()) != IID_DESTROYABLE_BRICK &&
						getWorld()->getLevelData(getX() + 1, getY()) != IID_BUGSPRAYER)
					{
						getWorld()->setLevelData(getX(), getY(), -1);
						getWorld()->setLevelData(getX() + 1, getY(), IID_COMPLEX_ZUMI);
						moveTo(getX() + 1, getY());
					}
					else
						setDirection(-1);
					break;
				}
			case 3: // Move down.
				{
					if (getWorld()->getLevelData(getX(), getY() - 1) != IID_PERMA_BRICK && getWorld()->getLevelData(getX(), getY() - 1) != IID_DESTROYABLE_BRICK &&
						getWorld()->getLevelData(getX(), getY() - 1) != IID_BUGSPRAYER)
					{
						getWorld()->setLevelData(getX(), getY(), -1);
						getWorld()->setLevelData(getX(), getY() - 1, IID_COMPLEX_ZUMI);
						moveTo(getX(), getY() - 1);
					}
					else
						setDirection(-1);
					break;
				}
			}
			setSleepTime(getWorld()->getLvl()->getOptionValue(optionTicksPerComplexZumiMove) - 1); // Reset sleep time.
		}
	}
}

ComplexZumi::~ComplexZumi()
{
	getWorld()->increaseScore(500);
}

Brick::Brick(int imageID, int startX, int startY, StudentWorld* world): Actor(imageID, startX, startY, world)
{
	setVisible(true);
}

void Brick::doSomething()
{
	; // A brick does nothing!
}

DestroyableBrick::~DestroyableBrick()
{
	getWorld()->setLevelData(getX(), getY(), -1);
}

BugSprayer::BugSprayer(int startX, int startY, StudentWorld* world): Actor(IID_BUGSPRAYER, startX, startY, world), m_lifeTime(40)
{
	setVisible(true);
}

void BugSprayer::doSomething()
{
	if (isDead())
		return;
	m_lifeTime--;
	if (m_lifeTime == 0)
	{
		setVisible(false);

		// Put bugspray:
		getWorld()->addActor(new BugSpray(getX(), getY(), getWorld()));
		if (getWorld()->getLevelData(getX() + 1, getY()) != IID_PERMA_BRICK)
			getWorld()->addActor(new BugSpray(getX() + 1, getY(), getWorld()));
		if (getWorld()->getLevelData(getX() + 2, getY()) != IID_PERMA_BRICK)
			getWorld()->addActor(new BugSpray(getX() + 2, getY(), getWorld()));
		if (getWorld()->getLevelData(getX() - 1, getY()) != IID_PERMA_BRICK)
			getWorld()->addActor(new BugSpray(getX() - 1, getY(), getWorld()));
		if (getWorld()->getLevelData(getX() - 2, getY()) != IID_PERMA_BRICK)
			getWorld()->addActor(new BugSpray(getX() - 2, getY(), getWorld()));
		if (getWorld()->getLevelData(getX(), getY() + 1) != IID_PERMA_BRICK)
			getWorld()->addActor(new BugSpray(getX(), getY() + 1, getWorld()));
		if (getWorld()->getLevelData(getX(), getY() + 2) != IID_PERMA_BRICK)
			getWorld()->addActor(new BugSpray(getX(), getY() + 2, getWorld()));
		if (getWorld()->getLevelData(getX(), getY() - 1) != IID_PERMA_BRICK)
			getWorld()->addActor(new BugSpray(getX(), getY() - 1, getWorld()));
		if (getWorld()->getLevelData(getX(), getY() - 2) != IID_PERMA_BRICK)
			getWorld()->addActor(new BugSpray(getX(), getY() - 2, getWorld()));

		setDead();
	}
}

BugSprayer::~BugSprayer()
{
	getWorld()->playSound(SOUND_SPRAY);
	getWorld()->setLevelData(getX(), getY(), -1);
	int cur_nBugSprayers = getWorld()->getAvatar()->getNumberofBugSprayers();
	int max_nBugSprayers = getWorld()->getAvatar()->getMaximumBugSprayers();
	if (cur_nBugSprayers < max_nBugSprayers)
		getWorld()->getAvatar()->addBugSprayers(1);
}

BugSpray::BugSpray(int startX, int startY, StudentWorld* world): Actor(IID_BUGSPRAY, startX, startY, world), m_lifeTime(3)
{
	setVisible(true);
	int locationID = getWorld()->getLevelData(startX, startY);
	if (locationID == IID_DESTROYABLE_BRICK || locationID == IID_SIMPLE_ZUMI || locationID == IID_COMPLEX_ZUMI)
		getWorld()->setActorAsDead(getX(), getY()); // Destroy brick, simple Zumi, or complex Zumi.
	else if (startX == getWorld()->getAvatar()->getX() && startY == getWorld()->getAvatar()->getY())
		getWorld()->getAvatar()->setDead(); // Set the player as dead.
}

void BugSpray::doSomething()
{
	m_lifeTime--;
	if (m_lifeTime == 0)
	{
		setVisible(false);
		setDead();
	}
}

Goodie::Goodie(int imageID, int startX, int startY, StudentWorld* world): Actor(imageID, startX, startY, world)
{
	m_lifeTime = getWorld()->getLvl()->getOptionValue(optionGoodieLifetimeInTicks);
}

int Goodie::getLifeTime() const
{
	return m_lifeTime;
}

void Goodie::decrementLifeTime()
{
	m_lifeTime--;
}

Goodie::~Goodie()
{
	getWorld()->setLevelData(getX(), getY(), -1);
}

void ExtraLifeGoodie::doSomething()
{
	if (isDead())
		return;
	else
	{
		if (getLifeTime() == 0)
			setDead();
		setVisible(true);
		decrementLifeTime();
		if (getX() == getWorld()->getAvatar()->getX() && getY() == getWorld()->getAvatar()->getY())
		{
			getWorld()->incLives();
			getWorld()->increaseScore(1000);
			getWorld()->playSound(SOUND_GOT_GOODIE);
			setDead();
		}
	}
}

void WalkThroughWallsGoodie::doSomething()
{
	if (isDead())
		return;
	else
	{
		if (getLifeTime() == 0)
			setDead();
		setVisible(true);
		decrementLifeTime();
		if (getX() == getWorld()->getAvatar()->getX() && getY() == getWorld()->getAvatar()->getY())
		{
			getWorld()->getAvatar()->walkThroughWalls(true);
			getWorld()->increaseScore(1000);
			getWorld()->playSound(SOUND_GOT_GOODIE);
			setDead();
		}
	}
}

void IncreaseSimultaneousBugSprayer::doSomething()
{
	if (isDead())
		return;
	else
	{
		if (getLifeTime() == 0)
			setDead();
		setVisible(true);
		decrementLifeTime();
		if (getX() == getWorld()->getAvatar()->getX() && getY() == getWorld()->getAvatar()->getY())
		{
			getWorld()->getAvatar()->setMaxBugSprayers(getWorld()->getLvl()->getOptionValue(optionMaxBoostedSprayers));
			getWorld()->increaseScore(1000);
			getWorld()->playSound(SOUND_GOT_GOODIE);
			setDead();
		}
	}
}

Exit::Exit(int startX, int startY, StudentWorld* world): Actor(IID_EXIT, startX, startY, world)
{
	setVisible(false);
}

void Exit::doSomething()
{
	if (getWorld()->getAvatar()->getX() == getX() && getWorld()->getAvatar()->getY() == getY() &&isVisible())
	{
		getWorld()->increaseScore(getWorld()->getBonus());
		getWorld()->setLevelFinished();
		getWorld()->playSound(SOUND_FINISHED_LEVEL);
	}
}