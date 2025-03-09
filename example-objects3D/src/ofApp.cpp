#include "ofApp.h"

void ofApp::setup()
{
	zed.open();

	auto objectOptions = ofxZED::ObjectDetectionOptions();
	objectOptions.bounds_3d = true;
	auto objectParams = ofxZED::ObjectDetectionParameters();
	objectParams.enable_segmentation = true;
	auto objectRTParams = ofxZED::ObjectDetectionRuntimeParameters();
	objectRTParams.detection_confidence_threshold = 40;
	zed.setObjectsEnabled(objectOptions, objectParams, objectRTParams);
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

			for (int i = 0; i < zed.getObjectsData().size(); ++i)
			{
				auto obj = zed.getObjectsData()[i];

				ofColor c;
				c.setHsb(i * 50, 255, 255);
				ofSetColor(c);

				// Draw bounds.
				ofNoFill();
				drawBounds3D(obj->getBounds3D());
				ofFill();
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
