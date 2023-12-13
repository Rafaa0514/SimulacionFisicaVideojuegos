#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(PxPhysics* g, PxScene* s) : fgs(), gPx(g), scene(s) {
	pfr = new ParticleForceRegistry();
	bb = new BoundingBox(Vector3(0), Vector3(0));
	currentModel = nullptr;
	fts = NONE;
	existGrav = false; existWind = false;

	RigidBody* caja = new RigidBody(g, s, Vector3(0), Vector3(0), true, Vector3(5), colores[WHITE], 2, 0.1);

	RigidBodyGenerator* rbg = new RigidBodyGenerator(g, s, "MESSI", caja, Vector3(10), 0.7, pfr, fgs);
	particleGenerators.push_back(rbg);
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

	// ----- PARTÍCULAS -----
	for (ParticleGenerator* p : particleGenerators) {
		if (myParticles.size() < particlesLimit || particlesLimit < 0) {
			list<Particle*> prtcls = p->generateParticles();
			if (!prtcls.empty()) myParticles.splice(myParticles.end(), prtcls);
		}
	}

	for (auto it = myParticles.begin(); it != myParticles.end(); it++) {
		if (!(*it)->integrate(t)) {
			deadParticles.push_back(it);
		}
	}

	for (int i = 0; i < deadParticles.size(); i++) {
		pfr->deleteParticleRegistry(*deadParticles[i]);
		delete* deadParticles[i];
		myParticles.erase(deadParticles[i]);
	}
	deadParticles.clear();

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
		pfr->deleteParticleRegistry(*deadFireworks[i]);
		delete* deadFireworks[i];
		fireworks_pool.erase(deadFireworks[i]);
	}
	deadFireworks.clear();


	pfr->updateForces();
}

ParticleGenerator* ParticleSystem::getParticleGenerator(string name) {
	auto it = particleGenerators.begin();
	bool found = false;

	while (!found && it != particleGenerators.end()) {
		found = (*it)->getName() == name;
		if (!found) it++;
	}

	return (*it);
}

