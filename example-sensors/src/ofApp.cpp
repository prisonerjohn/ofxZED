#include "ofApp.h"

void ofApp::setup()
{
	auto params = ofxZED::InitParameters();
	params.sensors_required = true;
	zed.open(params);

	keyPressed(' ');
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
		if (bUseIMU)
		{
			ofMultMatrix(zed.getIMUTransform());
		}
		else
		{
			ofMultMatrix(zed.getPoseTransform());
		}
		ofRotateXDeg(180);
		{
			zed.getPointsMesh().draw();

			if (bUseIMU)
			{
				ofSetColor(ofColor::red);
				ofDrawLine(glm::vec3(0.0f), zed.getIMULinearAcceleration());
				ofSetColor(ofColor::green);
				ofDrawLine(glm::vec3(0.0f), zed.getIMUAngularVelocity());
			}
		}
		ofPopMatrix();
		ofDisableDepthTest();
		cam.end();
	}
}

void ofApp::keyPressed(int key)
{
	if (key == ' ')
	{
		bUseIMU = !bUseIMU;

		bUseIMU ? zed.setPoseDisabled() : zed.setPoseEnabled();
		zed.setSensorsEnabled(bUseIMU);
	}
}
