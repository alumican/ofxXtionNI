/*
Copyright (c) 2012 Yukiya Okuda
See LICENSE for full license information.
*/

#pragma once
#ifndef __XTIONNI_USERMANAGER_H__ 
#define __XTIONNI_USERMANAGER_H__

#include <new>
#include "BaseManager.h"
#include "data/TrackedUser.h"

namespace xtionni
{
	class UserManager : BaseManager
	{
		public:
			UserManager(void);
			~UserManager(void);

			//----------------------------------------
			bool initialize(xtionni::XtionNI *xtion);
			bool update();
			bool finalize();

			xn::UserGenerator *getGenerator();

			void drawSkeletons(float offsetX = 0.0, float offsetY = 0.0, float scaleX = 1.0, float scaleY = 1.0);

			TrackedUser **getUsers();
			unsigned short getUserLength();
			unsigned short getMaxUserLength();

		private:
			//----------------------------------------
			bool initialize_();
			bool update_();
			bool finalize_();

			void XN_CALLBACK_TYPE newUserHandler_(xn::UserGenerator &generator, XnUserID userID, void *cookie);
			void XN_CALLBACK_TYPE lostUserHandler_(xn::UserGenerator &generator, XnUserID userID, void *cookie);
			void XN_CALLBACK_TYPE poseDetectedHandler_(xn::PoseDetectionCapability &capability, const XnChar *poseName, XnUserID userID, void *cookie);
			void XN_CALLBACK_TYPE calibrationStartedHandler_(xn::SkeletonCapability &capability, XnUserID userID, void *cookie);
			void XN_CALLBACK_TYPE calibrationCompletedHandler_(xn::SkeletonCapability &capability, XnUserID userID, XnCalibrationStatus status, void *cookie);

			static void XN_CALLBACK_TYPE staticNewUserHandler_(xn::UserGenerator &generator, XnUserID userID, void *cookie);
			static void XN_CALLBACK_TYPE staticLostUserHandler_(xn::UserGenerator &generator, XnUserID userID, void *cookie);
			static void XN_CALLBACK_TYPE staticPoseDetectedHandler_(xn::PoseDetectionCapability &capability, const XnChar *poseName, XnUserID userID, void *cookie);
			static void XN_CALLBACK_TYPE staticCalibrationStartedHandler_(xn::SkeletonCapability &capability, XnUserID userID, void *cookie);
			static void XN_CALLBACK_TYPE staticCalibrationCompletedHandler_(xn::SkeletonCapability &capability, XnUserID userID, XnCalibrationStatus status, void *cookie);

			//----------------------------------------
			xn::UserGenerator *generator_;

			bool isPoseNeeded_;
			XnChar calibrationPoseName_[20];

			unsigned short userMaxLength_;
			unsigned short userLength_;
			TrackedUser **users_;
	};
}

#endif