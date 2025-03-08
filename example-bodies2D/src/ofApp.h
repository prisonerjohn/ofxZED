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

	void drawBounds2D(const ofxZED::Bounds2D& bounds);
	void drawBody2D(const std::vector<glm::vec2>& joints);

	ofxZED::Camera zed;
};
