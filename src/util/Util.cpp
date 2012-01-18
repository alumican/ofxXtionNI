/*
Copyright (c) 2012 Yukiya Okuda
See LICENSE for full license information.
*/

#include "Util.h"

//--------------------------------------------------------------
int xtionni::Util::colorLength_ = 10;
ofColor *xtionni::Util::colors_[] = {
	new ofColor(255,   0,   0),
	new ofColor(255, 255,   0),
	new ofColor(255,   0, 255),
	new ofColor(  0,   0, 255),
	new ofColor(  0, 255, 255),
	new ofColor(  0, 128, 255),
	new ofColor(128,   0, 255),
	new ofColor(255, 128,   0),
	new ofColor(128, 255,   0),
	new ofColor(  0,   0, 128)
};

//--------------------------------------------------------------
xtionni::Util::Util(void)
{
}

xtionni::Util::~Util(void)
{
}

//--------------------------------------------------------------
void xtionni::Util::log(ostream &message, ofLogLevel level = OF_LOG_NOTICE)
{
	ofLog(level) << "XtionNI: " << message;
}

//--------------------------------------------------------------
void xtionni::Util::checkError(XnStatus status, string name)
{
	if (status != XN_STATUS_OK)
	{
		throw std::runtime_error("XtionNI: (" + name + ") " + (string)xnGetStatusString(status));
	}
}

void xtionni::Util::checkError(XnStatus status, string name, xn::EnumerationErrors *errors)
{
	if (status != XN_STATUS_OK)
	{
		XnChar strError[1024];
		errors->ToString(strError, 1024);
		throw std::runtime_error("XtionNI: (" + name + ") " + strError);
	}
}

//--------------------------------------------------------------
string xtionni::Util::getErrorString(XnCalibrationStatus error)
{
	switch (error)
	{
		case XN_CALIBRATION_STATUS_OK:
			return "OK";
		case XN_CALIBRATION_STATUS_NO_USER:
			return "NoUser";
		case XN_CALIBRATION_STATUS_ARM:
			return "Arm";
		case XN_CALIBRATION_STATUS_LEG:
			return "Leg";
		case XN_CALIBRATION_STATUS_HEAD:
			return "Head";
		case XN_CALIBRATION_STATUS_TORSO:
			return "Torso";
		case XN_CALIBRATION_STATUS_TOP_FOV:
			return "Top FOV";
		case XN_CALIBRATION_STATUS_SIDE_FOV:
			return "Side FOV";
		case XN_CALIBRATION_STATUS_POSE:
			return "Pose";
		default:
			return "Unknown";
	}
}

string xtionni::Util::getErrorString(XnPoseDetectionStatus error)
{
	switch (error)
	{
		case XN_POSE_DETECTION_STATUS_OK:
			return "OK";
		case XN_POSE_DETECTION_STATUS_NO_USER:
			return "NoUser";
		case XN_POSE_DETECTION_STATUS_TOP_FOV:
			return "Top FOV";
		case XN_POSE_DETECTION_STATUS_SIDE_FOV:
			return "Side FOV";
		case XN_POSE_DETECTION_STATUS_ERROR:
			return "General error";
		default:
			return "Unknown";
	}
}

//--------------------------------------------------------------
ofColor *xtionni::Util::getColor(int index)
{
	return colors_[index % colorLength_];
}

int xtionni::Util::getColorLength()
{
	return colorLength_;
}

//--------------------------------------------------------------
string xtionni::Util::getGestureName(xtionni::GestureType gestureType)
{
	return gestureType == xtionni::GESTURE_CLICK ? "Click":
		   gestureType == xtionni::GESTURE_WAVE ? "Wave" :
		   gestureType == xtionni::GESTURE_RAISE_HAND ? "RaiseHand" :
		   "";
}

xtionni::GestureType xtionni::Util::getGestureType(string gestureName)
{
	return gestureName == "Click" ? xtionni::GESTURE_CLICK :
		   gestureName == "Wave" ? xtionni::GESTURE_WAVE :
		   gestureName == "RaiseHand" ? xtionni::GESTURE_RAISE_HAND :
		   xtionni::GESTURE_NONE;
}