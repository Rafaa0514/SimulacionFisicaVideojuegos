#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem(PxPhysics* g, PxScene* s) : fgs(), gPx(g), scene(s) {
	afr = new ActorForceRegistry();
	bb = new BoundingBox(Vector3(0), Vector3(200));
	currentModel = nullptr;
	floor = nullptr;
	particlesLimit = -1;
}

PhysicsSystem::~PhysicsSystem() {
	clear();
}

void PhysicsSystem::update(double t) {
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

ActorGenerator* PhysicsSystem::getActorGenerator(string name) {
	auto it = actorsGenerators.begin();
	bool found = false;

	while (!found && it != actorsGenerators.end()) {
		found = (*it)->getName() == name;
		if (!found) it++;
	}

	return (*it);
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

void PhysicsSystem::level1() {
	
}

void PhysicsSystem::level2() {
	
}

void PhysicsSystem::level3() {

}

void PhysicsSystem::createActorGenerator(PhysicActor* model, Vector3 var_v, double prob, bool up, Vector3 var_p) {
	int type = rand() % 2;
	string name = "PartGenerator" + std::to_string(actorsGenerators.size() + 1);
	if (type % 2 == 0) {
		actorsGenerators.push_back(new UniformGenerator(name, gPx, scene, model, var_v, prob, afr, fgs, up, var_p));
	}
	else {
		actorsGenerators.push_back(new GaussianGenerator(name, gPx, scene, model, var_v, prob, afr, fgs, up, var_p));
	}
}

void PhysicsSystem::clear() {
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