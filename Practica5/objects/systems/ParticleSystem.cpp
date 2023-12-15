#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(PxPhysics* g, PxScene* s) : fgs(), gPx(g), scene(s) {
	afr = new ActorForceRegistry();
	bb = new BoundingBox(Vector3(0), Vector3(200, 50, 200));
	currentModel = nullptr;
	fts = NONE;
	existGrav = false; existWind = false;
	floor = nullptr;

}

ParticleSystem::~ParticleSystem() {
	clear();
}

void ParticleSystem::update(double t) {
	if (fts == EXPL && !exploded) {
		timer += t;
		if (timer >= 3) { exploded = true; createExplosionForce(); }
	}

	updateForcesTime(t);

	// ----- ACTORES -----
	for (ActorGenerator* p : actorsGenerators) {
		if (myActors.size() < particlesLimit || particlesLimit < 0) {
			list<PhysicActor*> prtcls = p->generateActors();
			if (!prtcls.empty()) myActors.splice(myActors.end(), prtcls);
		}
	}

	for (auto it = myActors.begin(); it != myActors.end(); it++) {
		if (!(*it)->integrate(t)) {
			deadActors.push_back(it);
		}
	}

	for (int i = 0; i < deadActors.size(); i++) {
		afr->deleteActorRegistry(*deadActors[i]);
		delete* deadActors[i];
		myActors.erase(deadActors[i]);
	}
	deadActors.clear();

	#pragma region Fireworks
	// ----- FUEGOS ARTIFICIALES -----
	for (auto it = fireworks_pool.begin(); it != fireworks_pool.end(); it++) {
		if (!(*it)->integrate(t)) {
			deadFireworks.push_back(it);
		}
	}

	for (int i = 0; i < deadFireworks.size(); i++) {
		if ((*deadFireworks[i])->getGeneration() > 1) {
			list<Firework*> fs = fireworkGen->generateFireworks(*deadFireworks[i]);
			if (!fireworks_pool.empty()) fireworks_pool.splice(fireworks_pool.end(), fs);
		}
		afr->deleteActorRegistry(*deadFireworks[i]);
		delete* deadFireworks[i];
		fireworks_pool.erase(deadFireworks[i]);
	}
	deadFireworks.clear();
	#pragma endregion

	afr->updateForces();
}

ActorGenerator* ParticleSystem::getActorGenerator(string name) {
	auto it = actorsGenerators.begin();
	bool found = false;

	while (!found && it != actorsGenerators.end()) {
		found = (*it)->getName() == name;
		if (!found) it++;
	}

	return (*it);
}

void ParticleSystem::updateForcesTime(double t) {
	for (auto it = fgs.begin(); it != fgs.end();) {
		if (!(*it)->updateTime(t)) {
			afr->deleteForceRegistry(*it);
			delete* it;
			it = fgs.erase(it);
		}
		else it++;
	}
}

#pragma region Forces Practica3

void ParticleSystem::showGravityForce() {
	if (fts == GRAV) return;
	if (fts != NONE) clear();
	fts = GRAV;
	existGrav = true;
	particlesLimit = 500;
	bb = new BoundingBox(Vector3(0, 0, 0), Vector3(300, 100, 300));

	currentModel = new Particle(Vector3(-50, 0, -100), Vector3(0, 50, 0), RADIOUS, 2, colores[BLUE], 5.0f, bb);
	createActorGenerator(currentModel, Vector3(10), 0.7);

	fgs.push_back(new GravityGenerator(GLOBAL_GRAVITY, Vector3(0), Vector3(300)));
}

void ParticleSystem::showWindForce() {
	if (fts == WIND) return;
	if (fts != NONE) clear();

	fts = WIND;
	existGrav = true;
	existWind = true;
	particlesLimit = 250;
	bb = new BoundingBox(Vector3(-100, 200, 0), Vector3(150, 300, 150));

	currentModel = new Particle(Vector3(-100, 0, 0), Vector3(0, 50, 0), RADIOUS / 2, 4, colores[YELLOW], 12.0f, bb);
	createActorGenerator(currentModel, Vector3(10), 0.5);

	fgs.push_back(new WindGenerator(1, 0.1, Vector3(50, 0, 0), Vector3(-100, 100, 0), Vector3(200, 100, 200)));
	fgs.push_back(new GravityGenerator(GLOBAL_GRAVITY, Vector3(-100, 0, 0), Vector3(300)));
}

