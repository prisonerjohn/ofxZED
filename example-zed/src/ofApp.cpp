#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	zed.init();

	ofShaderSettings depthSettings;
	depthSettings.bindDefaults = true;
	depthSettings.shaderFiles[GL_FRAGMENT_SHADER] = "shaders/gl2/depthMap.frag";
	depthShader.setup(depthSettings);

	ofShaderSettings colorSettings;
	colorSettings.bindDefaults = true;
	colorSettings.shaderFiles[GL_FRAGMENT_SHADER] = "shaders/gl2/colorImage.frag";
	colorShader.setup(colorSettings);
}

//--------------------------------------------------------------
void ofApp::update()
{
	zed.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	colorShader.begin();
	zed.getColorTexture(ofxZED::CAPTURE_LEFT).draw(0, 0);
	zed.getColorTexture(ofxZED::CAPTURE_RIGHT).draw(zed.zedWidth, 0);
	colorShader.end();

	depthShader.begin();
	zed.getDepthTexture(ofxZED::CAPTURE_LEFT).draw(0, zed.zedHeight);
	zed.getDepthTexture(ofxZED::CAPTURE_RIGHT).draw(zed.zedWidth, zed.zedHeight);
	depthShader.end();

	cam.begin();
	ofPushMatrix();
	ofScale(100, 100, 100);
	ofMultMatrix(zed.getTrackedPose());
	ofDrawAxis(0.3);
	ofDrawBox(0.1);
	ofSetColor(ofColor::white);
	zed.getPointsVbo().draw(GL_POINTS, 0, zed.zedWidth * zed.zedHeight);
	ofPopMatrix();

	ofDrawAxis(100);
	cam.end();

	ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 10, 20);
	{
		stringstream ss;
		auto pose = zed.getTrackedPose();
		ss << "pos:" << pose.getTranslation() << endl;
		ss << "rot:" << pose.getRotate().getEuler();
		ofDrawBitmapStringHighlight(ss.str(), 10, 40);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	//ofLog() << "Depth at (" << x << ", " << y << ") is " << zed.getDepthAtPoint(x, y) << " mm" << endl;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
