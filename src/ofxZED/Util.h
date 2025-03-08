#pragma once

#include "sl/Camera.hpp"

#include "ofVectorMath.h"

namespace ofxZED
{
	typedef sl::OBJECT_TRACKING_STATE ObjectTrackingState;
	typedef sl::OBJECT_ACTION_STATE ObjectActionState;

	class Bounds2D
	{
	public:
		glm::ivec2 origin;
		glm::ivec2 size;

		void setFromCorners(sl::uint2 tl, sl::uint2 br);
	};

	class Bounds3D
	{
	public:
		glm::vec3 center;
		glm::vec3 size;

		void setFromCorners(sl::float3 mn, sl::float3 mx);
	};
}