void ParticleSystem::showTornadoForce() {
	if (fts == TORN && !exploded) return;
	if (fts != NONE) clear();
	fts = TORN;
	existGrav = true;
	existWind = true;
	particlesLimit = -1;
	bb = new BoundingBox(Vector3(0), Vector3(500));

	currentModel = new Particle(Vector3(0), Vector3(0, 50, 0), RADIOUS, 5, colores[GREEN], 20, bb);
	createActorGenerator(currentModel, Vector3(20), 0.8);

	fgs.push_back(new TornadoGenerator(0.5, 1, 0.1, Vector3(0, 150, 0), Vector3(-20, 0, -20), Vector3(0), Vector3(200)));
	fgs.push_back(new GravityGenerator(GLOBAL_GRAVITY, Vector3(150), Vector3(200)));
}

void ParticleSystem::showExplosionForce() {
	if (fts == EXPL) return;
	if (fts != NONE) clear();

	fts = EXPL;
	particlesLimit = 500;
	timer = 0;
	exploded = false;
	bb = new BoundingBox(Vector3(0, 60, 0), Vector3(100));

	float rad = 7.5; float pi = 3.141516;

	for (int i = 0; i < particlesLimit; i++) {
		float theta = rand() % 360;
		float phi = rand() % 360;

		float x = rad * sin(theta * pi / 180.0f) * cos(phi * pi / 180.0f);
		float y = rad * sin(theta * pi / 180.0f) * sin(phi * pi / 180.0f);
		float z = rad * cos(theta * pi / 180.0f);

		Vector3 pos = Vector3(x, y + 60, z);

		myActors.push_back(new Particle(pos, Vector3(0), RADIOUS / 3, 3, colores[RED], 40, bb));
	}
}

void ParticleSystem::createExplosionForce() {
	fgs.push_back(new ExplosionGenerator(1000, 5000, 3, Vector3(0, 60, 0), 12));
	afr->addRegistry(fgs, myActors);
}

#pragma endregion

#pragma region Forces Practica4

void ParticleSystem::showSpringForce() {
	if (fts == SPRI) return;
	if (fts != NONE) clear();
	fts = SPRI;
	particlesLimit = -1;

	bb = new BoundingBox(Vector3(0, 0, 0), Vector3(300, 100, 300));
	list<PhysicActor*> parts;
	Particle* part1 = new Particle(Vector3(20, 0, 0), Vector3(0), 3, 1, colores[BLACK], 100, bb);
	Particle* part2 = new Particle(Vector3(-20, 0, 0), Vector3(0), 3, 1, colores[WHITE], 100, bb);
	parts.push_back(part1); parts.push_back(part2);

	float randK = (rand() % 10) + 1;
	SpringGenerator* sg1 = new SpringGenerator(randK, 30, part2);
	SpringGenerator* sg2 = new SpringGenerator(randK, 30, part1);
	fgs.push_back(sg1); fgs.push_back(sg2);

	myActors.splice(myActors.end(), parts);
	afr->addRegistry(sg1, part1);
	afr->addRegistry(sg2, part2);
}

void ParticleSystem::showAnchoredSpringForce() {
	if (fts == ANCH) return;
	if (fts != NONE) clear();
	fts = ANCH;
	particlesLimit = -1;

	bb = new BoundingBox(Vector3(0, 0, 0), Vector3(300, 100, 300));
	list<PhysicActor*> parts;
	Particle* part1 = new Particle(Vector3(-6, 0, 8), Vector3(0), 2, 0.5, colores[RED], 100, bb);
	Particle* part2 = new Particle(Vector3(1, 0, 0), Vector3(0), 2, 4, colores[BLUE], 100, bb);
	Particle* part3 = new Particle(Vector3(-5, 0, 0), Vector3(0), 2, 2, colores[GREEN], 100, bb);
	Particle* part4 = new Particle(Vector3(6, 0, -8), Vector3(0), 2, 1, colores[YELLOW], 100, bb);

	parts.push_back(part1); parts.push_back(part2); parts.push_back(part3); parts.push_back(part4);

	fgs.push_back(new AnchoredSpringGenerator((rand() % 10) + 1, 20, Vector3(0)));

	myActors.splice(myActors.end(), parts);
	afr->addRegistry(fgs, myActors);
}

void ParticleSystem::showElastic() {
	if (fts != NONE) clear();
	fts = ELAS;
	particlesLimit = -1;

	bb = new BoundingBox(Vector3(0, 0, 0), Vector3(300, 300, 300));
	list<PhysicActor*> parts;
	Particle* part1 = new Particle(Vector3(50, 0, 0), Vector3(0), 3, 1, colores[BLACK], -1, bb);
	Particle* part2 = new Particle(Vector3(-50, 0, 0), Vector3(0), 3, 1, colores[WHITE], -1, bb);
	parts.push_back(part1); parts.push_back(part2);

	float randK = (rand() % 10) + 1;
	SpringGenerator* sg1 = new SpringGenerator(randK, 90, part2, true);
	SpringGenerator* sg2 = new SpringGenerator(randK, 90, part1, true);
	fgs.push_back(sg1); fgs.push_back(sg2);

	myActors.splice(myActors.end(), parts);
	afr->addRegistry(sg1, part1);
	afr->addRegistry(sg2, part2);
}