void ParticleSystem::updateForcesTime(double t) {
	for (auto it = fgs.begin(); it != fgs.end();) {
		if (!(*it)->updateTime(t)) {
			pfr->deleteForceRegistry(*it);
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
	createParticleGenerator(currentModel, Vector3(10), 0.7);

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
	createParticleGenerator(currentModel, Vector3(10), 0.5);

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
	bb = new BoundingBox(Vector3(0), Vector3(400));

	currentModel = new Particle(Vector3(0), Vector3(0, 50, 0), RADIOUS, 5, colores[GREEN], 20, bb);
	createParticleGenerator(currentModel, Vector3(20), 0.8);

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

		myParticles.push_back(new Particle(pos, Vector3(0), RADIOUS / 3, 3, colores[RED], 40, bb));
	}
}

void ParticleSystem::createExplosionForce() {
	fgs.push_back(new ExplosionGenerator(1000, 5000, 3, Vector3(0, 60, 0), 12));
	pfr->addRegistry(fgs, myParticles);
}

#pragma endregion

#pragma region Forces Practica4

void ParticleSystem::showSpringForce() {
	if (fts == SPRI) return;
	if (fts != NONE) clear();
	fts = SPRI;
	particlesLimit = -1;

	bb = new BoundingBox(Vector3(0, 0, 0), Vector3(300, 100, 300));
	list<Particle*> parts;
	Particle* part1 = new Particle(Vector3(20, 0, 0), Vector3(0), 3, 1, colores[BLACK], 100, bb);
	Particle* part2 = new Particle(Vector3(-20, 0, 0), Vector3(0), 3, 1, colores[WHITE], 100, bb);
	parts.push_back(part1); parts.push_back(part2);

	float randK = (rand() % 10) + 1;
	SpringGenerator* sg1 = new SpringGenerator(randK, 30, part2);
	SpringGenerator* sg2 = new SpringGenerator(randK, 30, part1);
	fgs.push_back(sg1); fgs.push_back(sg2);

	myParticles.splice(myParticles.end(), parts);
	pfr->addRegistry(sg1, part1);
	pfr->addRegistry(sg2, part2);
}

void ParticleSystem::showAnchoredSpringForce() {
	if (fts == ANCH) return;
	if (fts != NONE) clear();
	fts = ANCH;
	particlesLimit = -1;

	bb = new BoundingBox(Vector3(0, 0, 0), Vector3(300, 100, 300));

	list<Particle*> parts;
	Particle* part1 = new Particle(Vector3(-6, 0, 8), Vector3(0), 2, 0.5, colores[RED], 100, bb);
	Particle* part2 = new Particle(Vector3(1, 0, 0), Vector3(0), 2, 4, colores[BLUE], 100, bb);
	Particle* part3 = new Particle(Vector3(-5, 0, 0), Vector3(0), 2, 2, colores[GREEN], 100, bb);
	Particle* part4 = new Particle(Vector3(6, 0, -8), Vector3(0), 2, 1, colores[YELLOW], 100, bb);

	parts.push_back(part1); parts.push_back(part2); parts.push_back(part3); parts.push_back(part4);

	fgs.push_back(new AnchoredSpringGenerator((rand() % 10) + 1, 20, Vector3(0)));

	myParticles.splice(myParticles.end(), parts);
	pfr->addRegistry(fgs, myParticles);
}

void ParticleSystem::showElastic() {
	if (fts != NONE) clear();
	fts = ELAS;
	particlesLimit = -1;

	bb = new BoundingBox(Vector3(0, 0, 0), Vector3(300, 300, 300));
	list<Particle*> parts;
	Particle* part1 = new Particle(Vector3(50, 0, 0), Vector3(0), 3, 1, colores[BLACK], -1, bb);
	Particle* part2 = new Particle(Vector3(-50, 0, 0), Vector3(0), 3, 1, colores[WHITE], -1, bb);
	parts.push_back(part1); parts.push_back(part2);

	float randK = (rand() % 10) + 1;
	SpringGenerator* sg1 = new SpringGenerator(randK, 90, part2, true);
	SpringGenerator* sg2 = new SpringGenerator(randK, 90, part1, true);
	fgs.push_back(sg1); fgs.push_back(sg2);

	myParticles.splice(myParticles.end(), parts);
	pfr->addRegistry(sg1, part1);
	pfr->addRegistry(sg2, part2);
}

void ParticleSystem::showBouyancyForce() {
	if (fts == BOUY) return;
	if (fts != NONE) clear();
	fts = BOUY;
	existGrav = true;
	particlesLimit = -1;

	bb = new BoundingBox(Vector3(0, 0, 0), Vector3(300, 100, 300));

	floutingBox = new BoxParticle(Vector3(0, 70, 0), Vector3(0), 1, 200, colores[RED], -1, bb);
	myParticles.push_back(floutingBox);

	fgs.push_back(new BouyancyForceGenerator(2, 8, 1000, Vector3(0, 50, 0), Vector3(20, 100, 20)));
	fgs.push_back(new GravityGenerator(GLOBAL_GRAVITY, Vector3(0), Vector3(300)));

	pfr->addRegistry(fgs, myParticles);
}

void ParticleSystem::addGravity() {
	if (!existGrav) {
		GravityGenerator* gg = new GravityGenerator(GLOBAL_GRAVITY, Vector3(0), Vector3(500));
		fgs.push_back(gg);
		pfr->addRegistry(gg, myParticles);
		existGrav = true;
	}
}

void ParticleSystem::addWind() {
	if (!existWind) {
		WindGenerator* wg = new WindGenerator(1, 0.1, Vector3(10, 0, 0), Vector3(-20, 0, -20), Vector3(200, 100, 200));
		fgs.push_back(wg);
		pfr->addRegistry(wg, myParticles);
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

void ParticleSystem::createParticleGenerator(Particle* model, Vector3 var_v, double prob, bool up, Vector3 var_p) {
	int type = rand() % 2;
	string name = "PartGenerator" + std::to_string(particleGenerators.size() + 1);
	if (type % 2 == 0) {
		particleGenerators.push_back(new UniformParticleGenerator(name, model, var_v, prob, pfr, fgs, up, var_p));
	}
	else {
		particleGenerators.push_back(new GaussianParticleGenerator(name, model, var_v, prob, pfr, fgs, up, var_p));
	}
}

void ParticleSystem::clear() {
	fts = NONE;
	existGrav = false;
	existWind = false;
	floutingBox = nullptr;
	delete currentModel; currentModel = nullptr;
	delete bb; bb = nullptr;
	
	pfr->deleteAllParticlesRegistry(myParticles);

	for (ParticleGenerator* pg : particleGenerators) delete pg;
	particleGenerators.clear();

	for (Particle* p : myParticles) delete p;
	myParticles.clear();

	for (ForceGenerator* fg : fgs) delete fg;
	fgs.clear();
}