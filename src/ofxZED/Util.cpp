#pragma once

#include "Util.h"

namespace ofxZED
{
	void Bounds2D::setFromCorners(sl::uint2 tl, sl::uint2 br)
	{
		this->origin = glm::ivec2(tl.x, tl.y);
		this->size = glm::ivec2(br.x - tl.x + 1, br.y - tl.y + 1);
	}

	void Bounds3D::setFromCorners(sl::float3 mn, sl::float3 mx)
	{
		this->center = glm::vec3(ofLerp(mn.x, mx.x, 0.5f), ofLerp(mn.y, mx.y, 0.5f), ofLerp(mn.z, mx.z, 0.5f));
		this->size = glm::vec3(fabs(mx.x - mn.x), fabs(mx.y - mn.y), fabs(mx.z - mn.z));
	}
}