void ParticleSystem::showBouyancyForce() {
	if (fts == BOUY) return;
	if (fts != NONE) clear();
	fts = BOUY;
	existGrav = true;
	particlesLimit = -1;

	bb = new BoundingBox(Vector3(0, 0, 0), Vector3(300, 100, 300));

	floutingBox = new BoxParticle(Vector3(0, 70, 0), Vector3(0), 1, 200, colores[RED], -1, bb);
	myActors.push_back(floutingBox);

	fgs.push_back(new BouyancyForceGenerator(2, 8, 1000, Vector3(0, 50, 0), Vector3(20, 100, 20)));
	fgs.push_back(new GravityGenerator(GLOBAL_GRAVITY, Vector3(0), Vector3(300)));

	afr->addRegistry(fgs, myActors);
}

void ParticleSystem::addGravity() {
	if (!existGrav) {
		GravityGenerator* gg = new GravityGenerator(GLOBAL_GRAVITY, Vector3(0), Vector3(500));
		fgs.push_back(gg);
		afr->addRegistry(gg, myActors);
		existGrav = true;
	}
}

void ParticleSystem::addWind() {
	if (!existWind) {
		WindGenerator* wg = new WindGenerator(1, 0.1, Vector3(10, 0, 0), Vector3(-20, 0, -20), Vector3(200, 100, 200));
		fgs.push_back(wg);
		afr->addRegistry(wg, myActors);
		existWind = true;
	}
}

void ParticleSystem::changeK(bool increase) {
	if (fts == SPRI || fts == ANCH || fts == ELAS) {
		for (auto it = fgs.begin(); it != fgs.end(); it++) {
			AnchoredSpringGenerator* cfg = static_cast<AnchoredSpringGenerator*>(*it);
			if (cfg != nullptr) {
				if (increase) cfg->setK(cfg->getK() + 1);
				else if(cfg->getK() > 1) cfg->setK(cfg->getK() - 1);
			}
		}
	}
}

void ParticleSystem::changeDimensions(bool increase) {
	if (fts == BOUY) {
		if (increase) floutingBox->changeDimensions(floutingBox->getRadious() + 0.5);
		else if (floutingBox->getRadious() > 0.5) floutingBox->changeDimensions(floutingBox->getRadious() - 0.5);
		cout << "\n- Nuevas dimensiones de la caja: " << floutingBox->getRadious() * 2;
	}
}

void ParticleSystem::changeMass(bool increase) {
	if (fts == BOUY) {
		if (increase) floutingBox->setMass(floutingBox->getMass() + 100);
		else if (floutingBox->getMass() > 100) floutingBox->setMass(floutingBox->getMass() - 100);
		cout << "\n- Nueva masa de la caja: " << floutingBox->getMass();
	}
}

#pragma endregion

#pragma region Forces Practica 5

void ParticleSystem::gravityRigid() {
	if (fts == GRAV) return;
	if (fts != NONE) clear();
	fts = GRAV;
	existGrav = true;
	particlesLimit = 1500;
	bb = new BoundingBox(Vector3(-50, 0, -100), Vector3(300, 400, 300));
	scene->setGravity(Vector3(0, -9.8, 0));
	floor = new RigidBody(gPx, scene, Vector3(-50, 0, -100), Vector3(150, 1, 150), Vector3(0), colores[BLACK], false, 30);

	currentModel = new RigidBody(gPx, scene, Vector3(-50, 100, -100), Vector3(1), Vector3(0, 50, 0), colores[BLUE], true, 2, bb, 7.5f);
	createActorGenerator(currentModel, Vector3(10), 0.7, false, Vector3(10));
}

void ParticleSystem::windRigid() {
	if (fts == WIND) return;
	if (fts != NONE) clear();

	fts = WIND;
	existGrav = true;
	existWind = true;
	particlesLimit = 250;
	bb = new BoundingBox(Vector3(-100, 200, 0), Vector3(150, 300, 150));
	scene->setGravity(Vector3(0, -9.8, 0));

	currentModel = new RigidBody(gPx, scene, Vector3(-100, 0, 0), Vector3(1), Vector3(0, 50, 0), colores[YELLOW], true, 4, bb);
	createActorGenerator(currentModel, Vector3(10), 0.5, false, Vector3(5));

	fgs.push_back(new WindGenerator(1, 0.1, Vector3(50, 0, 0), Vector3(-100, 100, 0), Vector3(200, 100, 200)));
}

