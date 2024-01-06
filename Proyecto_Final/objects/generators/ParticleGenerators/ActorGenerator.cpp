#include "ActorGenerator.h"

ActorGenerator::ActorGenerator(string n, PxPhysics* g, PxScene* s, PhysicActor* p, Vector3 var, double prob, bool up, Vector3 vp,
	ActorForceRegistry* r, ForceGenerators& _fgs) :
		name(n), meanVar(var), generation_probability(prob), uniquePoint(up), rg(rd()), rndProb(0.0, 1.0), varPos(vp),
			afr(r), fgs(_fgs), gPx(g), scene(s) {
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
		mean_pos = model_act->getPos();
		mean_vel = model_act->getVelocity();
	}
}