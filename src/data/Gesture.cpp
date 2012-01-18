/*
Copyright (c) 2012 Yukiya Okuda
See LICENSE for full license information.
*/

#include "Gesture.h"

//--------------------------------------------------------------
xtionni::Gesture::Gesture(void)
{
	position_ = new ofPoint();
	reset("", xtionni::GESTURE_RECOGNIZED, 0, 0, 0, 0, 0);
}

//--------------------------------------------------------------
xtionni::Gesture::~Gesture(void)
{
	delete position_;
	position_ = NULL;
}

//--------------------------------------------------------------
void xtionni::Gesture::reset(string name, xtionni::EventType type, float x, float y, float z, float progress, int timestamp)
{
	name_ = name;
	type_ = type;
	position_->x = x;
	position_->y = y;
	position_->z = z;
	progress_ = progress;
	timestamp_ = timestamp;
}

//----------------------------------------
string xtionni::Gesture::getName() { return name_; };
xtionni::EventType xtionni::Gesture::getType() { return type_; };
float xtionni::Gesture::getProgress() { return progress_; };
ofPoint *xtionni::Gesture::getPosition() { return position_; };
int xtionni::Gesture::getTimestamp() { return timestamp_; };