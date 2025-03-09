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

	void drawBounds3D(const ofxZED::Bounds3D& bounds);

	ofxZED::Camera zed;

	ofEasyCam cam;
};
