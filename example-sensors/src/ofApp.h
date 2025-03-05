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

	void keyPressed(int key);

	ofxZED::Camera zed;

	ofEasyCam cam;

	bool bUseIMU;
};
