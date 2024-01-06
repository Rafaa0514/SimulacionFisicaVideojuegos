#pragma once
#include "objects/systems/PhysicsSystem.h"

extern bool renderItems;
extern vector<string> texts;

enum Positions { TITLE, SUBTITLE_1, SUBTITLE_2, UPPER_LEFT, UPPER_RIGHT, DOWN_LEFT };
enum State {MAIN_MENU, GAME, PAUSE_MENU, END_MENU};
enum Level { NONE, ONE, TWO, THREE };

const double MAX_MATCH_TIME = 300;

class GameManager {
protected:
	State currentState;
	Level currentLevel;
	PhysicsSystem* pSys;
	bool win, pause;
	double matchTime;
	int objToKill;

	void showMainMenu();
	void nextLevel();
	void showPauseMenu();
	void showEndMenu();

public:
	GameManager(PxPhysics* g, PxScene* scene);
	~GameManager();
	void update(double t);
	void handleEvents(char key);
};