#include "GaussianGenerator.h"

GaussianGenerator::GaussianGenerator(string n, PxPhysics* g, PxScene* s, PhysicActor* model, float prob,
	ActorForceRegistry* r, ForceGenerators& _fgs, bool up, bool uv, Vector3 varP, Vector3 varV, Layer _l) :
	ActorGenerator(n, g, s, model, prob, up, uv, varP, varV, r, _fgs, _l) {
	// Distribuciones de velocidad
	if (!uv) assignVel();
	// Distribuciones de posición
	if (!up) assignPos();
}

GaussianGenerator::~GaussianGenerator() {
	delete velX; delete velY; delete velZ;
	if (!uniquePoint) { delete posX; delete posY; delete posZ; }
}

list<PhysicActor*> GaussianGenerator::generateActors() {
	list<PhysicActor*> newParts;
	
	if (rndProb(rg) < generation_probability) {
		PhysicActor* newP = nullptr;
		Vector3 v;
		if (uniqueVelocity) v = mean_vel;
		else v = calculateVel();
		// Crear partícula
		if (uniquePoint) newP = model_act->clone(gPx, scene, model_act->getPos(), v, model_act->getLT(), model_act->getBB());
		else newP = model_act->clone(gPx, scene, calculatePos(), v, model_act->getLT(), model_act->getBB());
		
		newParts.push_back(newP);
		afr->addRegistry(fgs, newP);
	}

	return newParts;
}

Vector3 GaussianGenerator::calculateVel() {
	return Vector3((*velX)(rg), (*velY)(rg), (*velZ)(rg));
}

Vector3 GaussianGenerator::calculatePos() {
	return Vector3((*posX)(rg), (*posY)(rg), (*posZ)(rg));
}

void GaussianGenerator::assignVel() {
	velX = new normal_distribution<float>(mean_vel.x, varVel.x);
	velY = new normal_distribution<float>(mean_vel.y, varVel.y);
	velZ = new normal_distribution<float>(mean_vel.z, varVel.z);
}


void GaussianGenerator::assignPos() {
	posX = new normal_distribution<float>(mean_pos.x, varPos.x);
	posY = new normal_distribution<float>(mean_pos.y, varPos.y);
	posZ = new normal_distribution<float>(mean_pos.z, varPos.z);
}