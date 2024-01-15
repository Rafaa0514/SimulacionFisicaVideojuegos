#include "GameManager.h"

GameManager::GameManager(PxPhysics* g, PxScene* scene) : cooldown(0), countdown(0), objKilled(0) {
	pSys = new PhysicsSystem(g, scene);
	showMainMenu();
	mainGravity = new GravityGenerator(Vector3(0, 7.5, 0), Vector3(0), Vector3(200));
}

GameManager::~GameManager() {
	delete pSys;
	delete mainGravity;
}

void GameManager::showMainMenu() {
	currentState = MAIN_MENU;
	currentLevel = NONE;
	pause = false;
	timePerLevel = 0;
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
	pSys->clear();
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
	wait = true;
	countdown = 10;
	currentState = END_MENU;
	currentLevel = NONE;

	texts[TITLE] = "[ FINAL ]";
	texts[SUBTITLE_1] = "¡ Se acabo el tiempo !";
	texts[SUBTITLE_2] = "Acertaste " + to_string(objKilled) + " objetivos";
	texts[DOWN_LEFT] = "Gracias por jugar";
	texts[UPPER_LEFT] = "Presione ESPACIO para ir a MENU PRINCIPAL";
	texts[UPPER_RIGHT] = "";

	// Añadir fireworks
	addFireworks();
}

void GameManager::createLevelOne() {
	currentLevel = ONE;
	texts[DOWN_LEFT] = "NIVEL 1";
	timePerLevel = 30;

	pSys->setObjectsLimit(10);
	pSys->setBB(CENTER_POSITION, Vector3(100));

	// Crear generador Particula 1 y generador de viento oeste
	Particle* model1 = new Particle(Vector3(45, 25, 80), Vector3(0, 0, 0), 2.5, 2, colores[YELLOW], 5, pSys->getBB());
	pSys->createActorGenerator(model1, Vector3(1, 20, 1), Vector3(0, 0, 0), 0.8, TARGET_1, false);
	
	WindGenerator* westWind = new WindGenerator(1, 0.1, Vector3(-25,0,0), Vector3(45, 25, 80), Vector3(20, 100, 20));
	pSys->addForceGenerator(westWind, TARGET_1);

	// Crear generador Particula 2 y generador de viento este
	Particle* model2 = new Particle(Vector3(-45, 25, 80), Vector3(0, 0, 0), 2.5, 2, colores[RED], 5, pSys->getBB());
	pSys->createActorGenerator(model2, Vector3(1, 20, 1), Vector3(0, 0, 0), 0.8, TARGET_2, false);

	WindGenerator* eastWind = new WindGenerator(1, 0.1, Vector3(25, 0, 0), Vector3(-45, 25, 80), Vector3(20, 100, 20));
	pSys->addForceGenerator(eastWind, TARGET_2);
}

void GameManager::createLevelTwo() {
	currentLevel = TWO;
	texts[DOWN_LEFT] = "NIVEL 2";
	timePerLevel = 45;
	pSys->setObjectsLimit(40);
	pSys->setBB(CENTER_POSITION, Vector3(200));

	Vector3 posPartR = Vector3(40, 10, 60);
	Vector3 posPartL = Vector3(-40, 10, 60);

	// Crear Particula y muelles
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 5; i++) {
			Particle* der = new Particle(posPartR, Vector3(0), 1.5, 1.25, colores[YELLOW], -1, pSys->getBB());
			Particle* izq = new Particle(posPartL, Vector3(0), 1.5, 1.25, colores[RED], -1, pSys->getBB());

			AnchoredSpringGenerator* muelleDer = new AnchoredSpringGenerator((rand() % 10) + 1, 20, posPartR + Vector3(10, 0, 0));
			AnchoredSpringGenerator* muelleIzq = new AnchoredSpringGenerator((rand() % 10) + 1, 20, posPartL - Vector3(10, 0, 0));

			pSys->addForceAndActor(muelleDer, der, TARGET_1); 
			pSys->addForceAndActor(muelleIzq, izq, TARGET_2);

			posPartR.y += 4;
			posPartL.y += 4;
		}
		posPartR.y = 10;
		posPartL.y = 10;
		posPartR.z += 15;
		posPartL.z += 15;
	}
}

void GameManager::createLevelThree() {
	currentLevel = THREE;
	texts[DOWN_LEFT] = "NIVEL 3";
	timePerLevel = 60;

	pSys->setGravity(Vector3(0, -9.8, 0));
	pSys->setBB(CENTER_POSITION, Vector3(70, 60, 70));
	pSys->setObjectsLimit(25);

	// Crear Trampolin y solidos 
	RigidBody* caja = 
		new RigidBody(pSys->gPx, pSys->scene, CENTER_POSITION, Vector3(1, 1, 1), Vector3(0), colores[RED], true, 2, pSys->getBB(), 12);

	pSys->createActorGenerator(caja, Vector3(15, 1, 15), Vector3(0), 0.7, TARGET_1, false);

	Vector3 centroTrampolin = CENTER_POSITION - Vector3(0, 25, 0);
	BouyancyForceGenerator* bfg = new BouyancyForceGenerator(2, 1000, centroTrampolin, Vector3(30, 300, 30));
	pSys->addForceGenerator(bfg, TARGET_1);
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
	Vector3 posP = GetCamera()->getTransform().p + GetCamera()->getDir() * 10;
	Particle* proyectile = new Particle(posP, GetCamera()->getDir() * 75, 1.5, 1.5, colores[WHITE], 1.5, pSys->getBB());
	pSys->addForceAndActor(mainGravity, proyectile, PROYECTILES, false);
}

void GameManager::addFireworks() {
	pSys->setBB(CENTER_POSITION, Vector3(900));
	pSys->setObjectsLimit(-1);
	renderItems = true;

	Vector3 pos = Vector3(30, -80, 900);
	Vector3 vel = Vector3(-5, 20, -100);

	pSys->addForceGenerator(new GravityGenerator(Vector3(0, -9.8, 0), CENTER_POSITION, Vector3(200)));

	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 3; i++) {
			Firework* fire = new Firework(pos, vel, 2.5, colores[rand() % 6], 3.5, pSys->getBB(), 4, 5);
			pSys->addFirework(fire);
			pos.x -= 30;
		}
		pos.x = 30;
		pos.y += 30;
	}
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
	else if (currentState == END_MENU) {
		countdown -= t;
		if (countdown <= 1) { countdown = 0; wait = false; }
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
				case MAIN_MENU: nextLevel(); break;

				case PAUSE_MENU: {
					pause = false;
					renderItems = true;
					currentState = GAME;
					clearTexts();
				} break;

				case END_MENU: if (!wait) showMainMenu(); break;

				case GAME: if (!wait) shoot(); break;
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

			case END_MENU: if (!wait) exit(0); break;
		}
	}
}