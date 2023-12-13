#pragma once
#include "Particle.h"

using namespace physx;

class RigidBody : public Particle {
protected:
	PxRigidStatic* bStatic = nullptr;
	PxRigidDynamic* bDynamic = nullptr;
	bool movable;
	PxShape* shape;
	Vector3 scale;

public:
	RigidBody(PxPhysics* gPx, PxScene* scene, Vector3 pos, Vector3 v, bool mov, Vector3 s, Vector4 color,
		double mass, double lt = -1, BoundingBox* bb = nullptr) : Particle(pos, v, 1, mass, color, lt, bb), scale(s), movable(mov) {
		shape = CreateShape(PxBoxGeometry(scale));
		renderItem->release();

		if (movable) {
			bDynamic = gPx->createRigidDynamic(pose);
			bDynamic->attachShape(*shape);
			renderItem = new RenderItem(shape, bDynamic, color);
			scene->addActor(*bDynamic);
		}
		else {
			bStatic = gPx->createRigidStatic(pose);
			bStatic->attachShape(*shape);
			renderItem = new RenderItem(shape, bStatic, color);
			scene->addActor(*bStatic);
		}
	}

	RigidBody* clone(PxPhysics* gPx, PxScene* scene) {
		return new RigidBody(gPx, scene, pose.p, vel, movable, scale, col, mass, lifeTime, bb);
	}

	RigidBody* clone(PxPhysics* gPx, PxScene* scene, Vector3 pos, Vector3 v) {
		return new RigidBody(gPx, scene, pos, v, movable, scale, col, mass, lifeTime, bb);
	}

	virtual bool integrate(double t) {
		lifeTime -= t;
		if (lifeTime <= 0.0) return false;
		return true;
	}
};

class RigidDynamic : public Particle{
protected:
	PxRigidDynamic* body;
	PxShape* shape;
	Vector3 scale;

public:
	RigidDynamic(PxPhysics* gPx, PxScene* scene, Vector3 pos, Vector3 v, Vector3 s, Vector4 color,
		double mass, double lt = -1, BoundingBox* bb = nullptr) : Particle(pos, v, 1, mass, color, lt, bb), scale(s) {
		shape = CreateShape(PxBoxGeometry(scale));
		body = gPx->createRigidDynamic(pose);
		body->attachShape(*shape);

		renderItem->release();
		renderItem = new RenderItem(shape, body, color);

		scene->addActor(*body);
	}

	RigidDynamic* clone(PxPhysics* gPx, PxScene* scene) { 
		return new RigidDynamic(gPx, scene, pose.p, vel, scale, col, mass, lifeTime, bb); 
	}

	RigidDynamic* clone(PxPhysics* gPx, PxScene* scene, Vector3 pos, Vector3 v) {
		return new RigidDynamic(gPx, scene, pos, v, scale, col, mass, lifeTime, bb);
	}
};


class RigidStatic : public Particle {
protected:
	PxRigidStatic* body;
	PxShape* shape;
	Vector3 scale;

public:
	RigidStatic(PxPhysics* gPx, PxScene* scene, Vector3 pos, Vector3 s, Vector4 color,
		double mass, double lt = -1, BoundingBox* bb = nullptr) : Particle(pos, Vector3(0), 1, mass, color, lt, bb), scale(s) {
		shape = CreateShape(PxBoxGeometry(scale));
		body = gPx->createRigidStatic(pose);
		body->attachShape(*shape);

		renderItem->release();
		renderItem = new RenderItem(shape, body, color);

		scene->addActor(*body);
	}

	RigidStatic* clone(PxPhysics* gPx, PxScene* scene) {
		return new RigidStatic(gPx, scene, pose.p, scale, col, mass, lifeTime, bb);
	}

	RigidStatic* clone(PxPhysics* gPx, PxScene* scene, Vector3 pos) {
		return new RigidStatic(gPx, scene, pos, scale, col, mass, lifeTime, bb);
	}

	virtual bool integrate(double t) {
		return true;
	}
};