#include "ofApp.h"

void ofApp::setup()
{
	zed.open();

	auto bodiesOptions = ofxZED::BodyTrackingOptions();
	bodiesOptions.bounds_3d = true;
	bodiesOptions.joints_3d = true;
	auto bodyParams = ofxZED::BodyTrackingParameters();
	bodyParams.detection_model = sl::BODY_TRACKING_MODEL::HUMAN_BODY_MEDIUM;
	bodyParams.enable_body_fitting = true;
	auto bodyRTParams = ofxZED::BodyTrackingRuntimeParameters();
	bodyRTParams.detection_confidence_threshold = 40;
	bodyRTParams.skeleton_smoothing = 0.7;
	zed.setBodiesEnabled(bodiesOptions, bodyParams, bodyRTParams);
}

void ofApp::exit()
{
	zed.close();
}

void ofApp::update() 
{

}

void ofApp::draw()
{
	if (zed.isRunning())
	{
		cam.begin();
		ofEnableDepthTest();
		ofPushMatrix();
		ofMultMatrix(zed.getPoseTransform());
		ofRotateXDeg(180);
		{
			zed.getPointsMesh().draw();

			for (int i = 0; i < zed.getBodiesData().size(); ++i)
			{
				auto body = zed.getBodiesData()[i];

				ofColor c;
				c.setHsb(i * 50, 255, 255);
				ofSetColor(c);

				// Draw bounds.
				ofNoFill();
				drawBounds3D(body->getBounds3D());
				drawBounds3D(body->getHeadBounds3D());
				ofFill();

				// Draw skeleton.
				drawBody3D(body->getJoints3D());
			}
			ofSetColor(255);
		}
		ofPopMatrix();
		ofDisableDepthTest();
		cam.end();
	}
}

void ofApp::drawBounds3D(const ofxZED::Bounds3D& bounds)
{
	ofDrawBox(bounds.center, bounds.size.x, bounds.size.y, bounds.size.z);
}

void ofApp::drawBody3D(const std::vector<glm::vec3>& joints)
{
	if (joints.size() == 18)
	{
		for (auto& p : sl::BODY_18_BONES)
		{
			const auto i = sl::getIdx(p.first);
			const auto j = sl::getIdx(p.second);
			ofDrawLine(joints[i], joints[j]);
		}
	}
	else if (joints.size() == 34)
	{
		for (auto& p : sl::BODY_34_BONES)
		{
			const auto i = sl::getIdx(p.first);
			const auto j = sl::getIdx(p.second);
			ofDrawLine(joints[i], joints[j]);
		}
	}
	else if (joints.size() == 38)
	{
		for (auto& p : sl::BODY_38_BONES)
		{
			const auto i = sl::getIdx(p.first);
			const auto j = sl::getIdx(p.second);
			ofDrawLine(joints[i], joints[j]);
		}
	}
}
