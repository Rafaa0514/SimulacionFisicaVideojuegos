#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem(PxPhysics* g, PxScene* s) : fgs(), gPx(g), scene(s) {
	afr = new ActorForceRegistry();
	bb = new BoundingBox(CENTER_POSITION, Vector3(50));
	currentModel = nullptr;
	floor = nullptr;
	objectsLimit = 10;
	fireworkGen = new FireworkGenerator("Fuego valirio", afr, fgs, Vector3(20, 30, 20));
}

PhysicsSystem::~PhysicsSystem() {
	clear();
}

void PhysicsSystem::update(double t) {
	updateForcesTime(t);

	// ----- ACTORES -----
	for (ActorGenerator* p : actorsGenerators) {
		Layer l = p->getLayer();
		if (myActors[l].size() < objectsLimit || objectsLimit < 0) {
			list<PhysicActor*> prtcls = p->generateActors();
			if (!prtcls.empty()) {
				myActors[l].splice(myActors[l].end(), prtcls);
			}
		}
	}

	for (int i = 0; i < NUM_LAYERS; i++) {
		for (auto it = myActors[i].begin(); it != myActors[i].end(); it++) {
			if (!(*it)->integrate(t)) {
				deadActors[i].push_back(it);
			}
		}
	}

	for (int i = 0; i < NUM_LAYERS; i++) {
		for (int j = 0; j < deadActors[i].size(); j++) {
			afr->deleteActorRegistry(*deadActors[i][j]);
			delete* deadActors[i][j];
			myActors[i].erase(deadActors[i][j]);
		}
		deadActors[i].clear();
	}

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

ActorGenerator* PhysicsSystem::getActorGenerator(string name) {
	auto it = actorsGenerators.begin();
	bool found = false;

	while (!found && it != actorsGenerators.end()) {
		found = (*it)->getName() == name;
		if (!found) it++;
	}

	return (*it);
}

void PhysicsSystem::addForceAndActor(ForceGenerator* fg, PhysicActor* a, Layer l, bool addFG) {
	if (addFG) fgs.push_back(fg);
	myActors[l].push_back(a);
	afr->addRegistry(fg, a); 
}

void PhysicsSystem::addFirework(Firework* f) {
	fireworks_pool.push_back(f);
	afr->addRegistry(fgs, f);
}

void PhysicsSystem::updateForcesTime(double t) {
	for (auto it = fgs.begin(); it != fgs.end();) {
		if (!(*it)->updateTime(t)) {
			afr->deleteForceRegistry(*it);
			delete* it;
			it = fgs.erase(it);
		}
		else it++;
	}
}

void PhysicsSystem::createActorGenerator(PhysicActor* model, Vector3 var_p, Vector3 var_v, double prob, Layer l, bool up, bool uv) {
	int type = rand() % 2;
	string name = "PartGenerator" + std::to_string(actorsGenerators.size() + 1);
	if (type % 2 == 0) {
		actorsGenerators.push_back(new UniformGenerator(name, gPx, scene, model, prob, afr, fgs, up, uv, var_p, var_v, l));
	}
	else {
		actorsGenerators.push_back(new GaussianGenerator(name, gPx, scene, model, prob, afr, fgs, up, uv, var_p, var_v, l));
	}
}

void PhysicsSystem::clear() {
	floutingBox = nullptr;
	delete currentModel; currentModel = nullptr;
	delete bb; bb = nullptr;
	if (floor != nullptr) { delete floor; floor = nullptr; }

	scene->setGravity(Vector3(0));
	
	for (int i = 0; i < NUM_LAYERS; i++) {
		afr->deleteAllActorsRegistry(myActors[i]);
		for (PhysicActor* p : myActors[i]) delete p;
		myActors[i].clear();
	}

	for (ActorGenerator* pg : actorsGenerators) delete pg;
	actorsGenerators.clear();

	for (ForceGenerator* fg : fgs) delete fg;
	fgs.clear();
}