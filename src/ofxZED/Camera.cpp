#include "Camera.h"

#include "ofLog.h"

namespace ofxZED
{
	Camera::Camera()
		: bRunning(false)
		, threadFrame(-1)
		, updateFrame(-1)
		, bColorEnabled(true)
		, bColorNeedsUpdate(false)
		, bDepthEnabled(true)
		, bDepthNeedsUpdate(false)
		, bDisparityEnabled(false)
		, bDisparityNeedsUpdate(false)
		, bConfidenceEnabled(false)
		, bConfidenceNeedsUpdate(false)
		, bNormalsEnabled(false)
		, bNormalsNeedsUpdate(false)
		, bPointsEnabled(true)
		, bPointsNeedsUpdate(false)
		, bPoseEnabled(false)
		, bSensorsEnabled(false)
		, bSensorsNeedsUpdate(false)
		, bBodiesEnabled(false)
		, bBodiesNeedsUpdate(false)
	{}

	Camera::~Camera()
	{
		this->close();
		this->waitForThread(false);
	}

	bool Camera::open(InitParameters params)
	{
		if (this->bRunning)
		{
			this->close();
		}

		auto result = this->camera.open(params);
		if (result != sl::ERROR_CODE::SUCCESS) 
		{
			ofLogError(__FUNCTION__) << "Failed with code " << result << ": " << sl::toString(result);
			return false;
		}

		this->initParams = params;

		this->info = this->camera.getCameraInformation();
		ofLogVerbose(__FUNCTION__) << "Success " << sl::toString(this->info.camera_model) << " (" << sl::toString(this->info.input_type) << ") with serial no " << this->info.serial_number;

		this->bSensorsEnabled = this->initParams.sensors_required;

		this->threadFrame = -1;
		this->updateFrame = 0;

		this->bColorNeedsUpdate = false;
		this->bDepthNeedsUpdate = false;
		this->bDisparityNeedsUpdate = false;
		this->bConfidenceNeedsUpdate = false;
		this->bNormalsNeedsUpdate = false;
		this->bPointsNeedsUpdate = false;
		this->bSensorsNeedsUpdate = false;
		this->bBodiesNeedsUpdate = false;

		ofAddListener(ofEvents().update, this, &Camera::update);
		this->startThread();
		this->bRunning = true;

		return true;
	}

	bool Camera::close()
	{
		if (!this->bRunning) return false;

		ofRemoveListener(ofEvents().update, this, &Camera::update);
		
		std::unique_lock<std::mutex> ulm(this->mutex);
		this->stopThread();
		this->condition.notify_all();
		
		this->bRunning = false;

		this->setBodiesDisabled();
		this->setPoseDisabled();
		this->camera.close();

		return true;
	}

