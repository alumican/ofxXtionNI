/*
Copyright (c) 2012 Yukiya Okuda
See LICENSE for full license information.
*/

#pragma once
#ifndef __XTIONNI_TRACKEDUSER_H__ 
#define __XTIONNI_TRACKEDUSER_H__

#include "ofMain.h"
#include "XnCppWrapper.h"
#include "TrackedLimb.h"
#include "util/Util.h"
#include "variable/Constant.h"

namespace xtionni
{
	class TrackedUser
	{
		public:
			TrackedUser(int index);
			~TrackedUser(void);

			//----------------------------------------
			void update(xn::UserGenerator *userGenerator, xn::DepthGenerator *depthGenerator, int userID);
			void debugDraw(float offsetX = 0.0, float offsetY = 0.0, float scaleX = 1.0, float scaleY = 1.0);

			int getIndex();
			int getUserID();

			bool isTracking();
			bool isCalibrating();
			bool isCalibrated();

			ofPoint *getCenter();
			ofPoint *getProjectiveCenter();

			XnPoint3D *getXnCenter();
			XnPoint3D *getXnProjectiveCenter();

			std::map<xtionni::SkeletonLimb, TrackedLimb*> getLlimbs();
			TrackedLimb *getLlimb(xtionni::SkeletonLimb position);

			TrackedLimb *getLimbNeck();
			TrackedLimb *getLimbLeftShoulder();
			TrackedLimb *getLimbUpperArm();
			TrackedLimb *getLimbLowerArm();
			TrackedLimb *getLimbRightShoulder();
			TrackedLimb *getLimbRightUpperArm();
			TrackedLimb *getLimbRightLowerArm();
			TrackedLimb *getLimbLeftUpperTorso();
			TrackedLimb *getLimbRightUpperTorso();
			TrackedLimb *getLimbLeftLowerTorso();
			TrackedLimb *getLimbLeftUpperLeg();
			TrackedLimb *getLimbLeftLowerLeg();
			TrackedLimb *getLimbRightLowerTorso();
			TrackedLimb *getLimbRightUpperLeg();
			TrackedLimb *getLimbRightLowerLeg();
			TrackedLimb *getLimbHip();

		private:
			//----------------------------------------
			
			//----------------------------------------
			int index_;
			int userID_;

			bool isTracking_;
			bool isCalibrating_;
			bool isCalibrated_;

			ofPoint *center_;
			ofPoint *projectiveCenter_;

			XnPoint3D *xnCenter_;
			XnPoint3D *xnProjectiveCenter_;

			xn::UserGenerator *userGenerator_;
			xn::DepthGenerator *depthGenerator_;

			//Limb
			std::map<xtionni::SkeletonLimb, TrackedLimb*> limbs_;

			//
			TrackedLimb *limbNeck_;
			TrackedLimb *limbLeftShoulder_;
			TrackedLimb *limbLeftUpperArm_;
			TrackedLimb *limbLeftLowerArm_;
			//
			TrackedLimb *limbRightShoulder_;
			TrackedLimb *limbRightUpperArm_;
			TrackedLimb *limbRightLowerArm_;
			//
			TrackedLimb *limbLeftUpperTorso_;
			TrackedLimb *limbRightUpperTorso_;
			//
			TrackedLimb *limbLeftLowerTorso_;
			TrackedLimb *limbLeftUpperLeg_;
			TrackedLimb *limbLeftLowerLeg_;
			//
			TrackedLimb *limbRightLowerTorso_;
			TrackedLimb *limbRightUpperLeg_;
			TrackedLimb *limbRightLowerLeg_;
			//
			TrackedLimb *limbHip_;
	};
}

#endif