/*
Copyright (c) 2012 Yukiya Okuda
See LICENSE.txt for full license information.
*/

#pragma once
#ifndef __XTIONNI_GESTURE_H__ 
#define __XTIONNI_GESTURE_H__

#include "ofMain.h"
#include "XnCppWrapper.h"
#include "util/Util.h"
#include "variable/Constant.h"

namespace xtionni
{
	class Gesture
	{
		public:
			Gesture(void);
			~Gesture(void);

			//----------------------------------------
			void reset(string name, xtionni::EventType type, float x, float y, float z, float progress, int timestamp);

			string getName();
			xtionni::EventType getType();
			float getProgress();
			ofPoint *getPosition();
			int getTimestamp();

		private:
			//----------------------------------------
			string name_;
			xtionni::EventType type_;
			float progress_;
			ofPoint *position_;
			int timestamp_;
			
			//----------------------------------------
	};
}

#endif