	void Camera::threadedFunction()
	{
		while (isThreadRunning())
		{
			// Lock the mutex until the end of the block.
			std::unique_lock<std::mutex> ulm(this->mutex);

			ofLogVerbose(__FUNCTION__) << "Begin... " << this->threadFrame << "::" << this->updateFrame;

			auto result = this->camera.grab();
			if (result != sl::ERROR_CODE::SUCCESS)
			{
				ofLogError(__FUNCTION__) << "Failed with code " << result << ": " << sl::toString(result);
				continue;
			}

			if (this->isColorEnabled())
			{
				this->camera.retrieveImage(this->colorMat, sl::VIEW::LEFT);
				this->colorPixels.setFromPixels(this->colorMat.getPtr<sl::uchar1>(sl::MEM::CPU), this->colorMat.getWidth(), this->colorMat.getHeight(), OF_PIXELS_BGRA);
				this->bColorNeedsUpdate = true;
			}

			if (this->isDepthEnabled())
			{
				this->camera.retrieveMeasure(this->depthMat, sl::MEASURE::DEPTH);
				this->depthPixels.setFromPixels(this->depthMat.getPtr<sl::float1>(sl::MEM::CPU), this->depthMat.getWidth(), this->depthMat.getHeight(), 1);
				this->bDepthNeedsUpdate = true;
			}

			if (this->isDisparityEnabled())
			{
				this->camera.retrieveMeasure(this->disparityMat, sl::MEASURE::DISPARITY);
				this->disparityPixels.setFromPixels(this->disparityMat.getPtr<sl::float1>(sl::MEM::CPU), this->disparityMat.getWidth(), this->disparityMat.getHeight(), 1);
				this->bDisparityNeedsUpdate = true;
			}

			if (this->isConfidenceEnabled())
			{
				this->camera.retrieveMeasure(this->confidenceMat, sl::MEASURE::CONFIDENCE);
				this->confidencePixels.setFromPixels(this->confidenceMat.getPtr<sl::float1>(sl::MEM::CPU), this->confidenceMat.getWidth(), this->confidenceMat.getHeight(), 1);
				this->bConfidenceNeedsUpdate = true;
			}

			if (this->isNormalsEnabled())
			{
				this->camera.retrieveMeasure(this->normalsMat, sl::MEASURE::NORMALS);
				this->normalsPixels.setFromPixels(this->normalsMat.getPtr<sl::float1>(sl::MEM::CPU), this->normalsMat.getWidth(), this->normalsMat.getHeight(), 4);
				this->bNormalsNeedsUpdate = true;
			}

			if (this->isPointsEnabled())
			{
				this->camera.retrieveMeasure(this->pointsMat, sl::MEASURE::XYZRGBA);
				this->pointsPositions.resize(this->pointsMat.getResolution().area());
				this->pointsColors.resize(this->pointsMat.getResolution().area());
				sl::float4 val;
				for (int y = 0; y < this->pointsMat.getHeight(); ++y)
				{
					for (int x = 0; x < this->pointsMat.getWidth(); ++x)
					{
						this->pointsMat.getValue(x, y, &val, sl::MEM::CPU);
						int i = y * this->pointsMat.getWidth() + x;
						this->pointsPositions[i] = glm::vec3(val.x, val.y, val.z);
						char* col = reinterpret_cast<char*>(&val.w);
						this->pointsColors[i] = ofColor(col[0], col[1], col[2], col[3]);
					}
				}
				this->bPointsNeedsUpdate = true;
			}

			if (this->isPoseEnabled())
			{
				auto state = this->camera.getPosition(this->pose, sl::REFERENCE_FRAME::WORLD);
				if (state != sl::POSITIONAL_TRACKING_STATE::OK)
				{
					ofLogWarning(__FUNCTION__) << "Tracking state " << sl::toString(state);
				}
			}

			if (this->isSensorsEnabled())
			{
				auto result = this->camera.getSensorsData(this->sensorsData, sl::TIME_REFERENCE::IMAGE);
				if (result != sl::ERROR_CODE::SUCCESS)
				{
					ofLogError(__FUNCTION__) << "Sensors failed with code " << result << ": " << sl::toString(result);
				}
			}

			if (this->isBodiesEnabled())
			{
				this->camera.retrieveBodies(this->bodies, this->bodiesRTParams);

				//if (this->bodies.is_new)
				{
					if (this->bodiesOptions.mask_combined)
					{
						this->bodiesPixels.allocate(this->colorMat.getWidth(), this->colorMat.getHeight(), 1);
						this->bodiesPixels.set(0);
					}

					this->bodiesData.resize(this->bodies.body_list.size());
					for (size_t i = 0; i < this->bodies.body_list.size(); ++i)
					{
						sl::BodyData& b = this->bodies.body_list[i];

						if (this->bodiesData[i] == nullptr)
						{
							this->bodiesData[i] = std::make_shared<BodyData>();
						}

						auto d = this->bodiesData[i];
						d->set(b, this->bodiesOptions, this->colorMat.getWidth(), this->colorMat.getHeight());

						if (this->bodiesOptions.mask_combined)
						{
							auto pixelsData = this->bodiesPixels.getData();
							unsigned char v = b.id % 255;
							const auto maskData = b.mask.getPtr<sl::uchar1>(sl::MEM::CPU);
							for (size_t y = 0; y < b.mask.getHeight(); ++y)
							{
								for (size_t x = 0; x < b.mask.getWidth(); ++x)
								{
									const size_t m = y * b.mask.getWidth() + x;
									const size_t p = (b.bounding_box_2d[0].y + y) * this->bodiesPixels.getWidth() + (b.bounding_box_2d[0].x + x);
									pixelsData[p] = maskData[m] ? v : 0;
								}
							}
						}
					}
					this->bBodiesNeedsUpdate = true;
				}
			}

			ofLogVerbose(__FUNCTION__) << "Done! " << this->threadFrame << "::" << this->updateFrame;

			// Wait for the main thread to finish.
			this->condition.wait(ulm);
		}
	}

