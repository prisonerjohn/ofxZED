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
	void drawBody3D(const std::vector<glm::vec3>& joints);

	ofxZED::Camera zed;

	ofEasyCam cam;
};