void ParticleSystem::tornadoRigid() {
	if (fts == TORN && !exploded) return;
	if (fts != NONE) clear();
	fts = TORN;
	existGrav = true;
	existWind = true;
	particlesLimit = -1;
	bb = new BoundingBox(Vector3(0), Vector3(500));
	scene->setGravity(Vector3(0, -9.8, 0));

	currentModel = new RigidBody(gPx, scene, Vector3(0), Vector3(1), Vector3(0, 50, 0), colores[GREEN], true, 15, bb);
	createActorGenerator(currentModel, Vector3(20), 0.8, false, Vector3(15, 1, 15));

	fgs.push_back(new TornadoGenerator(0.5, 1, 0.1, Vector3(0, 150, 0), Vector3(-20, 0, -20), Vector3(0), Vector3(200)));
}

void ParticleSystem::explosionRigid() {
	if (fts == EXPL) return;
	if (fts != NONE) clear();

	fts = EXPL;
	particlesLimit = 150;
	timer = 0;
	exploded = false;
	bb = new BoundingBox(Vector3(0, 60, 0), Vector3(100));
	scene->setGravity(Vector3(0));

	float rad = 17.5; float pi = 3.141516;

	for (int i = 0; i < particlesLimit; i++) {
		float theta = rand() % 360;
		float phi = rand() % 360;

		float x = rad * sin(theta * pi / 180.0f) * cos(phi * pi / 180.0f);
		float y = rad * sin(theta * pi / 180.0f) * sin(phi * pi / 180.0f);
		float z = rad * cos(theta * pi / 180.0f);

		Vector3 pos = Vector3(x, y + 60, z);

		myActors.push_back(new RigidBody(gPx, scene, pos, Vector3(0.5), Vector3(0), colores[RED], true, 4, bb, 20));
	}
}

void ParticleSystem::anchoredSpringsRigid() {
	if (fts == ANCH) return;
	if (fts != NONE) clear();
	fts = ANCH;
	particlesLimit = -1;

	bb = new BoundingBox(Vector3(0, 0, 0), Vector3(300, 100, 300));
	scene->setGravity(Vector3(0));

	//list<PhysicActor*> parts;
	RigidBody* part1 = new RigidBody(gPx, scene, Vector3(-9, 0, 12), Vector3(1), Vector3(0), colores[RED], true, 3, bb);
	RigidBody* part2 = new RigidBody(gPx, scene, Vector3(7, 0, 0), Vector3(1), Vector3(0), colores[BLUE], true, 6, bb);
	RigidBody* part3 = new RigidBody(gPx, scene, Vector3(-10, 0, 0), Vector3(1), Vector3(0), colores[GREEN], true, 3, bb);
	RigidBody* part4 = new RigidBody(gPx, scene, Vector3(9, 0, -12), Vector3(1), Vector3(0), colores[YELLOW], true, 1.5, bb);

	myActors.push_back(part1); myActors.push_back(part2); myActors.push_back(part3); myActors.push_back(part4);
	int randK = (rand() % 10) + 1;
	fgs.push_back(new AnchoredSpringGenerator( randK, 20, Vector3(0)));

	//myActors.splice(myActors.end(), parts);
	afr->addRegistry(fgs, myActors);
}

#pragma endregion

void ParticleSystem::createActorGenerator(PhysicActor* model, Vector3 var_v, double prob, bool up, Vector3 var_p) {
	int type = rand() % 2;
	string name = "PartGenerator" + std::to_string(actorsGenerators.size() + 1);
	if (type % 2 == 0) {
		actorsGenerators.push_back(new UniformGenerator(name, gPx, scene, model, var_v, prob, afr, fgs, up, var_p));
	}
	else {
		actorsGenerators.push_back(new GaussianGenerator(name, gPx, scene, model, var_v, prob, afr, fgs, up, var_p));
	}
}

void ParticleSystem::clear() {
	fts = NONE;
	existGrav = false;
	existWind = false;
	floutingBox = nullptr;
	delete currentModel; currentModel = nullptr;
	delete bb; bb = nullptr;
	if (floor != nullptr) { delete floor; floor = nullptr; }

	scene->setGravity(Vector3(0));
	
	afr->deleteAllActorsRegistry(myActors);

	for (ActorGenerator* pg : actorsGenerators) delete pg;
	actorsGenerators.clear();

	for (PhysicActor* p : myActors) delete p;
	myActors.clear();

	for (ForceGenerator* fg : fgs) delete fg;
	fgs.clear();
}