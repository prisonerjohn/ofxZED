#pragma once

#include "Object.h"

namespace ofxZED
{
	ObjectData::ObjectData()
		: id(-1)
		, label(ObjectClass::LAST)
		, sublabel(ObjectSubclass::LAST)
		, trackingState(ObjectTrackingState::LAST)
		, actionState(ObjectActionState::LAST)
		, confidence(0.0f)
	{};

	void ObjectData::set(const sl::ObjectData& data, ObjectDetectionOptions options, int frameWidth, int frameHeight)
	{
		this->id = data.id;
		this->label = data.label;
		this->sublabel = data.sublabel;
		this->trackingState = data.tracking_state;
		this->actionState = data.action_state;
		this->confidence = data.confidence;

		if (options.bounds_2d)
		{
			this->bounds2D.setFromCorners(data.bounding_box_2d[0], data.bounding_box_2d[2]);
		}
		if (options.bounds_3d)
		{
			this->bounds3D.setFromCenterSize(data.position, data.dimensions);
		}
		if (options.mask_split)
		{
			this->maskPixels.allocate(frameWidth, frameHeight, 1);
			this->maskPixels.set(0);
			auto pixelsData = this->maskPixels.getData();
			const auto maskData = data.mask.getPtr<sl::uchar1>(sl::MEM::CPU);
			for (size_t y = 0; y < data.mask.getHeight(); ++y)
			{
				for (size_t x = 0; x < data.mask.getWidth(); ++x)
				{
					const size_t m = y * data.mask.getWidth() + x;
					const size_t p = (data.bounding_box_2d[0].y + y) * this->maskPixels.getWidth() + (data.bounding_box_2d[0].x + x);
					pixelsData[p] = maskData[m];
				}
			}
		}
	}

	void ObjectData::updateMask()
	{
		this->maskTexture.loadData(this->maskPixels);
	}

	size_t ObjectData::getId() const
	{
		return this->id;
	}

	ObjectClass ObjectData::getLabel() const
	{
		return this->label;
	}

	ObjectSubclass ObjectData::getSubLabel() const
	{
		return this->sublabel;
	}

	ObjectTrackingState ObjectData::getTrackingState() const
	{
		return this->trackingState;
	}

	ObjectActionState ObjectData::getActionState() const
	{
		return this->actionState;
	}

	float ObjectData::getConfidence() const
	{
		return this->confidence;
	}

	const Bounds2D& ObjectData::getBounds2D() const
	{
		return this->bounds2D;
	}

	const Bounds3D& ObjectData::getBounds3D() const
	{
		return this->bounds3D;
	}

	const ofPixels& ObjectData::getMaskPixels() const
	{
		return this->maskPixels;
	}

	const ofTexture& ObjectData::getMaskTexture() const
	{
		return this->maskTexture;
	}
}
