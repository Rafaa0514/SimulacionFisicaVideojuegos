#include "GaussianGenerator.h"

GaussianGenerator::GaussianGenerator(string n, PxPhysics* g, PxScene* s, PhysicActor* model, Vector3 var, float prob,
	ActorForceRegistry* r, ForceGenerators& _fgs, bool up, Vector3 vp) :
	ActorGenerator(n, g, s, model, var, prob, up, vp, r, _fgs) {
	// Distribuciones de velocidad
	assignVel();
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
		// Crear partícula
		if (uniquePoint) newP = model_act->clone(gPx, scene, model_act->getPos(), calculateVel(), model_act->getLT(), model_act->getBB());
		else newP = model_act->clone(gPx, scene, calculatePos(), calculateVel(), model_act->getLT(), model_act->getBB());
		
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
	velX = new normal_distribution<float>(mean_vel.x, meanVar.x);
	velY = new normal_distribution<float>(mean_vel.y, meanVar.y);
	velZ = new normal_distribution<float>(mean_vel.z, meanVar.z);
}


void GaussianGenerator::assignPos() {
	posX = new normal_distribution<float>(mean_pos.x, varPos.x);
	posY = new normal_distribution<float>(mean_pos.y, varPos.y);
	posZ = new normal_distribution<float>(mean_pos.z, varPos.z);
}