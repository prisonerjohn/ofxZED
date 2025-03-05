#include "ofApp.h"

void ofApp::setup()
{
	zed.open();
	zed.setPoseEnabled();
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
		}
		ofPopMatrix();
		ofDisableDepthTest();
		cam.end();
	}
}
