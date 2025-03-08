#pragma once

#include <vector>

#include "Util.h"

#include "sl/Camera.hpp"

#include "ofPixels.h"
#include "ofTexture.h"
#include "ofVectorMath.h"

namespace ofxZED
{
	struct BodyTrackingOptions
	{
	public:
		bool bounds_2d;
		bool bounds_3d;
		bool joints_2d;
		bool joints_3d;
		bool mask_split;
		bool mask_combined;

		BodyTrackingOptions()
			: bounds_2d(false)
			, bounds_3d(false)
			, joints_2d(false)
			, joints_3d(false)
			, mask_split(false)
			, mask_combined(false)
		{}
	};

	class BodyData
	{
	public:
		BodyData();

		void set(const sl::BodyData& data, BodyTrackingOptions options, int frameWidth, int frameHeight);
		void updateMask();

		size_t getId() const;

		ObjectTrackingState getTrackingState() const;
		ObjectActionState getActionState() const;
		float getConfidence() const;

		const Bounds2D& getBounds2D() const;
		const Bounds2D& getHeadBounds2D() const;
		const std::vector<glm::vec2>& getJoints2D() const;

		const ofPixels& getMaskPixels() const;
		const ofTexture& getMaskTexture() const;

	private:
		size_t id;
		
		ObjectTrackingState trackingState;
		ObjectActionState actionState;
		float confidence;

		Bounds2D bounds2D;
		Bounds2D headBounds2D;
		std::vector<glm::vec2> joints2D;

		Bounds3D bounds3D;
		Bounds3D headBounds3D;
		std::vector<glm::vec3> joints3D;
		glm::quat rootOrientation;

		ofPixels maskPixels;
		ofTexture maskTexture;
	};
}
