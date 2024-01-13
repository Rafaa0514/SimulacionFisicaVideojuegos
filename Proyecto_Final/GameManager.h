#pragma once
#include "objects/systems/PhysicsSystem.h"

extern bool renderItems;
extern vector<string> texts;

enum Positions { TITLE, SUBTITLE_1, SUBTITLE_2, UPPER_LEFT, UPPER_RIGHT, DOWN_LEFT };
enum State {MAIN_MENU, GAME, PAUSE_MENU, END_MENU};
enum Level { NONE, ONE, TWO, THREE };

const double MAX_MATCH_TIME = 180;

class GameManager {
protected:
	State currentState;
	Level currentLevel;
	PhysicsSystem* pSys;

	int objKilled;

	bool wait;
	bool pause;

	double cooldown;
	double countdown;
	double timePerLevel;


	void showMainMenu();
	void nextLevel();
	void showPauseMenu();
	void showEndMenu();

	// NIVELES
	void createLevelOne();
	void createLevelTwo();
	void createLevelThree();

	// Colisiones
	void checkCollisions(Layer a, Layer b);

	// Acciones del juego
	void shoot();
	void addFireworks();

	void clearTexts();

public:
	GameManager(PxPhysics* g, PxScene* scene);
	~GameManager();
	void update(double t);
	void handleEvents(char key);
};