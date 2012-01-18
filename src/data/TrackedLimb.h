/*
Copyright (c) 2012 Yukiya Okuda
See LICENSE for full license information.
*/

#pragma once
#ifndef __XTIONNI_TRACKEDLIMB_H__ 
#define __XTIONNI_TRACKEDLIMB_H__

#include "ofMain.h"
#include "XnCppWrapper.h"
#include "util/Util.h"
#include "variable/Constant.h"

namespace xtionni
{
	class TrackedLimb
	{
		public:
			TrackedLimb(int index, xtionni::SkeletonLimb limbType);
			~TrackedLimb(void);

			//----------------------------------------
			void update(xn::UserGenerator *userGenerator, xn::DepthGenerator *depthGenerator, int userID);
			void debugDraw(float offsetX = 0.0, float offsetY = 0.0, float scaleX = 1.0, float scaleY = 1.0);

			int getIndex();
			int getUserID();

			xtionni::SkeletonLimb getLimbType(); 
			XnSkeletonJoint *getJointTypes();

			bool isTracking();

			ofPoint **getJointPositions();
			ofPoint **getProjectiveJointPositions();
			float *getJointConfidences();

			float getConfidenceThreshold();
			void setConfidenceThreshold(float threshold);

		private:
			//----------------------------------------
			
			//----------------------------------------
			int index_;
			xtionni::SkeletonLimb limbType_;
			XnSkeletonJoint *jointTypes_;

			int userID_;
			xn::UserGenerator *userGenerator_;
			xn::DepthGenerator *depthGenerator_;
			
			bool isTracking_;

			ofPoint **jointPositions_;
			ofPoint **projectiveJointPositions_;

			float *jointConfidences_;
			float confidenceThreshold_;
	};
}

#endif