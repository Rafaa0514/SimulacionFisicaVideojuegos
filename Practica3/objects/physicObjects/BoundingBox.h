#pragma once
#include "../../core.hpp"

class BoundingBox {
protected:
	Vector3 pos;
	Vector3 scale;
public:
	BoundingBox(Vector3 p, Vector3 s) : pos(p), scale(s) {}

	bool isInside(Vector3 p) {
		return (p.x < pos.x + scale.x && p.x > pos.x - scale.x) &&
			(p.y < pos.y + scale.y && p.y > pos.y - scale.y) &&
			(p.z < pos.z + scale.z && p.z > pos.z - scale.z);
	}
};