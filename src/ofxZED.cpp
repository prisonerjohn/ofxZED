#include "ofxZED.h"
namespace ofxZED
{
	Camera::Camera()
	{
	}

	Camera::~Camera()
	{
		close();
	}

	void Camera::init(bool useColorImage, bool useDepthImage, bool useTracking, int cameraID, sl::DEPTH_MODE depthMode, sl::RESOLUTION resolution, sl::SENSING_MODE sensingMode, bool captureStereo, float fps)
	{
		bCaptureStereo = captureStereo;
		bUseColorImage = useColorImage;
		bUseDepthImage = useDepthImage;
		bUseTracking = useTracking;

		// Set configuration parameters
		sl::InitParameters init_params;
		init_params.camera_resolution = resolution;
		init_params.camera_fps = fps;
		init_params.depth_mode = depthMode;
		init_params.enable_right_side_measure = bCaptureStereo;
		init_params.input.setFromCameraID(cameraID);
		init_params.coordinate_units = sl::UNIT_METER;
		init_params.coordinate_system = sl::COORDINATE_SYSTEM_RIGHT_HANDED_Y_UP;

		ofLog() << "Initializing ZED camera." << std::endl;

		zed = new sl::Camera();
		ofLog() << "Resolution Mode:" << resolution << std::endl;

		sl::ERROR_CODE zederr = zed->open(init_params);

		if (zederr != sl::SUCCESS)
		{
			close();
			ofLog() << "ERROR: " << sl::toString(zederr).c_str() << endl;
			return;
		}
		else
		{
			bZedReady = true;
			ofLog() << "ZED initialized." << endl;
		}

		if (bUseTracking) {
			// Enable positional tracking with default parameters
			sl::TrackingParameters tracking_parameters;
			zederr = zed->enableTracking(tracking_parameters);

			if (zederr != sl::SUCCESS)
			{
				close();
				ofLog() << "ERROR: " << sl::toString(zederr).c_str() << endl;
				return;
			}
		}

		zedWidth = zed->getResolution().width;
		zedHeight = zed->getResolution().height;

		ofLog() << "Resolution: " << zedWidth << ", " << zedHeight << endl;
		ofLog() << "FPS: " << getCurrentFPS() << endl;


		colorTextures[CAPTURE_LEFT].allocate(zedWidth, zedHeight, GL_RGBA);
		depthTextures[CAPTURE_LEFT].allocate(zedWidth, zedHeight, GL_LUMINANCE32F_ARB);
		if (bCaptureStereo)
		{
			colorTextures[CAPTURE_RIGHT].allocate(zedWidth, zedHeight, GL_RGBA);
			depthTextures[CAPTURE_RIGHT].allocate(zedWidth, zedHeight, GL_LUMINANCE32F_ARB);
		}

		bRequestNewFrame = true;
		if (bUseDepthImage)
		{
			rt.enable_depth = true;
			rt.sensing_mode = sensingMode;
		}
		else if (bUseColorImage)
		{
			rt.enable_depth = false;
		}

		startThread();
	}

	void Camera::close()
	{
		if (isThreadRunning()) {
			waitForThread(true);
		}

		if (zed) {
			if (bUseTracking) {
				zed->disableTracking();
			}
			zed->close();
			zed = nullptr;
			bZedReady = false;
		}
	}

	ofVec2f Camera::getImageDimensions()
	{
		return ofVec2f(zedWidth, zedHeight);
	}

	float Camera::getCurrentFPS() 
	{
		return zed->getCurrentFPS();
	}

	void Camera::threadedFunction()
	{
		while (isThreadRunning()) {
			sl::ERROR_CODE zederr = sl::ERROR_CODE_NOT_A_NEW_FRAME;
			if (bRequestNewFrame && bZedReady) {
				zederr = zed->grab(rt);

				if (lock()) {
					if (zederr == sl::ERROR_CODE_CAMERA_NOT_DETECTED) {
						bDisconnected = true;
						bRequestNewFrame = false;
						ofSleepMillis(10);
					}
					else if (zederr == sl::SUCCESS) {
						cameraTimestamp = zed->getTimestamp(sl::TIME_REFERENCE_IMAGE);

						if (bUseTracking) {
							trackingState = zed->getPosition(pose);
						}
						else {
							trackingState = sl::TRACKING_STATE_OFF;
						}

						bNewBuffer = true;
						bRequestNewFrame = false;
					}
					unlock();
				}
			}
			ofSleepMillis(1);
		}
	}


	void Camera::update()
	{
		bNewFrame = false;

		if (bZedReady && bDisconnected) {
			close();
			bDisconnected = false;
			return;
		}

		if (!bZedReady || !zed) {
			return;
		}

		if (bNewBuffer) {
			if (lock()) {
				if (bUseColorImage) {
					{
						auto ret = zed->retrieveImage(this->cl, sl::VIEW_LEFT);
						if (ret == sl::SUCCESS) {
							colorTextures[CAPTURE_LEFT].loadData(this->cl.getPtr<uint8_t>(), zedWidth, zedHeight, GL_RGBA);
						}
					}
					if (bCaptureStereo)
					{
						auto ret = zed->retrieveImage(this->cr, sl::VIEW_RIGHT);
						if (ret == sl::SUCCESS) {
							colorTextures[CAPTURE_RIGHT].loadData(this->cr.getPtr<uint8_t>(), zedWidth, zedHeight, GL_RGBA);
						}
					}
				}

				if (bUseDepthImage) {
					{
						auto ret = zed->retrieveMeasure(this->dl, sl::MEASURE_DEPTH);
						if (ret == sl::SUCCESS) {
							depthTextures[CAPTURE_LEFT].loadData(this->dl.getPtr<float>(), zedWidth, zedHeight, GL_LUMINANCE);
						}
						else {
							ofLogError() << sl::toString(ret).c_str() << endl;
						}
					}
					if (bCaptureStereo)
					{
						auto ret = zed->retrieveMeasure(this->dr, sl::MEASURE_DEPTH_RIGHT);
						if (ret == sl::SUCCESS) {
							depthTextures[CAPTURE_RIGHT].loadData(this->dr.getPtr<float>(), zedWidth, zedHeight, GL_LUMINANCE);
						}
						else {
							ofLogError() << sl::toString(ret).c_str() << endl;
						}
					}
				}
				unlock();
				bNewBuffer = false;
				bRequestNewFrame = true;
				bNewFrame = true;
				lastNewFrame = ofGetFrameNum();
			}
		}

	}
}
