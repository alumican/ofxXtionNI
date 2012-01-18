/*
Copyright (c) 2012 Yukiya Okuda
See LICENSE for full license information.
*/

#pragma once
#ifndef __XTIONNI_IRMANAGER_H__ 
#define __XTIONNI_IRMANAGER_H__

#include "BaseManager.h"
#include "ofxOpenCv.h"

namespace xtionni
{
	class IRManager : BaseManager
	{
		public:
			IRManager(void);
			~IRManager(void);

			//----------------------------------------
			bool initialize(xtionni::XtionNI *xtion);
			bool update();
			bool finalize();

			xn::IRGenerator *getGenerator();

			unsigned char *getPixels();
			bool isPixelsUpdated();

			ofxCvGrayscaleImage *getBitmap();
			bool isBitmapUpdated();

			void setValueRange(float minValue, float maxValue);
			float getMinValue();
			float getMaxValue();

		private:
			//----------------------------------------
			bool initialize_();
			bool update_();
			bool finalize_();

			void updateBitmap_();

			//----------------------------------------
			xn::IRGenerator *generator_;

			unsigned char *pixels_;
			ofxCvGrayscaleImage *bitmap_;
			bool isBitmapUpdated_;

			float minValue_;
			float maxValue_;
	};
}

#endif