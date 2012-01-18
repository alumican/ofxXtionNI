/*
Copyright (c) 2012 Yukiya Okuda
See LICENSE for full license information.
*/

#include "UserManager.h"
#include "XtionNI.h"

//--------------------------------------------------------------
void XN_CALLBACK_TYPE xtionni::UserManager::staticNewUserHandler_(xn::UserGenerator& generator, XnUserID userID, void* cookie)
{
	static_cast<xtionni::UserManager*>(cookie)->newUserHandler_(generator, userID, NULL);
}

void XN_CALLBACK_TYPE xtionni::UserManager::staticLostUserHandler_(xn::UserGenerator& generator, XnUserID userID, void* cookie)
{
	static_cast<xtionni::UserManager*>(cookie)->lostUserHandler_(generator, userID, NULL);
}

void XN_CALLBACK_TYPE xtionni::UserManager::staticPoseDetectedHandler_(xn::PoseDetectionCapability& capability, const XnChar* poseName, XnUserID userID, void* cookie)
{
	static_cast<xtionni::UserManager*>(cookie)->poseDetectedHandler_(capability, poseName, userID, NULL);
}

void XN_CALLBACK_TYPE xtionni::UserManager::staticCalibrationStartedHandler_(xn::SkeletonCapability& capability, XnUserID userID, void* cookie)
{
	static_cast<xtionni::UserManager*>(cookie)->calibrationStartedHandler_(capability, userID, NULL);
}

void XN_CALLBACK_TYPE xtionni::UserManager::staticCalibrationCompletedHandler_(xn::SkeletonCapability& capability, XnUserID userID, XnCalibrationStatus status, void* cookie)
{
	static_cast<xtionni::UserManager*>(cookie)->calibrationCompletedHandler_(capability, userID, status, NULL);
}

//--------------------------------------------------------------
xtionni::UserManager::UserManager(void)
{
	generator_ = NULL;
	users_ = NULL;
}

//--------------------------------------------------------------
xtionni::UserManager::~UserManager(void)
{
	finalize();
}

//--------------------------------------------------------------
bool xtionni::UserManager::initialize(xtionni::XtionNI *xtion)
{
	return xtionni::BaseManager::initialize(xtion);
}

bool xtionni::UserManager::update()
{
	return xtionni::BaseManager::update();
}

bool xtionni::UserManager::finalize()
{
	return xtionni::BaseManager::finalize();
}

//--------------------------------------------------------------
bool xtionni::UserManager::initialize_()
{
	generator_ = new xn::UserGenerator();
	XnStatus status = xtion_->getContext()->FindExistingNode(XN_NODE_TYPE_USER, *generator_);
	xtionni::Util::checkError(status, "Find user generator");

	if (!generator_->IsCapabilitySupported(XN_CAPABILITY_SKELETON) || !generator_->IsCapabilitySupported(XN_CAPABILITY_POSE_DETECTION))
	{
		throw std::runtime_error("User generator doesn't support either skeleton or pose detection.");
	}

	//
	generator_->GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);

	//
	XnCallbackHandle hUserCallbacks, hCalibrationStart, hCalibrationComplete, hPoseDetected, hCalibrationInProgress, hPoseInProgress;

	generator_->RegisterUserCallbacks(&xtionni::UserManager::staticNewUserHandler_, &xtionni::UserManager::staticLostUserHandler_, this, hUserCallbacks);
	xtionni::Util::checkError(status, "Register to user callbacks");
	
	status = generator_->GetSkeletonCap().RegisterToCalibrationStart(&xtionni::UserManager::staticCalibrationStartedHandler_, this, hCalibrationStart);
	xtionni::Util::checkError(status, "Register to calibration start");
	
	status = generator_->GetSkeletonCap().RegisterToCalibrationComplete(&xtionni::UserManager::staticCalibrationCompletedHandler_, this, hCalibrationComplete);
	xtionni::Util::checkError(status, "Register to calibration complete");

	isPoseNeeded_ = generator_->GetSkeletonCap().NeedPoseForCalibration();
	if (isPoseNeeded_)
	{
		status = generator_->GetPoseDetectionCap().RegisterToPoseDetected(&xtionni::UserManager::staticPoseDetectedHandler_, this, hPoseDetected);
		xtionni::Util::checkError(status, "Register to Pose Detected");
		generator_->GetSkeletonCap().GetCalibrationPose(calibrationPoseName_);
	}

	//
	userMaxLength_ = 15;
	users_ = new TrackedUser*[userMaxLength_];
	for (int i = 0; i < userMaxLength_; ++i)
	{
		users_[i] = new TrackedUser(i);
	}

	ofLog(OF_LOG_NOTICE) << "Initialize User";
	return true;
}