	void Camera::update(ofEventArgs& args)
	{
		std::unique_lock<std::mutex> ulm(this->mutex);

		ofLogVerbose(__FUNCTION__) << "Begin... " << this->threadFrame << "::" << this->updateFrame;

		++this->updateFrame;

		if (this->bColorNeedsUpdate)
		{
			this->colorTexture.loadData(this->colorPixels);
			this->bColorNeedsUpdate = false;
			this->threadFrame = this->updateFrame;
		}

		if (this->bDepthNeedsUpdate)
		{
			this->depthTexture.loadData(this->depthPixels);
			this->bDepthNeedsUpdate = false;
			this->threadFrame = this->updateFrame;
		}

		if (this->bDisparityNeedsUpdate)
		{
			this->disparityTexture.loadData(this->disparityPixels);
			this->bDisparityNeedsUpdate = false;
			this->threadFrame = this->updateFrame;
		}

		if (this->bConfidenceNeedsUpdate)
		{
			this->confidenceTexture.loadData(this->confidencePixels);
			this->bConfidenceNeedsUpdate = false;
			this->threadFrame = this->updateFrame;
		}

		if (this->bNormalsNeedsUpdate)
		{
			this->normalsTexture.loadData(this->normalsPixels);
			this->bNormalsNeedsUpdate = false;
			this->threadFrame = this->updateFrame;
		}

		if (this->bPointsNeedsUpdate)
		{
			this->pointsMesh.setUsage(GL_STREAM_DRAW);
			this->pointsMesh.setMode(OF_PRIMITIVE_POINTS);
			this->pointsMesh.getVertices().assign(this->pointsPositions.data(), this->pointsPositions.data() + this->pointsPositions.size());
			this->pointsMesh.getColors().assign(this->pointsColors.data(), this->pointsColors.data() + this->pointsColors.size());
			this->bPointsNeedsUpdate = false;
			this->threadFrame = this->updateFrame;
		}

		if (this->bBodiesNeedsUpdate)
		{
			if (this->bodiesOptions.mask_split)
			{
				for (auto d : this->bodiesData)
				{
					d->updateMask();
				}
			}
			if (this->bodiesOptions.mask_combined)
			{
				this->bodiesTexture.loadData(this->bodiesPixels);
			}
			this->bBodiesNeedsUpdate = false;
			this->threadFrame = this->updateFrame;
		}

		ofLogVerbose(__FUNCTION__) << "Done! " << this->threadFrame << "::" << this->updateFrame;

		this->condition.notify_all();
	}

	bool Camera::isRunning() const
	{
		return this->bRunning;
	}

	bool Camera::isFrameNew() const
	{
		return this->threadFrame == this->updateFrame;
	}

	const sl::Camera& Camera::getNativeCamera() const
	{
		return this->camera;
	}

	void Camera::setColorEnabled(bool enabled)
	{
		this->bColorEnabled = enabled;
	}

	bool Camera::isColorEnabled() const
	{
		return this->bColorEnabled;
	}

	const ofPixels& Camera::getColorPixels() const
	{
		return this->colorPixels;
	}

	const ofTexture& Camera::getColorTexture() const
	{
		return this->colorTexture;
	}

	void Camera::setDepthEnabled(bool enabled)
	{
		this->bDepthEnabled = enabled;
	}

	bool Camera::isDepthEnabled() const
	{
		return this->bDepthEnabled && this->initParams.depth_mode != sl::DEPTH_MODE::NONE;
	}

	const ofFloatPixels& Camera::getDepthPixels() const
	{
		return this->depthPixels;
	}

	const ofTexture& Camera::getDepthTexture() const
	{
		return this->depthTexture;
	}

	void Camera::setDisparityEnabled(bool enabled)
	{
		this->bDisparityEnabled = enabled;
	}

	bool Camera::isDisparityEnabled() const
	{
		return this->bDisparityEnabled && this->initParams.depth_mode != sl::DEPTH_MODE::NONE;
	}

	const ofFloatPixels& Camera::getDisparityPixels() const
	{
		return this->disparityPixels;
	}

	const ofTexture& Camera::getDisparityTexture() const
	{
		return this->disparityTexture;
	}

	void Camera::setConfidenceEnabled(bool enabled)
	{
		this->bConfidenceEnabled = enabled;
	}

	bool Camera::isConfidenceEnabled() const
	{
		return this->bConfidenceEnabled && this->initParams.depth_mode != sl::DEPTH_MODE::NONE;
	}

	const ofFloatPixels& Camera::getConfidencePixels() const
	{
		return this->confidencePixels;
	}

	const ofTexture& Camera::getConfidenceTexture() const
	{
		return this->confidenceTexture;
	}

	void Camera::setNormalsEnabled(bool enabled)
	{
		this->bNormalsEnabled = enabled;
	}

	bool Camera::isNormalsEnabled() const
	{
		return this->bNormalsEnabled;
	}

	const ofFloatPixels& Camera::getNormalsPixels() const
	{
		return this->normalsPixels;
	}

	const ofTexture& Camera::getNormalsTexture() const
	{
		return this->normalsTexture;
	}

