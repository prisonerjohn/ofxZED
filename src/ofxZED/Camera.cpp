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
	{

	}

	Camera::~Camera()
	{
		this->close();
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

		this->threadFrame = -1;
		this->updateFrame = 0;

		this->bColorNeedsUpdate = false;
		this->bDepthNeedsUpdate = false;
		this->bDisparityNeedsUpdate = false;
		this->bConfidenceNeedsUpdate = false;
		this->bNormalsNeedsUpdate = false;
		this->bPointsNeedsUpdate = false;

		ofAddListener(ofEvents().update, this, &Camera::update);
		this->startThread();
		this->bRunning = true;

		return true;
	}

	bool Camera::close()
	{
		if (!this->bRunning) return false;

		ofRemoveListener(ofEvents().update, this, &Camera::update);
		this->stopThread();
		this->bRunning = false;

		this->waitForThread();
		
		this->setPoseDisabled();
		this->camera.close();

		return true;
	}

	void Camera::threadedFunction()
	{
		while (isThreadRunning())
		{
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
		}
	}

	void Camera::update(ofEventArgs& args)
	{
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
	}

	bool Camera::isRunning() const
	{
		return this->bRunning;
	}

	bool Camera::isFrameNew() const
	{
		return this->threadFrame == this->updateFrame;
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

	void Camera::setPoseEnabled(PositionalTrackingParameters params)
	{
		auto result = this->camera.enablePositionalTracking(params);
		if (result != sl::ERROR_CODE::SUCCESS)
		{
			ofLogError(__FUNCTION__) << "Failed with code " << result << ": " << sl::toString(result);
			this->bPoseEnabled = false;
			return;
		}

		this->bPoseEnabled = true;
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
	const sl::Camera& Camera::getNativeCamera() const
	{
		return this->camera;
	}
}
