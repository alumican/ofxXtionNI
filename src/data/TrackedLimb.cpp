/*
Copyright (c) 2012 Yukiya Okuda
See LICENSE for full license information.
*/

#include "TrackedLimb.h"

//--------------------------------------------------------------
xtionni::TrackedLimb::TrackedLimb(int index, xtionni::SkeletonLimb limbType)
{
	index_ = index;
	limbType_ = limbType;

	jointTypes_ = new XnSkeletonJoint[2];
	switch (limbType_)
	{
		case xtionni::LIMB_NECK:
			jointTypes_[0] = XN_SKEL_HEAD;
			jointTypes_[1] = XN_SKEL_NECK;
			break;

		case xtionni::LIMB_LEFT_SHOULDER:
			jointTypes_[0] = XN_SKEL_NECK;
			jointTypes_[1] = XN_SKEL_LEFT_SHOULDER;
			break;

		case xtionni::LIMB_LEFT_UPPER_ARM:
			jointTypes_[0] = XN_SKEL_LEFT_SHOULDER;
			jointTypes_[1] = XN_SKEL_LEFT_ELBOW;
			break;

		case xtionni::LIMB_LEFT_LOWER_ARM:
			jointTypes_[0] = XN_SKEL_LEFT_ELBOW;
			jointTypes_[1] = XN_SKEL_LEFT_HAND;
			break;

		case xtionni::LIMB_RIGHT_SHOULDER:
			jointTypes_[0] = XN_SKEL_NECK;
			jointTypes_[1] = XN_SKEL_RIGHT_SHOULDER;
			break;

		case xtionni::LIMB_RIGHT_UPPER_ARM:
			jointTypes_[0] = XN_SKEL_RIGHT_SHOULDER;
			jointTypes_[1] = XN_SKEL_RIGHT_ELBOW;
			break;

		case xtionni::LIMB_RIGHT_LOWER_ARM:
			jointTypes_[0] = XN_SKEL_RIGHT_ELBOW;
			jointTypes_[1] = XN_SKEL_RIGHT_HAND;
			break;

		case xtionni::LIMB_LEFT_UPPER_TORSO:
			jointTypes_[0] = XN_SKEL_LEFT_SHOULDER;
			jointTypes_[1] = XN_SKEL_TORSO;
			break;

		case xtionni::LIMB_RIGHT_UPPER_TORSO:
			jointTypes_[0] = XN_SKEL_RIGHT_SHOULDER;
			jointTypes_[1] = XN_SKEL_TORSO;
			break;

		case xtionni::LIMB_LEFT_LOWER_TORSO:
			jointTypes_[0] = XN_SKEL_TORSO;
			jointTypes_[1] = XN_SKEL_LEFT_HIP;
			break;

		case xtionni::LIMB_LEFT_UPPER_LEG:
			jointTypes_[0] = XN_SKEL_LEFT_HIP;
			jointTypes_[1] = XN_SKEL_LEFT_KNEE;
			break;

		case xtionni::LIMB_LEFT_LOWER_LEG:
			jointTypes_[0] = XN_SKEL_LEFT_KNEE;
			jointTypes_[1] = XN_SKEL_LEFT_FOOT;
			break;

		case xtionni::LIMB_RIGHT_LOWER_TORSO:
			jointTypes_[0] = XN_SKEL_TORSO;
			jointTypes_[1] = XN_SKEL_RIGHT_HIP;
			break;

		case xtionni::LIMB_RIGHT_UPPER_LEG:
			jointTypes_[0] = XN_SKEL_RIGHT_HIP;
			jointTypes_[1] = XN_SKEL_RIGHT_KNEE;
			break;

		case xtionni::LIMB_RIGHT_LOWER_LEG:
			jointTypes_[0] = XN_SKEL_RIGHT_KNEE;
			jointTypes_[1] = XN_SKEL_RIGHT_FOOT;
			break;

		case xtionni::LIMB_HIP:
			jointTypes_[0] = XN_SKEL_LEFT_HIP;
			jointTypes_[1] = XN_SKEL_RIGHT_HIP;
			break;
	}

	isTracking_ = false;

	jointPositions_ = new ofPoint*[2];
	jointPositions_[0] = new ofPoint();
	jointPositions_[1] = new ofPoint();

	projectiveJointPositions_ = new ofPoint*[2];
	projectiveJointPositions_[0] = new ofPoint();
	projectiveJointPositions_[1] = new ofPoint();

	jointConfidences_ = new float[2];
	jointConfidences_[0] = 0;
	jointConfidences_[1] = 0;

	confidenceThreshold_ = 0.5;
}

