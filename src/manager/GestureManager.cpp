/*
Copyright (c) 2012 Yukiya Okuda
See LICENSE for full license information.
*/

#include "GestureManager.h"
#include "XtionNI.h"

//--------------------------------------------------------------
void XN_CALLBACK_TYPE xtionni::GestureManager::staticGestureRecognizedHandler_(xn::GestureGenerator &generator, const XnChar *gestureName, const XnPoint3D *idPosition, const XnPoint3D* endPosition, void* cookie)
{
	static_cast<xtionni::GestureManager*>(cookie)->gestureRecognizedHandler_(generator, gestureName, idPosition, endPosition, NULL);
}

void XN_CALLBACK_TYPE xtionni::GestureManager::staticGestureProgressHandler_(xn::GestureGenerator &generator, const XnChar *gestureName, const XnPoint3D *position, XnFloat progress, void *cookie)
{
	static_cast<xtionni::GestureManager*>(cookie)->gestureProgressHandler_(generator, gestureName, position, progress, NULL);
}

//--------------------------------------------------------------
xtionni::GestureManager::GestureManager(void)
{
	generator_ = NULL;
	lastGesture_ = NULL;
}

//--------------------------------------------------------------
xtionni::GestureManager::~GestureManager(void)
{
	finalize();
}

//--------------------------------------------------------------
bool xtionni::GestureManager::initialize(xtionni::XtionNI *xtion)
{
	return xtionni::BaseManager::initialize(xtion);
}

bool xtionni::GestureManager::update()
{
	return xtionni::BaseManager::update();
}

bool xtionni::GestureManager::finalize()
{
	return xtionni::BaseManager::finalize();
}

//--------------------------------------------------------------
bool xtionni::GestureManager::initialize_()
{
	generator_ = new xn::GestureGenerator();
	XnStatus status = xtion_->getContext()->FindExistingNode(XN_NODE_TYPE_GESTURE, *generator_);
	xtionni::Util::checkError(status, "Find gesture generator");

	XnCallbackHandle hGestureCallbacks;
	generator_->RegisterGestureCallbacks(staticGestureRecognizedHandler_, staticGestureProgressHandler_, this, hGestureCallbacks);

	lastGesture_ = new xtionni::Gesture();

	useProgress_ = false;
	minGestureInterval_ = 0;

	isGestureProgress_ = false;
	isGestureRecognized_ = false;

	ofLog(OF_LOG_NOTICE) << "Initialize Gesture";
	return true;
}

//--------------------------------------------------------------
bool xtionni::GestureManager::update_()
{
	isGestureProgress_ = false;
	isGestureRecognized_ = false;

	if (generator_->IsNewDataAvailable())
	{
		generator_->WaitAndUpdateData();
		return true;
	}
	return false;
}

//--------------------------------------------------------------
bool xtionni::GestureManager::finalize_()
{
	if (lastGesture_ != NULL)
	{
		delete lastGesture_;
		lastGesture_ = NULL;
	}

	if (generator_ != NULL)
	{
		generator_->Unref();
		generator_->Release();
		delete generator_;
		generator_ = NULL;
	}

	return true;
}

//--------------------------------------------------------------
void xtionni::GestureManager::addGesture(xtionni::GestureType gestureType)
{
	string gestureName = xtionni::Util::getGestureName(gestureType);
	XnStatus status = generator_->AddGesture(gestureName.c_str(), NULL);
	xtionni::Util::checkError(status, "Adding simple (openNI) gesture " + gestureName);
}

//--------------------------------------------------------------
void xtionni::GestureManager::removeGesture(xtionni::GestureType gestureType)
{
	string gestureName = xtionni::Util::getGestureName(gestureType);
	XnStatus status = generator_->RemoveGesture(gestureName.c_str());
	xtionni::Util::checkError(status, "Removing simple (openNI) gesture " + gestureName);
}

//--------------------------------------------------------------
void XN_CALLBACK_TYPE xtionni::GestureManager::gestureRecognizedHandler_(xn::GestureGenerator &generator, const XnChar *gestureName, const XnPoint3D *idPosition, const XnPoint3D* endPosition, void* cookie)
{
	int timestamp = lastGesture_->getTimestamp();
	int time = ofGetElapsedTimeMillis();

	if (timestamp == 0 || time > timestamp + minGestureInterval_)
	{
		ofLog(OF_LOG_NOTICE) << "Gesture RECOGNIZED: " << gestureName << " posID [" << (int)idPosition->X << ", " << (int)idPosition->Y << ", " << (int)idPosition->Z << "]  posEND [" << (int)endPosition->X << ", " << (int)endPosition->Y << ", " << (int)endPosition->Z << "]";

		lastGesture_->reset(gestureName, xtionni::GESTURE_RECOGNIZED, endPosition->X, endPosition->Y, endPosition->Z, 1.0, time);
		isGestureRecognized_ = true;
	}
	else
	{
		//ofLog(OF_LOG_NOTICE) << "Gesture FILTERED by time: " << ofGetElapsedTimeMillis() << " < " << (lastGesture_->timestamp + minGestureInterval_);
	}
}

void XN_CALLBACK_TYPE xtionni::GestureManager::gestureProgressHandler_(xn::GestureGenerator &generator, const XnChar *gestureName, const XnPoint3D *position, XnFloat progress, void *cookie)
{
	if (useProgress_)
	{
		ofLog(OF_LOG_NOTICE) << "Gesture PROGRESS: " << gestureName << "  posID [" << (int)position->X << ", " << (int)position->Y << ", " << (int)position->Z << "]  progress [" << progress << "]";

		lastGesture_->reset(gestureName, xtionni::GESTURE_PROGRESS, position->X, position->Y, position->Z, (float)progress, ofGetElapsedTimeMillis());
		isGestureProgress_ = true;
	}
}

//--------------------------------------------------------------
//GETTER
xn::GestureGenerator *xtionni::GestureManager::getGenerator() { return generator_; }

xtionni::Gesture *xtionni::GestureManager::getLastGesture() { return lastGesture_; }

bool xtionni::GestureManager::getUseProgress() { return useProgress_; }
void xtionni::GestureManager::setUseProgress(bool flag){ useProgress_ = flag; }

int xtionni::GestureManager::getMinGestureInterval() { return minGestureInterval_; }
void xtionni::GestureManager::setMinGestureInterval(int milliseconds){ minGestureInterval_ = milliseconds; }

bool xtionni::GestureManager::isGestureProgress() { return isGestureProgress_; }
bool xtionni::GestureManager::isGestureRecognized() { return isGestureRecognized_; }