/*
Copyright (c) 2012 Yukiya Okuda
See LICENSE.txt for full license information.
*/

#include "DepthManager.h"
#include "XtionNI.h"

#define MAX_DEPTH 10000

//--------------------------------------------------------------
xtionni::DepthManager::DepthManager(void)
{
	generator_ = NULL;
	pixels_ = NULL;
	bitmap_ = NULL;
}

//--------------------------------------------------------------
xtionni::DepthManager::~DepthManager(void)
{
	finalize();
}

//--------------------------------------------------------------
bool xtionni::DepthManager::initialize(xtionni::XtionNI *xtion)
{
	return xtionni::BaseManager::initialize(xtion);
}

bool xtionni::DepthManager::update()
{
	return xtionni::BaseManager::update();
}

bool xtionni::DepthManager::finalize()
{
	return xtionni::BaseManager::finalize();
}

//--------------------------------------------------------------
bool xtionni::DepthManager::initialize_()
{
	isBitmapUpdated_ = false;

	minDepth_ = 500.0 / USHRT_MAX;
	maxDepth_ = 3000.0 / USHRT_MAX;

	generator_ = new xn::DepthGenerator();
	XnStatus status = xtion_->getContext()->FindExistingNode(XN_NODE_TYPE_DEPTH, *generator_);
	xtionni::Util::checkError(status, "Find depth generator");

	XnMapOutputMode mapMode;
	generator_->GetMapOutputMode(mapMode);

	int w = mapMode.nXRes;
	int h = mapMode.nYRes;
	int n = w * h;

	pixels_ = new unsigned char[n];
	xnOSMemSet(pixels_, 0, n * sizeof(unsigned char));

	bitmap_ = new ofxCvGrayscaleImage();
	bitmap_->allocate(w, h);

	ofLog(OF_LOG_NOTICE) << "Initialize Depth";
	return true;
}

//--------------------------------------------------------------
bool xtionni::DepthManager::update_()
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
bool xtionni::DepthManager::finalize_()
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
void xtionni::DepthManager::updateBitmap_()
{
	if (!isBitmapUpdated_)
	{
		isBitmapUpdated_ = true;

		xn::DepthMetaData metaData;
		generator_->GetMetaData(metaData);

		XnMapOutputMode mapMode;
		generator_->GetMapOutputMode(mapMode);

		int w = mapMode.nXRes;
		int h = mapMode.nYRes;
		int n = w * h;

		const XnDepthPixel* pixels = metaData.Data();

		float range = maxDepth_ - minDepth_;
		for(int i = 0; i < n; ++i)
		{
			float ratio = (float)pixels[i] / USHRT_MAX;
			if (ratio > 0.0)
			{
				ratio = (ratio <= minDepth_) ? 0.0 : (ratio >= maxDepth_) ? 1.0 : ((ratio - minDepth_) / range);
				pixels_[i] = (unsigned char)((1.0 - ratio) * UCHAR_MAX);
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
void xtionni::DepthManager::setDepthRange(float minDepth, float maxDepth)
{
	minDepth_ = minDepth;
	maxDepth_ = maxDepth;
}

//--------------------------------------------------------------
//GETTER
xn::DepthGenerator *xtionni::DepthManager::getGenerator() { return generator_; }

unsigned char *xtionni::DepthManager::getPixels() { updateBitmap_(); return pixels_; }
bool xtionni::DepthManager::isPixelsUpdated() { return isBitmapUpdated_; }

ofxCvGrayscaleImage *xtionni::DepthManager::getBitmap() { updateBitmap_(); return bitmap_; }
bool xtionni::DepthManager::isBitmapUpdated() { return isBitmapUpdated_; }

float xtionni::DepthManager::getMinDepth() { return minDepth_; };
float xtionni::DepthManager::getMaxDepth() { return maxDepth_; };