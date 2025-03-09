#pragma once

#include "ofMain.h"
#include "ofxZED.h"

class ofApp
	: public ofBaseApp
{
public:
	void setup();
	void exit();

	void update();
	void draw();

	void drawBounds2D(std::shared_ptr<ofxZED::ObjectData> obj);

	ofxZED::Camera zed;
};
