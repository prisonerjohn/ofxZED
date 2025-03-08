#include "ofApp.h"

void ofApp::setup()
{
	auto initParams = ofxZED::InitParameters();
	initParams.coordinate_units = sl::UNIT::METER;
	initParams.coordinate_system = sl::COORDINATE_SYSTEM::RIGHT_HANDED_Y_UP;
	zed.open(initParams);

	auto bodiesOptions = ofxZED::BodyTrackingOptions();
	bodiesOptions.bounds_2d = true;
	bodiesOptions.joints_2d = true;
	bodiesOptions.mask_split = true;
	auto bodyParams = ofxZED::BodyTrackingParameters();
	bodyParams.detection_model = sl::BODY_TRACKING_MODEL::HUMAN_BODY_MEDIUM;
	bodyParams.enable_segmentation = true;
	bodyParams.enable_body_fitting = true;
	auto bodyRTParams = ofxZED::BodyTrackingRuntimeParameters();
	bodyRTParams.detection_confidence_threshold = 40;
	bodyRTParams.skeleton_smoothing = 0.7;
	zed.setBodiesEnabled(bodiesOptions, bodyParams, bodyRTParams);
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
		for (int i = 0; i < zed.getBodiesData().size(); ++i)
		{
			auto body = zed.getBodiesData()[i];
			if (body == nullptr)
			{
				//ofLogError(__FUNCTION__) << "NO BODY " << i << " WTH???";
				continue;
			}

			c.setHsb(i * 50, 255, 255);
			
			// Draw bounds.
			ofSetColor(c);
			ofNoFill();
			drawBounds2D(body->getBounds2D());
			drawBounds2D(body->getHeadBounds2D());
			
			// Draw skeleton.
			drawBody2D(body->getJoints2D());

			// Draw mask.
			ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
			{
				c.a = 64;
				ofSetColor(c);
				ofFill();
				body->getMaskTexture().draw(0, 0);
			}
			ofEnableBlendMode(OF_BLENDMODE_ALPHA);
		}

		ofSetColor(255);
	}
}

void ofApp::drawBounds2D(const ofxZED::Bounds2D& bounds)
{
	ofDrawRectangle(bounds.origin, bounds.size.x, bounds.size.y);
}

void ofApp::drawBody2D(const std::vector<glm::vec2>& joints)
{
	if (joints.size() == 18)
	{
		for (auto& p : sl::BODY_18_BONES)
		{
			const auto i = sl::getIdx(p.first);
			const auto j = sl::getIdx(p.second);
			ofDrawLine(joints[i], joints[j]);
		}
	}
	else if (joints.size() == 34)
	{
		for (auto& p : sl::BODY_34_BONES)
		{
			const auto i = sl::getIdx(p.first);
			const auto j = sl::getIdx(p.second);
			ofDrawLine(joints[i], joints[j]);
		}
	}
	else if (joints.size() == 38)
	{
		for (auto& p : sl::BODY_38_BONES)
		{
			const auto i = sl::getIdx(p.first);
			const auto j = sl::getIdx(p.second);
			ofDrawLine(joints[i], joints[j]);
		}
	}
}
