/*
Copyright (c) 2012 Yukiya Okuda
See LICENSE.txt for full license information.
*/

#include "IRManager.h"
#include "XtionNI.h"

//--------------------------------------------------------------
xtionni::IRManager::IRManager(void)
{
	generator_ = NULL;
	pixels_ = NULL;
	bitmap_ = NULL;
}

//--------------------------------------------------------------
xtionni::IRManager::~IRManager(void)
{
	finalize();
}

//--------------------------------------------------------------
bool xtionni::IRManager::initialize(xtionni::XtionNI *xtion)
{
	return xtionni::BaseManager::initialize(xtion);
}

bool xtionni::IRManager::update()
{
	return xtionni::BaseManager::update();
}

bool xtionni::IRManager::finalize()
{
	return xtionni::BaseManager::finalize();
}

//--------------------------------------------------------------
bool xtionni::IRManager::initialize_()
{
	isBitmapUpdated_ = false;

	minValue_ = 0.0;
	maxValue_ = 255.0 / USHRT_MAX;

	generator_ = new xn::IRGenerator();
	XnStatus status = xtion_->getContext()->FindExistingNode(XN_NODE_TYPE_IR, *generator_);
	xtionni::Util::checkError(status, "Find IR generator");

	XnMapOutputMode mapMode;
	generator_->GetMapOutputMode(mapMode);

	int w = mapMode.nXRes;
	int h = mapMode.nYRes;
	int n = w * h;

	pixels_ = new unsigned char[n];
	xnOSMemSet(pixels_, 0, n * sizeof(unsigned char));

	bitmap_ = new ofxCvGrayscaleImage();
	bitmap_->allocate(w, h);

	ofLog(OF_LOG_NOTICE) << "initialized IR";
	return true;
}

//--------------------------------------------------------------
bool xtionni::IRManager::update_()
{
	if (generator_->IsNewDataAvailable())
	{
		isBitmapUpdated_ = false;
		generator_->WaitAndUpdateData();
		return true;
	}
	return false;
}

//--------------------------------------------------------------
bool xtionni::IRManager::finalize_()
{
	if (generator_ != NULL)
	{
		generator_->Unref();
		generator_->Release();
		delete generator_;
		generator_ = NULL;
	}

	if (pixels_ != NULL)
	{
		delete[] pixels_;
		pixels_ = NULL;
	}

	if (bitmap_ != NULL)
	{
		bitmap_->clear();
		delete bitmap_;
		bitmap_ = NULL;
	}

	return true;
}

//--------------------------------------------------------------
void xtionni::IRManager::updateBitmap_()
{
	if (!isBitmapUpdated_)
	{
		isBitmapUpdated_ = true;

		xn::IRMetaData metaData;
		generator_->GetMetaData(metaData);

		XnMapOutputMode mapMode;
		generator_->GetMapOutputMode(mapMode);

		int w = mapMode.nXRes;
		int h = mapMode.nYRes;
		int n = w * h;

		const XnIRPixel* pixels = metaData.Data();
		
		float range = maxValue_ - minValue_;
		for(int i = 0; i < n; ++i)
		{
			float ratio = (float)pixels[i] / USHRT_MAX;
			if (ratio > 0.0)
			{
				ratio = (ratio <= minValue_) ? 0.0 : (ratio >= maxValue_) ? 1.0 : ((ratio - minValue_) / range);
				pixels_[i] = (unsigned char)(ratio * UCHAR_MAX);
			}
			else
			{
				pixels_[i] = 0.0;
			}
		}

		bitmap_->setFromPixels(pixels_, w, h);
	}
}

//--------------------------------------------------------------
void xtionni::IRManager::setValueRange(float minValue, float maxValue)
{
	minValue_ = minValue;
	maxValue_ = maxValue;
}

//--------------------------------------------------------------
//GETTER
xn::IRGenerator *xtionni::IRManager::getGenerator() { return generator_; }

unsigned char *xtionni::IRManager::getPixels() { updateBitmap_(); return pixels_; }
bool xtionni::IRManager::isPixelsUpdated() { return isBitmapUpdated_; }

ofxCvGrayscaleImage *xtionni::IRManager::getBitmap() { updateBitmap_(); return bitmap_; }
bool xtionni::IRManager::isBitmapUpdated() { return isBitmapUpdated_; }

float xtionni::IRManager::getMinValue() { return minValue_; };
float xtionni::IRManager::getMaxValue() { return maxValue_; };