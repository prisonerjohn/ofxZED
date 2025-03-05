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

	ofxZED::Camera zed;

	ofEasyCam cam;
};
