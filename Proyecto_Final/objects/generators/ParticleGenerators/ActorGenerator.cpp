#include "ActorGenerator.h"

ActorGenerator::ActorGenerator(string n, PxPhysics* g, PxScene* s, PhysicActor* p, double prob, bool up, bool uv,
	Vector3 vp, Vector3 vv,	ActorForceRegistry* r, ForceGenerators& _fgs, Layer _l) :
		name(n), varVel(vv), generation_probability(prob), uniquePoint(up), uniqueVelocity(uv), rg(rd()), rndProb(0.0, 1.0), 
		varPos(vp),	layer(_l), afr(r), fgs(_fgs), gPx(g), scene(s) {
	setModel(p);
	srand((unsigned)time);
}

ActorGenerator::~ActorGenerator() {
	delete model_act;
}

void ActorGenerator::setModel(PhysicActor* model, bool modify_pos_vel) {
	delete model_act;
	model_act = model->clone(gPx, scene, model->getPos(), Vector3(0), model->getLT(), model->getBB());
	model_act->releaseRender();
	if (modify_pos_vel) {
		mean_pos = model->getPos();
		mean_vel = model->getVelocity();
	}
	delete model;
}