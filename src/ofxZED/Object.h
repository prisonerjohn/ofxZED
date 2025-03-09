#pragma once

#include <vector>

#include "Util.h"

#include "sl/Camera.hpp"

#include "ofPixels.h"
#include "ofTexture.h"
#include "ofVectorMath.h"

namespace ofxZED
{
	struct ObjectDetectionOptions
	{
	public:
		bool bounds_2d;
		bool bounds_3d;
		bool mask_split;
		bool mask_combined;

		ObjectDetectionOptions()
			: bounds_2d(false)
			, bounds_3d(false)
			, mask_split(false)
			, mask_combined(false)
		{}
	};

	class ObjectData
	{
	public:
		ObjectData();

		void set(const sl::ObjectData& data, ObjectDetectionOptions options, int frameWidth, int frameHeight);
		void updateMask();

		size_t getId() const;

		ObjectClass getLabel() const;
		ObjectSubclass getSubLabel() const;

		ObjectTrackingState getTrackingState() const;
		ObjectActionState getActionState() const;
		float getConfidence() const;

		const Bounds2D& getBounds2D() const;
		const Bounds3D& getBounds3D() const;

		const ofPixels& getMaskPixels() const;
		const ofTexture& getMaskTexture() const;

	private:
		size_t id;

		ObjectClass label;
		ObjectSubclass sublabel;
		
		ObjectTrackingState trackingState;
		ObjectActionState actionState;
		float confidence;

		Bounds2D bounds2D;
		Bounds3D bounds3D;

		ofPixels maskPixels;
		ofTexture maskTexture;
	};
}
