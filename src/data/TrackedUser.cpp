/*
Copyright (c) 2012 Yukiya Okuda
See LICENSE.txt for full license information.
*/

#include "TrackedUser.h"

//--------------------------------------------------------------
xtionni::TrackedUser::TrackedUser(int index)
{
	index_ = index;
	userID_ = -1;

	isTracking_ = false;
	isCalibrating_ = false;
	isCalibrated_ = false;

	center_ = new ofPoint();
	projectiveCenter_ = new ofPoint();

	xnCenter_ = new XnPoint3D();
	xnProjectiveCenter_ = new XnPoint3D();

	limbs_[xtionni::LIMB_NECK]              = limbNeck_            = new TrackedLimb(index_, xtionni::LIMB_NECK);
	limbs_[xtionni::LIMB_LEFT_SHOULDER]     = limbLeftShoulder_    = new TrackedLimb(index_, xtionni::LIMB_LEFT_SHOULDER);
	limbs_[xtionni::LIMB_LEFT_UPPER_ARM]    = limbLeftUpperArm_    = new TrackedLimb(index_, xtionni::LIMB_LEFT_UPPER_ARM);
	limbs_[xtionni::LIMB_LEFT_LOWER_ARM]    = limbLeftLowerArm_    = new TrackedLimb(index_, xtionni::LIMB_LEFT_LOWER_ARM);
	limbs_[xtionni::LIMB_RIGHT_SHOULDER]    = limbRightShoulder_   = new TrackedLimb(index_, xtionni::LIMB_RIGHT_SHOULDER);
	limbs_[xtionni::LIMB_RIGHT_UPPER_ARM]   = limbRightUpperArm_   = new TrackedLimb(index_, xtionni::LIMB_RIGHT_UPPER_ARM);
	limbs_[xtionni::LIMB_RIGHT_LOWER_ARM]   = limbRightLowerArm_   = new TrackedLimb(index_, xtionni::LIMB_RIGHT_LOWER_ARM);
	limbs_[xtionni::LIMB_LEFT_UPPER_TORSO]  = limbLeftUpperTorso_  = new TrackedLimb(index_, xtionni::LIMB_LEFT_UPPER_TORSO);
	limbs_[xtionni::LIMB_RIGHT_UPPER_TORSO] = limbRightUpperTorso_ = new TrackedLimb(index_, xtionni::LIMB_RIGHT_UPPER_TORSO);
	limbs_[xtionni::LIMB_LEFT_LOWER_TORSO]  = limbLeftLowerTorso_  = new TrackedLimb(index_, xtionni::LIMB_LEFT_LOWER_TORSO);
	limbs_[xtionni::LIMB_LEFT_UPPER_LEG]    = limbLeftUpperLeg_    = new TrackedLimb(index_, xtionni::LIMB_LEFT_UPPER_LEG);
	limbs_[xtionni::LIMB_LEFT_LOWER_LEG]    = limbLeftLowerLeg_    = new TrackedLimb(index_, xtionni::LIMB_LEFT_LOWER_LEG);
	limbs_[xtionni::LIMB_RIGHT_LOWER_TORSO] = limbRightLowerTorso_ = new TrackedLimb(index_, xtionni::LIMB_RIGHT_LOWER_TORSO);
	limbs_[xtionni::LIMB_RIGHT_UPPER_LEG]   = limbRightUpperLeg_   = new TrackedLimb(index_, xtionni::LIMB_RIGHT_UPPER_LEG);
	limbs_[xtionni::LIMB_RIGHT_LOWER_LEG]   = limbRightLowerLeg_   = new TrackedLimb(index_, xtionni::LIMB_RIGHT_LOWER_LEG);
	limbs_[xtionni::LIMB_HIP]               = limbHip_             = new TrackedLimb(index_, xtionni::LIMB_HIP);
}

//--------------------------------------------------------------
xtionni::TrackedUser::~TrackedUser(void)
{
	userGenerator_ = NULL;
	depthGenerator_ = NULL;

	delete center_;
	center_ = NULL;

	delete projectiveCenter_;
	projectiveCenter_ = NULL;

	delete xnCenter_;
	xnCenter_ = NULL;

	delete xnProjectiveCenter_;
	xnProjectiveCenter_ = NULL;

	for (std::map<xtionni::SkeletonLimb, TrackedLimb*>::iterator it = limbs_.begin(); it != limbs_.end(); ++it)
	{
		TrackedLimb *limb = (*it).second;
		delete limb;
	}
	limbNeck_ = NULL;
	limbLeftShoulder_ = NULL;
	limbLeftUpperArm_ = NULL;
	limbLeftLowerArm_ = NULL;
	limbRightShoulder_ = NULL;
	limbRightUpperArm_ = NULL;
	limbRightLowerArm_ = NULL;
	limbLeftUpperTorso_ = NULL;
	limbRightUpperTorso_ = NULL;
	limbLeftLowerTorso_ = NULL;
	limbLeftUpperLeg_ = NULL;
	limbLeftLowerLeg_ = NULL;
	limbRightLowerTorso_ = NULL;
	limbRightUpperLeg_ = NULL;
	limbRightLowerLeg_ = NULL;
	limbHip_ = NULL;
}

