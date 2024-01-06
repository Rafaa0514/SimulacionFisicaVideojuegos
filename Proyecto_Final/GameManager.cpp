#include "GameManager.h"

GameManager::GameManager(PxPhysics* g, PxScene* scene) {
	pSys = new PhysicsSystem(g, scene);
	showMainMenu();
}

GameManager::~GameManager() {
	delete pSys;
}

void GameManager::showMainMenu() {
	currentState = MAIN_MENU;
	currentLevel = NONE;
	win = false;
	pause = false;
	matchTime = MAX_MATCH_TIME;

	texts[TITLE] = "[ AIM DOWN ]";
	texts[SUBTITLE_1] = "Presione SPACE para jugar";
	texts[SUBTITLE_2] = "Presione ESC para salir";
	texts[UPPER_LEFT] = "";
	texts[UPPER_RIGHT] = "";
	texts[DOWN_LEFT] = "Proyecto Final - Rafael Argandoña";

	renderItems = false;
}

void GameManager::nextLevel() {
	switch (currentLevel)
	{
		case NONE: {
			pSys->level1();
			objToKill = 5;
			currentLevel = ONE;
			texts[DOWN_LEFT] = "NIVEL 1";
		}
		break;

		case ONE: {
			pSys->level2();
			objToKill = 10;
			currentLevel = TWO;
			texts[DOWN_LEFT] = "NIVEL 2";
		}
		break;
		case TWO: {
			pSys->level3();
			objToKill = 20;
			currentLevel = THREE;
			texts[DOWN_LEFT] = "NIVEL 3";
		}
		break;
		case THREE: {
			pSys->clear();
			showEndMenu();
			currentLevel = NONE;
		}
		break;
	}
}

void GameManager::showPauseMenu() {
	currentState = PAUSE_MENU;

	texts[TITLE] = "[ PAUSA ]";
	texts[SUBTITLE_1] = "Presione SPACE para jugar";
	texts[SUBTITLE_2] = "Presione ESC para salir";
	texts[UPPER_LEFT] = "";
	texts[UPPER_RIGHT] = "";

	renderItems = false;
}

void GameManager::showEndMenu() {
	currentState = END_MENU;
	currentLevel = NONE;

	texts[TITLE] = "[ FINAL ]";
	if (win) {
		texts[SUBTITLE_1] = "Eliminaste los objetivos :(";
		texts[SUBTITLE_2] = "Tiempo restante : " + to_string(matchTime) + " seg";
		texts[DOWN_LEFT] = "VICTORIA";
	}
	else {
		texts[SUBTITLE_1] = "Se te acabo el tiempo :(";
		texts[SUBTITLE_2] = "Dejaste objetivos con vida";
		texts[DOWN_LEFT] = "DERROTA";
	}

	texts[UPPER_LEFT] = "";
	texts[UPPER_RIGHT] = "";

	renderItems = false;
}

void GameManager::update(double t) {
	if (currentState == GAME) {
		pSys->update(t);
	}
}

void GameManager::handleEvents(char key) {
	switch (key)
	{
		case 'S': {

		}break;

		case ' ': {
			switch (currentState)
			{
				case MAIN_MENU: {
					nextLevel();
				}
				break;
				case PAUSE_MENU:
					break;
				case END_MENU:
					break;
			}
		}break;
	}
	if (key == 27) {
		switch (currentState)
		{
			case MAIN_MENU: {
				exit(0);
			}break;

			case GAME: {
				pause = true;
				showPauseMenu();
			}break;

			case PAUSE_MENU: {
				pSys->clear();
				showMainMenu();
			}break;

			case END_MENU: {
				exit(0);
			}break;
		}
	}
}