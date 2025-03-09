#include "ofApp.h"

void ofApp::setup()
{
	zed.open();

	auto objectOptions = ofxZED::ObjectDetectionOptions();
	objectOptions.bounds_2d = true;
	objectOptions.mask_split = true;
	auto objectParams = ofxZED::ObjectDetectionParameters();
	objectParams.enable_segmentation = true;
	zed.setObjectsEnabled(objectOptions, objectParams);
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
		zed.getColorTexture().draw(0, 0);

		ofColor c;
		for (int i = 0; i < zed.getObjectsData().size(); ++i)
		{
			auto obj = zed.getObjectsData()[i];
			
			c.setHsb(i * 50, 255, 255);
			
			// Draw bounds.
			ofSetColor(c);
			ofNoFill();
			drawBounds2D(obj);
			
			// Draw mask.
			ofEnableBlendMode(OF_BLENDMODE_SCREEN);
			{
				c.a = 96;
				ofSetColor(c);
				ofFill();
				obj->getMaskTexture().draw(0, 0);
			}
			ofEnableBlendMode(OF_BLENDMODE_ALPHA);
		}

		ofSetColor(255);
	}
}

void ofApp::drawBounds2D(std::shared_ptr<ofxZED::ObjectData> obj)
{
	const auto& bounds = obj->getBounds2D();

	ofDrawRectangle(bounds.origin, bounds.size.x, bounds.size.y);

	std::ostringstream oss;
	oss << obj->getLabel() << "::" << obj->getSubLabel();
	ofDrawBitmapStringHighlight(oss.str(), bounds.origin.x + 5, bounds.origin.y + 15);
}
