/*
Copyright (c) 2012 Yukiya Okuda
See LICENSE for full license information.
*/

#pragma once
#ifndef __XTIONNI_IMAGEMANAGER_H__ 
#define __XTIONNI_IMAGEMANAGER_H__

#include "BaseManager.h"
#include "ofxOpenCv.h"

namespace xtionni
{
	class ImageManager : BaseManager
	{
		public:
			ImageManager(void);
			~ImageManager(void);

			//----------------------------------------
			bool initialize(xtionni::XtionNI *xtion);
			bool update();
			bool finalize();

			xn::ImageGenerator *getGenerator();

			unsigned char *getPixels();
			bool isPixelsUpdated();

			ofxCvColorImage *getBitmap();
			bool isBitmapUpdated();

		private:
			//----------------------------------------
			bool initialize_();
			bool update_();
			bool finalize_();

			void updateBitmap_();

			//----------------------------------------
			xn::ImageGenerator *generator_;

			unsigned char *pixels_;
			ofxCvColorImage *bitmap_;
			bool isBitmapUpdated_;
	};
}

#endif