//--------------------------------------------------------------
bool xtionni::UserManager::update_()
{
	if (generator_->IsNewDataAvailable())
	{
		//
		generator_->WaitAndUpdateData();

		//
		userLength_ = userMaxLength_;
		XnUserID *userIDs = new XnUserID[userMaxLength_];
		generator_->GetUsers(userIDs, userLength_);
		xn::DepthGenerator* depthGenerator = xtion_->getDepthManager()->getGenerator();
		for (int i = 0; i < userMaxLength_; ++i)
		{
			XnUserID userID = i < userLength_ ? userIDs[i] : -1;
			users_[i]->update(generator_, depthGenerator, userID);
		}
		delete userIDs;

		return true;
	}

	return false;
}

//--------------------------------------------------------------
bool xtionni::UserManager::finalize_()
{
	if (generator_ != NULL)
	{
		generator_->Unref();
		generator_->Release();
		delete generator_;
		generator_ = NULL;
	}

	if (users_ != NULL)
	{
		for (int i = 0; i < userMaxLength_; ++i)
		{
			delete users_[i];
		}
		delete[] users_;
		users_ = NULL;
	}

	return true;
}

//--------------------------------------------------------------
void XN_CALLBACK_TYPE xtionni::UserManager::newUserHandler_(xn::UserGenerator& generator, XnUserID userID, void* cookie)
{
	XnUInt32 epochTime = 0;
	xnOSGetEpochTime(&epochTime);
	ofLog(OF_LOG_NOTICE) << epochTime << " New User " << userID;

	//New user found
	if (isPoseNeeded_)
	{
		generator_->GetPoseDetectionCap().StartPoseDetection(calibrationPoseName_, userID);
	}
	else
	{
		generator_->GetSkeletonCap().RequestCalibration(userID, TRUE);
	}
}

void XN_CALLBACK_TYPE xtionni::UserManager::lostUserHandler_(xn::UserGenerator& generator, XnUserID userID, void* cookie)
{
	XnUInt32 epochTime = 0;
	xnOSGetEpochTime(&epochTime);
	ofLog(OF_LOG_NOTICE) << epochTime << " Lost user " << userID;	
}

void XN_CALLBACK_TYPE xtionni::UserManager::poseDetectedHandler_(xn::PoseDetectionCapability& capability, const XnChar* poseName, XnUserID userID, void* cookie)
{
	XnUInt32 epochTime = 0;
	xnOSGetEpochTime(&epochTime);
	ofLog(OF_LOG_NOTICE) << epochTime << " Pose " << poseName << " detected for user " << userID;

	generator_->GetPoseDetectionCap().StopPoseDetection(userID);
	generator_->GetSkeletonCap().RequestCalibration(userID, TRUE);
}

void XN_CALLBACK_TYPE xtionni::UserManager::calibrationStartedHandler_(xn::SkeletonCapability& capability, XnUserID userID, void* cookie)
{
	XnUInt32 epochTime = 0;
	xnOSGetEpochTime(&epochTime);
	ofLog(OF_LOG_NOTICE) << epochTime << " Calibration started for user " << userID;
}

void XN_CALLBACK_TYPE xtionni::UserManager::calibrationCompletedHandler_(xn::SkeletonCapability& capability, XnUserID userID, XnCalibrationStatus status, void* cookie)
{
	XnUInt32 epochTime = 0;
	xnOSGetEpochTime(&epochTime);

	if (status == XN_CALIBRATION_STATUS_OK)
	{
		//Calibration succeeded
		ofLog(OF_LOG_NOTICE) << epochTime << " Calibration complete, start tracking user " << userID;
		generator_->GetSkeletonCap().StartTracking(userID);
	}
	else
	{
		//Calibration failed
		ofLog(OF_LOG_NOTICE) << epochTime << " Calibration failed for user " << userID;

		if(status == XN_CALIBRATION_STATUS_MANUAL_ABORT)
		{
			ofLog(OF_LOG_NOTICE) << "Manual abort occured, stop attempting to calibrate!";
			return;
		}
		if (isPoseNeeded_)
		{
			generator_->GetPoseDetectionCap().StartPoseDetection(calibrationPoseName_, userID);
		}
		else
		{
			generator_->GetSkeletonCap().RequestCalibration(userID, TRUE);
		}
	}
}

//--------------------------------------------------------------
void xtionni::UserManager::drawSkeletons(float offsetX, float offsetY, float scaleX, float scaleY)
{
	for (int i = 0; i < userLength_; ++i)
	{
		users_[i]->debugDraw(offsetX, offsetY, scaleX, scaleY);
	}
}

//--------------------------------------------------------------
//GETTER
xn::UserGenerator *xtionni::UserManager::getGenerator() { return generator_; }

xtionni::TrackedUser **xtionni::UserManager::getUsers() { return users_; };
unsigned short xtionni::UserManager::getUserLength() { return userLength_; };
unsigned short xtionni::UserManager::getMaxUserLength() { return userMaxLength_; };