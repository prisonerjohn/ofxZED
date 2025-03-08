#pragma once

#include "Util.h"

namespace ofxZED
{
	void Bounds2D::setFromCorners(sl::uint2 tl, sl::uint2 br)
	{
		this->origin = glm::ivec2(tl.x, tl.y);
		this->size = glm::ivec2(br.x - tl.x + 1, br.y - tl.y + 1);
	}

	void Bounds3D::setFromCenterSize(sl::float3 center, sl::float3 size)
	{
		this->center = glm::vec3(center.x, center.y, center.z);
		this->size = glm::vec3(size.x, size.y, size.z);
	}

	void Bounds3D::setFromCorners(sl::float3 tlf, sl::float3 brb)
	{
		this->center = glm::vec3(ofLerp(tlf.x, brb.x, 0.5f), ofLerp(tlf.y, brb.y, 0.5f), ofLerp(tlf.z, brb.z, 0.5f));
		this->size = glm::vec3(fabs(brb.x - tlf.x), fabs(brb.y - tlf.y), fabs(brb.z - tlf.z));
	}
}
