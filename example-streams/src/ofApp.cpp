#include "ofApp.h"

void ofApp::setup()
{
	zed.open();
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
		auto viewport = ofRectangle(0, 0, 1280, 720);
		cam.setControlArea(viewport);
		cam.begin(viewport);
		ofEnableDepthTest();
		ofPushMatrix();
		ofRotateXDeg(180);
		{
			zed.getPointsMesh().draw();
		}
		ofPopMatrix();
		ofDisableDepthTest();
		cam.end();
		ofDrawBitmapStringHighlight("POINTS", 10, 20);

		zed.getColorTexture().draw(1280, 0, 640, 360);
		ofDrawBitmapStringHighlight("COLOR", 1280 + 10, 20);

		zed.getDepthTexture().draw(1280, 360, 640, 360);
		ofDrawBitmapStringHighlight("DEPTH", 1280 + 10, 360 + 20);
	}
}
