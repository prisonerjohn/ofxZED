#pragma once

#include "Body.h"
#include "Object.h"
#include "Util.h"

#include "sl/Camera.hpp"

#include "ofEvents.h"
#include "ofParameter.h"
#include "ofPixels.h"
#include "ofTexture.h"
#include "ofThread.h"
#include "ofVboMesh.h"

namespace ofxZED
{
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

		const sl::Camera& getNativeCamera() const;

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

		bool setPoseEnabled(PositionalTrackingParameters params = PositionalTrackingParameters());
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

		bool setObjectsEnabled(ObjectDetectionOptions options, ObjectDetectionParameters params = ObjectDetectionParameters(), ObjectDetectionRuntimeParameters rtParams = ObjectDetectionRuntimeParameters());
		void setObjectsDisabled();
		bool isObjectsEnabled() const;
		const std::vector<std::shared_ptr<ObjectData>>& getObjectsData() const;
		const ofPixels& getObjectsPixels() const;
		const ofTexture& getObjectsTexture() const;

		bool setBodiesEnabled(BodyTrackingOptions options, BodyTrackingParameters params = BodyTrackingParameters(), BodyTrackingRuntimeParameters rtParams = BodyTrackingRuntimeParameters());
		void setBodiesDisabled();
		bool isBodiesEnabled() const;
		const std::vector<std::shared_ptr<BodyData>>& getBodiesData() const;
		const ofPixels& getBodiesPixels() const;
		const ofTexture& getBodiesTexture() const;

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

		sl::Objects objects;
		ObjectDetectionRuntimeParameters objectsRTParams;
		ObjectDetectionOptions objectsOptions;
		std::vector<std::shared_ptr<ObjectData>> objectsData;
		ofPixels objectsPixels;
		ofTexture objectsTexture;
		bool bObjectsEnabled;
		bool bObjectsNeedsUpdate;

		sl::Bodies bodies;
		BodyTrackingRuntimeParameters bodiesRTParams;
		BodyTrackingOptions bodiesOptions;
		std::vector<std::shared_ptr<BodyData>> bodiesData;
		ofPixels bodiesPixels;
		ofTexture bodiesTexture;
		bool bBodiesEnabled;
		bool bBodiesNeedsUpdate;

		bool bRunning;

		int threadFrame;
		int updateFrame;

		std::condition_variable condition;
	};
}
