/*
Copyright (c) 2012 Yukiya Okuda
See LICENSE for full license information.
*/

#pragma once
#ifndef __XTIONNI_GESTUREMANAGER_H__ 
#define __XTIONNI_GESTUREMANAGER_H__

#include "BaseManager.h"
#include "data/Gesture.h"

namespace xtionni
{
	class GestureManager : BaseManager
	{
		public:
			GestureManager(void);
			~GestureManager(void);

			//----------------------------------------
			bool initialize(xtionni::XtionNI *xtion);
			bool update();
			bool finalize();

			xn::GestureGenerator *getGenerator();

			void xtionni::GestureManager::addGesture(xtionni::GestureType gestureType);
			void xtionni::GestureManager::removeGesture(xtionni::GestureType gestureType);

			xtionni::Gesture *getLastGesture();

			bool getUseProgress();
			void setUseProgress(bool flag);

			int getMinGestureInterval();
			void setMinGestureInterval(int milliseconds);

			bool isGestureRecognized();
			bool isGestureProgress();

		private:
			//----------------------------------------
			bool initialize_();
			bool update_();
			bool finalize_();

			void XN_CALLBACK_TYPE gestureRecognizedHandler_(xn::GestureGenerator &generator, const XnChar *gestureName, const XnPoint3D *idPosition, const XnPoint3D* endPosition, void* cookie);
			void XN_CALLBACK_TYPE gestureProgressHandler_(xn::GestureGenerator &generator, const XnChar *gestureName, const XnPoint3D *position, XnFloat progress, void *cookie);

			static void XN_CALLBACK_TYPE staticGestureRecognizedHandler_(xn::GestureGenerator &generator, const XnChar *gestureName, const XnPoint3D *idPosition, const XnPoint3D* endPosition, void* cookie);
			static void XN_CALLBACK_TYPE staticGestureProgressHandler_(xn::GestureGenerator &generator, const XnChar *gestureName, const XnPoint3D *position, XnFloat progress, void *cookie);

			//----------------------------------------
			xn::GestureGenerator *generator_;

			xtionni::Gesture *lastGesture_;
			bool useProgress_;
			int minGestureInterval_;

			bool isGestureRecognized_;
			bool isGestureProgress_;
	};
}

#endif