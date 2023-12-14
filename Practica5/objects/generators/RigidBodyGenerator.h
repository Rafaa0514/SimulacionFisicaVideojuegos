//#pragma once
//#include "ParticleGenerators/UniformParticleGenerator.h"
//#include "../physicObjects/RigidBodies.h"
//
//
//class RigidBodyGenerator : public UniformParticleGenerator {
//protected:
//	RigidBody* model;
//	PxPhysics* gPx;
//	PxScene* scene;
//
//public:
//	RigidBodyGenerator(PxPhysics* g, PxScene* s, string name, RigidBody* mod, Vector3 var_v, double prob, ParticleForceRegistry* pfr,
//		ForceGenerators& _fgs, bool up = true, Vector3 var_p = Vector3(0)) :
//		UniformParticleGenerator(name, mod, var_v, prob, pfr, _fgs, up, var_p), model(mod), gPx(g), scene(s) {
//		model_part->removeRender();
//	}
//
//	virtual list<Particle*> generateParticles() {
//		list<Particle*> newParts;
//
//		if (rndProb(rg) < generation_probability) {
//			RigidBody* newP = nullptr;
//			// Crear partícula
//			if (!uniquePoint) newP = model->clone(gPx, scene, calculatePos(), calculateVel());
//			else newP = model->clone(gPx, scene, model_part->getPos(), calculateVel());
//
//			newParts.push_back(newP);
//			pfr->addRegistry(fgs, newP);
//		}
//
//		return newParts;
//	}
//};