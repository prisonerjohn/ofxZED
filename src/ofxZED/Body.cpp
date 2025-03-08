#pragma once

#include "Body.h"

namespace ofxZED
{
	BodyData::BodyData()
		: id(-1)
		, trackingState(ObjectTrackingState::LAST)
		, actionState(ObjectActionState::LAST)
		, confidence(0.0f)
	{};

	void BodyData::set(const sl::BodyData& data, BodyTrackingOptions options, int frameWidth, int frameHeight)
	{
		this->id = data.id;
		this->trackingState = data.tracking_state;
		this->actionState = data.action_state;
		this->confidence = data.confidence;

		if (options.bounds_2d)
		{
			this->bounds2D.setFromCorners(data.bounding_box_2d[0], data.bounding_box_2d[2]);
			this->headBounds2D.setFromCorners(data.head_bounding_box_2d[0], data.head_bounding_box_2d[2]);
		}
		//if (options.bounds_3d)
		//{
		//	this->.bounds3D.setFromCorners(data.bounding_box[0], data.bounding_box[6]);
		//	this->.headBounds3D.setFromCorners(data.head_bounding_box[0], data.head_bounding_box[6]);
		//}
		if (options.joints_2d)
		{
			this->joints2D.resize(data.keypoint_2d.size());
			for (size_t j = 0; j < data.keypoint_2d.size(); ++j)
			{
				this->joints2D[j] = glm::vec2(data.keypoint_2d[j].x, data.keypoint_2d[j].y);
			}
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

	void BodyData::updateMask()
	{
		this->maskTexture.loadData(this->maskPixels);
	}

	size_t BodyData::getId() const
	{
		return this->id;
	}

	ObjectTrackingState BodyData::getTrackingState() const
	{
		return this->trackingState;
	}

	ObjectActionState BodyData::getActionState() const
	{
		return this->actionState;
	}

	float BodyData::getConfidence() const
	{
		return this->confidence;
	}

	const Bounds2D& BodyData::getBounds2D() const
	{
		return this->bounds2D;
	}

	const Bounds2D& BodyData::getHeadBounds2D() const
	{
		return this->headBounds2D;
	}

	const std::vector<glm::vec2>& BodyData::getJoints2D() const
	{
		return this->joints2D;
	}

	const ofPixels& BodyData::getMaskPixels() const
	{
		return this->maskPixels;
	}

	const ofTexture& BodyData::getMaskTexture() const
	{
		return this->maskTexture;
	}
}