//--------------------------------------------------------------
void xtionni::TrackedUser::update(xn::UserGenerator *userGenerator, xn::DepthGenerator *depthGenerator, int userID)
{
	userID_ = userID;
	if (userID != -1)
	{
		userGenerator_ = userGenerator;
		depthGenerator_ = depthGenerator;

		isTracking_ = userGenerator->GetSkeletonCap().IsTracking(userID);
		isCalibrating_ = userGenerator->GetSkeletonCap().IsCalibrating(userID);
		isCalibrated_ = userGenerator->GetSkeletonCap().IsCalibrated(userID);

		userGenerator->GetCoM(userID, *xnCenter_);
		depthGenerator->ConvertRealWorldToProjective(1, xnCenter_, xnProjectiveCenter_);
		center_->set(xnCenter_->X, xnCenter_->Y, xnCenter_->Z);
		projectiveCenter_->set(xnProjectiveCenter_->X, xnProjectiveCenter_->Y, xnProjectiveCenter_->Z);

		for (std::map<xtionni::SkeletonLimb, TrackedLimb*>::iterator it = limbs_.begin(); it != limbs_.end(); ++it)
		{
			TrackedLimb *limb = (*it).second;
			limb->update(userGenerator_, depthGenerator_, userID_);
		}
	}
	else
	{
		isTracking_ = false;
		isCalibrating_ = false;
		isCalibrated_ = false;
	}
}

//--------------------------------------------------------------
void xtionni::TrackedUser::debugDraw(float offsetX, float offsetY, float scaleX, float scaleY)
{
	if (userID_ == -1 || !userGenerator_ || !depthGenerator_) return;

	ofPushStyle();
	ofSetColor(*xtionni::Util::getColor(userID_));

	string label = userID_ + (isTracking_ ? " - Tracking" : isCalibrating_ ? " - Calibrating" : " - Looking for pose");
	ofDrawBitmapString(label, projectiveCenter_->x * scaleX + offsetX, projectiveCenter_->y * scaleY + offsetY);

	if (isTracking_)
	{
		for (std::map<xtionni::SkeletonLimb, TrackedLimb*>::iterator it = limbs_.begin(); it != limbs_.end(); ++it)
		{
			TrackedLimb *limb = (*it).second;
			limb->debugDraw(offsetX, offsetY, scaleX, scaleY);
		}
	}

	ofPopStyle();
}

//--------------------------------------------------------------
//GETTER
int xtionni::TrackedUser::getIndex() { return index_; }
int xtionni::TrackedUser::getUserID() { return userID_; }

bool xtionni::TrackedUser::isTracking() { return isTracking_; }
bool xtionni::TrackedUser::isCalibrating() { return isCalibrating_; }
bool xtionni::TrackedUser::isCalibrated() { return isCalibrated_; }

ofPoint *xtionni::TrackedUser::getCenter() { return center_; }
ofPoint *xtionni::TrackedUser::getProjectiveCenter() { return projectiveCenter_; }

XnPoint3D *xtionni::TrackedUser::getXnCenter() { return xnCenter_; }
XnPoint3D *xtionni::TrackedUser::getXnProjectiveCenter() { return xnProjectiveCenter_; }

std::map<xtionni::SkeletonLimb, xtionni::TrackedLimb*> xtionni::TrackedUser::getLlimbs() { return limbs_; }
xtionni::TrackedLimb *xtionni::TrackedUser::getLlimb(xtionni::SkeletonLimb position) { return limbs_[position]; }

xtionni::TrackedLimb *xtionni::TrackedUser::getLimbNeck()            { return limbNeck_;            }
xtionni::TrackedLimb *xtionni::TrackedUser::getLimbLeftShoulder()    { return limbLeftShoulder_;    }
xtionni::TrackedLimb *xtionni::TrackedUser::getLimbUpperArm()        { return limbLeftUpperArm_;    }
xtionni::TrackedLimb *xtionni::TrackedUser::getLimbLowerArm()        { return limbLeftLowerArm_;    }
xtionni::TrackedLimb *xtionni::TrackedUser::getLimbRightShoulder()   { return limbRightShoulder_;   }
xtionni::TrackedLimb *xtionni::TrackedUser::getLimbRightUpperArm()   { return limbRightUpperArm_;   }
xtionni::TrackedLimb *xtionni::TrackedUser::getLimbRightLowerArm()   { return limbRightLowerArm_;   }
xtionni::TrackedLimb *xtionni::TrackedUser::getLimbLeftUpperTorso()  { return limbLeftUpperTorso_;  }
xtionni::TrackedLimb *xtionni::TrackedUser::getLimbRightUpperTorso() { return limbRightUpperTorso_; }
xtionni::TrackedLimb *xtionni::TrackedUser::getLimbLeftLowerTorso()  { return limbLeftLowerTorso_;  }
xtionni::TrackedLimb *xtionni::TrackedUser::getLimbLeftUpperLeg()    { return limbLeftUpperLeg_;    }
xtionni::TrackedLimb *xtionni::TrackedUser::getLimbLeftLowerLeg()    { return limbLeftLowerLeg_;    }
xtionni::TrackedLimb *xtionni::TrackedUser::getLimbRightLowerTorso() { return limbRightLowerTorso_; }
xtionni::TrackedLimb *xtionni::TrackedUser::getLimbRightUpperLeg()   { return limbRightUpperLeg_;   }
xtionni::TrackedLimb *xtionni::TrackedUser::getLimbRightLowerLeg()   { return limbRightLowerLeg_;   }
xtionni::TrackedLimb *xtionni::TrackedUser::getLimbHip()             { return limbHip_;             }