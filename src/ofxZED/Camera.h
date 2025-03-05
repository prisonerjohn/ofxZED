#pragma once

#include "sl/Camera.hpp"

#include "ofEvents.h"
#include "ofParameter.h"
#include "ofPixels.h"
#include "ofTexture.h"
#include "ofThread.h"
#include "ofVboMesh.h"

namespace ofxZED
{
	typedef sl::InitParameters InitParameters;
	typedef sl::PositionalTrackingParameters PositionalTrackingParameters;

	class Camera
		: ofThread
	{
	public:
		Camera();
		~Camera();

		bool open(InitParameters params = InitParameters());
		bool close();

		void threadedFunction() override;
		void update(ofEventArgs& args);

		bool isRunning() const;
		bool isFrameNew() const;

		void setColorEnabled(bool enabled);
		bool isColorEnabled() const;
		const ofPixels& getColorPixels() const;
		const ofTexture& getColorTexture() const;

		void setDepthEnabled(bool enabled);
		bool isDepthEnabled() const;
		const ofFloatPixels& getDepthPixels() const;
		const ofTexture& getDepthTexture() const;

		void setDisparityEnabled(bool enabled);
		bool isDisparityEnabled() const;
		const ofFloatPixels& getDisparityPixels() const;
		const ofTexture& getDisparityTexture() const;

		void setConfidenceEnabled(bool enabled);
		bool isConfidenceEnabled() const;
		const ofFloatPixels& getConfidencePixels() const;
		const ofTexture& getConfidenceTexture() const;

		void setNormalsEnabled(bool enabled);
		bool isNormalsEnabled() const;
		const ofFloatPixels& getNormalsPixels() const;
		const ofTexture& getNormalsTexture() const;

		void setPointsEnabled(bool enabled);
		bool isPointsEnabled() const;
		const ofVboMesh& getPointsMesh() const;

		void setPoseEnabled(PositionalTrackingParameters params = PositionalTrackingParameters());
		void setPoseDisabled();
		bool isPoseEnabled() const;
		glm::vec3 getPoseTranslation() const;
		glm::quat getPoseOrientation() const;
		glm::mat4 getPoseTransform() const;
		uint64_t getPoseMillis() const;

		void setSensorsEnabled(bool enabled);
		bool isSensorsEnabled();
		glm::vec3 getIMUTranslation() const;
		glm::quat getIMUOrientation() const;
		glm::mat4 getIMUTransform() const;
		glm::vec3 getIMULinearAcceleration() const;
		glm::vec3 getIMUAngularVelocity() const;
		uint64_t getIMUMillis() const;

		const sl::Camera& getNativeCamera() const;

	public:

	private:
		sl::InitParameters initParams;
		sl::Camera camera;
		sl::CameraInformation info;

		sl::Mat colorMat;
		ofPixels colorPixels;
		ofTexture colorTexture;
		bool bColorEnabled;
		bool bColorNeedsUpdate;

		sl::Mat depthMat;
		ofFloatPixels depthPixels;
		ofTexture depthTexture;
		bool bDepthEnabled;
		bool bDepthNeedsUpdate;

		sl::Mat disparityMat;
		ofFloatPixels disparityPixels;
		ofTexture disparityTexture;
		bool bDisparityEnabled;
		bool bDisparityNeedsUpdate;

		sl::Mat confidenceMat;
		ofFloatPixels confidencePixels;
		ofTexture confidenceTexture;
		bool bConfidenceEnabled;
		bool bConfidenceNeedsUpdate;

		sl::Mat normalsMat;
		ofFloatPixels normalsPixels;
		ofTexture normalsTexture;
		bool bNormalsEnabled;
		bool bNormalsNeedsUpdate;

		sl::Mat pointsMat;
		std::vector<glm::vec3> pointsPositions;
		std::vector<ofFloatColor> pointsColors;
		ofVboMesh pointsMesh;
		bool bPointsEnabled;
		bool bPointsNeedsUpdate;

		sl::Pose pose;
		bool bPoseEnabled;

		sl::SensorsData sensorsData;
		bool bSensorsEnabled;
		bool bSensorsNeedsUpdate;

		bool bRunning;

		int threadFrame;
		int updateFrame;
	};
}
