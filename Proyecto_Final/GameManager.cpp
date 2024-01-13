#include "GameManager.h"

GameManager::GameManager(PxPhysics* g, PxScene* scene) : cooldown(0), countdown(0) {
	pSys = new PhysicsSystem(g, scene);
	showMainMenu();
}

GameManager::~GameManager() {
	delete pSys;
}

void GameManager::showMainMenu() {
	currentState = MAIN_MENU;
	currentLevel = NONE;
	pause = false;
	timePerLevel = MAX_MATCH_TIME;
	objKilled = 0;

	texts[TITLE] = "[ AIM DOWN ]";
	texts[SUBTITLE_1] = "Presione SPACE para jugar";
	texts[SUBTITLE_2] = "Presione ESC para salir";
	texts[UPPER_LEFT] = "";
	texts[UPPER_RIGHT] = "";
	texts[DOWN_LEFT] = "Proyecto Final - Rafael Argandona";

	renderItems = false;
}

void GameManager::nextLevel() {
	currentState = GAME;
	renderItems = true;
	clearTexts();
	switch (currentLevel)
	{
		case NONE:	createLevelOne();	break;
		case ONE:	createLevelTwo();	break;
		case TWO:	createLevelThree(); break;
		case THREE: showEndMenu();		break;
	}
	wait = true;
	countdown = 3.5;
}

void GameManager::showPauseMenu() {
	currentState = PAUSE_MENU;

	texts[TITLE] = "[ PAUSA ]";
	texts[SUBTITLE_1] = "Presione SPACE para jugar";
	texts[SUBTITLE_2] = "Presione ESC para salir";
	texts[UPPER_LEFT] = "";
	texts[UPPER_RIGHT] = "";

	pause = true;
	renderItems = false;
}

void GameManager::showEndMenu() {
	pSys->clear();
	// Añadir fireworks

	currentState = END_MENU;
	currentLevel = NONE;

	texts[TITLE] = "[ FINAL ]";
	texts[SUBTITLE_1] = "¡ Se acabo el tiempo !";
	texts[SUBTITLE_2] = "Acertaste " + to_string(objKilled) + " objetivos";
	texts[DOWN_LEFT] = "Gracias por jugar";
	texts[UPPER_LEFT] = "";
	texts[UPPER_RIGHT] = "";

	renderItems = false;
}

void GameManager::createLevelOne() {
	currentLevel = ONE;
	texts[DOWN_LEFT] = "NIVEL 1";
	timePerLevel = 30;


	// Crear generador Particula 1 y generador de viento oeste
	Particle* model1 = new Particle(Vector3(45, 25, 100), Vector3(0, 0, 0), 2.5, 2, colores[YELLOW], -1, pSys->getBB());
	pSys->createActorGenerator(model1, Vector3(1, 20, 1), Vector3(0, 0, 0), 0.8, TARGET_1, false);
	
	WindGenerator* westWind = new WindGenerator(1, 0.1, Vector3(-25,0,0), Vector3(45, 25, 100), Vector3(20, 100, 20));
	pSys->addForceGenerator(westWind, TARGET_1);

	// Crear generador Particula 2 y generador de viento este
	Particle* model2 = new Particle(Vector3(-45, 25, 100), Vector3(0, 0, 0), 2.5, 2, colores[RED], -1, pSys->getBB());
	pSys->createActorGenerator(model2, Vector3(1, 20, 1), Vector3(0, 0, 0), 0.8, TARGET_2, false);

	WindGenerator* eastWind = new WindGenerator(1, 0.1, Vector3(25, 0, 0), Vector3(-45, 25, 100), Vector3(20, 100, 20));
	pSys->addForceGenerator(eastWind, TARGET_2);
}

void GameManager::createLevelTwo() {
	currentLevel = TWO;
	texts[DOWN_LEFT] = "NIVEL 2";
	timePerLevel = 45;
	pSys->clear();

	pSys->setGravity(Vector3(0, 0, 0));

	// Crear Particula y muelles

	// Crear Solidos y muelles
	
}

void GameManager::createLevelThree() {
	currentLevel = THREE;
	texts[DOWN_LEFT] = "NIVEL 3";
	timePerLevel = 60;

	// Crear Trampolin y solidos 


}

void GameManager::checkCollisions(Layer a, Layer b) {
	auto actorsA = pSys->getActorsFromLayer(a);

	auto actorsB = pSys->getActorsFromLayer(b);
	for (auto it1 = actorsA.begin(); it1 != actorsA.end(); it1++) {
		for (auto it2 = actorsB.begin(); it2 != actorsB.end() && !(*it1)->isDead(); it2++) {
			if ((*it1)->collides((*it2))) {
				(*it1)->kill();
				(*it2)->kill();
				objKilled++;
			}
		}
	}
}

void GameManager::shoot() {
	// Disparar
	
}

void GameManager::addFireworks() {

}

void GameManager::clearTexts() {
	for (int i = 0; i < texts.size(); i++) {
		texts[i] = "";
	}
}

void GameManager::update(double t) {
	if (currentState == GAME) {
		if (wait) {
			countdown -= t;
			texts[TITLE] = to_string((int)countdown);
			if (countdown <= 1) { wait = false; countdown = 0; texts[TITLE] = ""; }
		}
		else {
			// Update de los objetos y checkeo de colisiones
			pSys->update(t);
			checkCollisions(PROYECTILES, TARGET_1);
			checkCollisions(PROYECTILES, TARGET_2);

			// Escrbimos la info
			texts[UPPER_LEFT] = "Aciertos: " + to_string(objKilled);
			texts[UPPER_RIGHT] = "Tiempo: " + to_string((int)timePerLevel);

			// Si el tiempo del nivel x se ha acabado, se pasa al siguiente
			timePerLevel -= t;
			if (timePerLevel <= 0.0) nextLevel();
		}
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
				case MAIN_MENU: nextLevel(); break;

				case PAUSE_MENU: showPauseMenu(); break;

				case END_MENU: showMainMenu(); break;
			}
		}break;
	}
	if (key == 27) {
		switch (currentState)
		{
			case MAIN_MENU: exit(0); break;

			case GAME: showPauseMenu(); break;

			case PAUSE_MENU: {
				pSys->clear();
				showMainMenu();
			}break;

			case END_MENU: exit(0); break;
		}
	}
}