//--------------------------------------------------------------
xtionni::TrackedLimb::~TrackedLimb(void)
{
	userGenerator_ = NULL;
	depthGenerator_ = NULL;

	delete jointPositions_[0];
	delete jointPositions_[1];
	delete[] jointPositions_;
	jointPositions_ = NULL;

	delete projectiveJointPositions_[0];
	delete projectiveJointPositions_[1];
	delete[] projectiveJointPositions_;
	projectiveJointPositions_ = NULL;

	delete[] jointTypes_;
	jointTypes_ = NULL;

	delete[] jointConfidences_;
	jointConfidences_ = NULL;
}

//--------------------------------------------------------------
void xtionni::TrackedLimb::update(xn::UserGenerator *userGenerator, xn::DepthGenerator *depthGenerator, int userID)
{
	userID_ = userID;
	if (userID_ != -1)
	{
		userGenerator_ = userGenerator;
		depthGenerator_ = depthGenerator;

		XnSkeletonJointPosition p0, p1;
		userGenerator_->GetSkeletonCap().GetSkeletonJointPosition(userID_, jointTypes_[0], p0);
		userGenerator_->GetSkeletonCap().GetSkeletonJointPosition(userID_, jointTypes_[1], p1);

		jointConfidences_[0] = p0.fConfidence;
		jointConfidences_[1] = p1.fConfidence;

		isTracking_ = jointConfidences_[0] >= confidenceThreshold_ && jointConfidences_[1] >= confidenceThreshold_;

		if (isTracking_)
		{
			jointPositions_[0]->x = p0.position.X;
			jointPositions_[0]->y = p0.position.Y;
			jointPositions_[0]->z = p0.position.Z;

			jointPositions_[1]->x = p1.position.X;
			jointPositions_[1]->y = p1.position.Y;
			jointPositions_[1]->z = p1.position.Z;

			XnPoint3D pt[2];
			pt[0] = p0.position;
			pt[1] = p1.position;
			depthGenerator_->ConvertRealWorldToProjective(2, pt, pt);

			projectiveJointPositions_[0]->x = pt[0].X;
			projectiveJointPositions_[0]->y = pt[0].Y;
			projectiveJointPositions_[0]->z = pt[0].Z;

			projectiveJointPositions_[1]->x = pt[1].X;
			projectiveJointPositions_[1]->y = pt[1].Y;
			projectiveJointPositions_[1]->z = pt[1].Z;
		}
	}
	else
	{
		isTracking_ = false;
	}
}

//--------------------------------------------------------------
void xtionni::TrackedLimb::debugDraw(float offsetX, float offsetY, float scaleX, float scaleY)
{
	if (!isTracking_ || !userGenerator_ || !depthGenerator_) return;

	ofLine(
		projectiveJointPositions_[0]->x * scaleX + offsetX,
		projectiveJointPositions_[0]->y * scaleY + offsetY,
		projectiveJointPositions_[1]->x * scaleX + offsetX,
		projectiveJointPositions_[1]->y * scaleY + offsetY
	);
}

//--------------------------------------------------------------
void xtionni::TrackedLimb::setConfidenceThreshold(float threshold)
{
	confidenceThreshold_ = threshold;
}

//--------------------------------------------------------------
//GETTER
int xtionni::TrackedLimb::getIndex() { return index_; }
int xtionni::TrackedLimb::getUserID() { return userID_; }

xtionni::SkeletonLimb xtionni::TrackedLimb::getLimbType() { return limbType_; }
XnSkeletonJoint *xtionni::TrackedLimb::getJointTypes() { return jointTypes_; }

bool xtionni::TrackedLimb::isTracking() { return isTracking_; }

ofPoint **xtionni::TrackedLimb::getJointPositions() { return jointPositions_; }
ofPoint **xtionni::TrackedLimb::getProjectiveJointPositions() { return projectiveJointPositions_; }
float *xtionni::TrackedLimb::getJointConfidences() { return jointConfidences_; }

float xtionni::TrackedLimb::getConfidenceThreshold() { return confidenceThreshold_; }