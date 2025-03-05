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

		bool bRunning;

		int threadFrame;
		int updateFrame;
	};
}
