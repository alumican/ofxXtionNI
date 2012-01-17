/*
Copyright (c) 2012 Yukiya Okuda
See LICENSE.txt for full license information.
*/

#pragma once
#ifndef __XTIONNI_UTIL_H__ 
#define __XTIONNI_UTIL_H__

#include <iostream>
#include "ofMain.h"
#include "XnCppWrapper.h"
#include "variable/Constant.h"

namespace xtionni
{
	class Util
	{
		public:
			Util(void);
			~Util(void);

			static void log(ostream &message, ofLogLevel level);

			static void checkError(XnStatus status, string name);
			static void checkError(XnStatus status, string name, xn::EnumerationErrors *errors);

			static string getErrorString(XnCalibrationStatus error);
			static string getErrorString(XnPoseDetectionStatus error);

			static int getColorLength();
			static ofColor *getColor(int index);

			static string getGestureName(xtionni::GestureType gestureType);
			static xtionni::GestureType getGestureType(string gestureName);

		private:
			static ofColor *colors_[];
			static int colorLength_;
	};
}

#endif