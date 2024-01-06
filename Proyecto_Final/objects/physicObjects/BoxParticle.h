#pragma once
#include "Particle.h"

class BoxParticle : public Particle{
public:
	BoxParticle(Vector3 pos, Vector3 vel, float rad, double m, Vector4 col,
		double lt, BoundingBox* _bb, float dp = 0.998f) : Particle(pos, vel, rad, m, col, lt, _bb, dp) {
		renderItem->release();
		renderItem = new RenderItem(CreateShape(physx::PxBoxGeometry(rad * 2, rad * 2, rad * 2)), &pose, col);
	}

	void changeDimensions(double newR) {
		radious = newR;
		renderItem->release();
		renderItem = new RenderItem(CreateShape(physx::PxBoxGeometry(radious * 2, radious * 2, radious * 2)), &pose, color);
	}
};