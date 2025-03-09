#pragma once

#include "sl/Camera.hpp"

#include "ofVectorMath.h"

namespace ofxZED
{
	typedef sl::InitParameters InitParameters;
	typedef sl::PositionalTrackingParameters PositionalTrackingParameters;
	typedef sl::ObjectDetectionParameters  ObjectDetectionParameters;
	typedef sl::ObjectDetectionRuntimeParameters ObjectDetectionRuntimeParameters;
	typedef sl::BodyTrackingParameters BodyTrackingParameters;
	typedef sl::BodyTrackingRuntimeParameters BodyTrackingRuntimeParameters;

	typedef sl::OBJECT_CLASS ObjectClass;
	typedef sl::OBJECT_SUBCLASS ObjectSubclass;
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

		void setFromCenterSize(sl::float3 center, sl::float3 size);
		void setFromCorners(sl::float3 tlf, sl::float3 brb);
	};
}
