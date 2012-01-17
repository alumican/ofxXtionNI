/*
Copyright (c) 2012 Yukiya Okuda
See LICENSE.txt for full license information.
*/

#pragma once
#ifndef __XTIONNI_H__ 
#define __XTIONNI_H__

#include <iostream>
#include "ofMain.h"
#include "XnCppWrapper.h"

#include "util/Util.h"
#include "event/EventDispatcher.h"
#include "manager/DepthManager.h"
#include "manager/ImageManager.h"
#include "manager/AudioManager.h"
#include "manager/UserManager.h"
#include "manager/GestureManager.h"
#include "manager/HandsManager.h"
#include "manager/IRManager.h"

namespace xtionni
{
	class XtionNI
	{
		public:
			XtionNI(void);
			~XtionNI(void);

			//----------------------------------------
			//Initialize
			void initialize(
				string configFilePath,
				string recordFilePath = "",
				bool useDepth   = true,
				bool useImage   = true,
				bool useAudio   = false,
				bool useUser    = false,
				bool useGesture = false,
				bool useHands   = false,
				bool useIR      = false
			);
			
			//Update
			void update();

			//Finalize
			void finalize();

			//----------------------------------------
			//OpenNI Context
			xn::Context *getContext();

			//OpenNI Generator
			xtionni::DepthManager   *getDepthManager();
			xtionni::ImageManager   *getImageManager();
			xtionni::AudioManager   *getAudioManager();
			xtionni::UserManager    *getUserManager();
			xtionni::GestureManager *getGestureManager();
			xtionni::HandsManager   *getHandsManager();
			xtionni::IRManager      *getIRManager();

			//Mode Flag
			bool useDepth();
			bool useImage();
			bool useAudio();
			bool useUser();
			bool useGesture();
			bool useHands();
			bool useIR();

			//Record
			void startRecord(string outputDirectory);
			void stopRecord();

			//void dispatchEvent(string eventType);
			//void addEventListener();

		private:
			//----------------------------------------
			void xtionni::XtionNI::checkRecordError_(XnStatus status, string name);

			//----------------------------------------
			xn::Context *context_;

			xtionni::DepthManager   *depthManager_;
			xtionni::ImageManager   *imageManager_;
			xtionni::AudioManager   *audioManager_;
			xtionni::UserManager    *userManager_;
			xtionni::GestureManager *gestureManager_;
			xtionni::HandsManager   *handsManager_;
			xtionni::IRManager      *irManager_;

			//xn::SceneAnalyzer    sceneAnalyzer_;
			//xn::Recorder         recorder_;

			bool useDepth_;
			bool useImage_;
			bool useAudio_;
			bool useUser_;
			bool useGesture_;
			bool useHands_;
			bool useIR_;

			xn::Player   *player_;
			xn::Recorder *recorder_;

			bool isRecording_;
	};
}

#endif