	void Camera::setPointsEnabled(bool enabled)
	{
		this->bPointsEnabled = enabled;
	}

	bool Camera::isPointsEnabled() const
	{
		return this->bPointsEnabled;
	}

	const ofVboMesh& Camera::getPointsMesh() const
	{
		return this->pointsMesh;
	}

	bool Camera::setPoseEnabled(PositionalTrackingParameters params)
	{
		if (this->isPoseEnabled())
		{
			this->setPoseDisabled();
		}

		auto result = this->camera.enablePositionalTracking(params);
		if (result != sl::ERROR_CODE::SUCCESS)
		{
			ofLogError(__FUNCTION__) << "Failed with code " << result << ": " << sl::toString(result);
			this->bPoseEnabled = false;
			return false;
		}

		this->bPoseEnabled = true;
		return true;
	}

	void Camera::setPoseDisabled()
	{
		this->camera.disablePositionalTracking();
		this->bPoseEnabled = false;
	}

	bool Camera::isPoseEnabled() const
	{
		return this->bPoseEnabled;
	}

	glm::vec3 Camera::getPoseTranslation() const
	{
		const auto t = this->pose.getTranslation();
		return glm::vec3(t.x, t.y, t.z);
	}

	glm::quat Camera::getPoseOrientation() const
	{
		const auto o = this->pose.getOrientation();
		return glm::quat(o.w, o.x, o.y, o.z);
	}

	glm::mat4 Camera::getPoseTransform() const
	{
		auto transform = glm::mat4(1.0);
		transform *= glm::translate(this->getPoseTranslation());
		transform *= glm::toMat4(this->getPoseOrientation());
		return transform;
	}

	uint64_t Camera::getPoseMillis() const
	{
		return this->pose.timestamp.getMilliseconds();
	}

	void Camera::setSensorsEnabled(bool enabled)
	{
		this->bSensorsEnabled = enabled;
	}

	bool Camera::isSensorsEnabled()
	{
		return this->bSensorsEnabled && this->initParams.sensors_required;
	}

	glm::vec3 Camera::getIMUTranslation() const
	{
		const auto t = this->sensorsData.imu.pose.getTranslation();
		return glm::vec3(t.x, t.y, t.z);
	}

	glm::quat Camera::getIMUOrientation() const
	{
		const auto o = this->sensorsData.imu.pose.getOrientation();
		return glm::quat(o.w, o.x, o.y, o.z);
	}

	glm::mat4 Camera::getIMUTransform() const
	{
		auto transform = glm::mat4(1.0);
		transform *= glm::translate(this->getIMUTranslation());
		transform *= glm::toMat4(this->getIMUOrientation());
		return transform;
	}

	glm::vec3 Camera::getIMULinearAcceleration() const
	{
		const auto la = this->sensorsData.imu.linear_acceleration;
		return glm::vec3(la.x, la.y, la.z);
	}

	glm::vec3 Camera::getIMUAngularVelocity() const
	{
		const auto av = this->sensorsData.imu.angular_velocity;
		return glm::vec3(av.x, av.y, av.z);
	}

	uint64_t Camera::getIMUMillis() const
	{
		return this->sensorsData.imu.timestamp.getMilliseconds();
	}

	bool Camera::setBodiesEnabled(BodyTrackingOptions options, BodyTrackingParameters params, BodyTrackingRuntimeParameters rtParams)
	{
		if (params.enable_tracking && !this->isPoseEnabled())
		{
			if (!this->setPoseEnabled())
			{
				return false;
			}
		}
		
		if (this->isBodiesEnabled())
		{
			this->setBodiesDisabled();
		}

		auto result = this->camera.enableBodyTracking(params);
		if (result != sl::ERROR_CODE::SUCCESS)
		{
			ofLogError(__FUNCTION__) << "Failed with code " << result << ": " << sl::toString(result);
			this->bBodiesEnabled = false;
			return false;
		}

		this->bodiesOptions = options;
		this->bodiesRTParams = rtParams;

		this->bBodiesEnabled = true;
		return true;
	}

	void Camera::setBodiesDisabled()
	{
		this->camera.disableBodyTracking();
		this->bodiesData.clear();
		this->bBodiesEnabled = false;
	}

	bool Camera::isBodiesEnabled() const
	{
		return this->bBodiesEnabled;
	}

	const std::vector<std::shared_ptr<BodyData>>& Camera::getBodiesData() const
	{
		return this->bodiesData;
	}

	const ofPixels& Camera::getBodiesPixels() const
	{
		return this->bodiesPixels;
	}

	const ofTexture& Camera::getBodiesTexture() const
	{
		return this->bodiesTexture;
	}
}
