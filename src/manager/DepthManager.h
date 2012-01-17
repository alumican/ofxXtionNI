/*
Copyright (c) 2012 Yukiya Okuda
See LICENSE.txt for full license information.
*/

#pragma once
#ifndef __XTIONNI_DEPTHMANAGER_H__ 
#define __XTIONNI_DEPTHMANAGER_H__

#include "BaseManager.h"
#include "ofxOpenCv.h"

namespace xtionni
{
	class DepthManager : BaseManager
	{
		public:
			DepthManager(void);
			~DepthManager(void);

			//----------------------------------------
			bool initialize(xtionni::XtionNI *xtion);
			bool update();
			bool finalize();

			xn::DepthGenerator *getGenerator();

			unsigned char *getPixels();
			bool isPixelsUpdated();

			ofxCvGrayscaleImage *getBitmap();
			bool isBitmapUpdated();

			void setDepthRange(float minDepth, float maxDepth);
			float getMinDepth();
			float getMaxDepth();

		private:
			//----------------------------------------
			bool initialize_();
			bool update_();
			bool finalize_();

			void updateBitmap_();

			//----------------------------------------
			xn::DepthGenerator *generator_;

			unsigned char *pixels_;
			ofxCvGrayscaleImage *bitmap_;
			bool isBitmapUpdated_;

			float minDepth_;
			float maxDepth_;
	};
}

#endif