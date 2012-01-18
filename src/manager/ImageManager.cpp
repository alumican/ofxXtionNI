/*
Copyright (c) 2012 Yukiya Okuda
See LICENSE for full license information.
*/

#include "ImageManager.h"
#include "XtionNI.h"

//--------------------------------------------------------------
xtionni::ImageManager::ImageManager(void)
{
	generator_ = NULL;
	pixels_ = NULL;
	bitmap_ = NULL;
}

//--------------------------------------------------------------
xtionni::ImageManager::~ImageManager(void)
{
	finalize();
}

//--------------------------------------------------------------
bool xtionni::ImageManager::initialize(xtionni::XtionNI *xtion)
{
	return xtionni::BaseManager::initialize(xtion);
}

bool xtionni::ImageManager::update()
{
	return xtionni::BaseManager::update();
}

bool xtionni::ImageManager::finalize()
{
	return xtionni::BaseManager::finalize();
}

//--------------------------------------------------------------
bool xtionni::ImageManager::initialize_()
{
	isBitmapUpdated_ = false;

	generator_ = new xn::ImageGenerator();
	XnStatus status = xtion_->getContext()->FindExistingNode(XN_NODE_TYPE_IMAGE, *generator_);
	xtionni::Util::checkError(status, "Find image generator");

	XnMapOutputMode mapMode;
	generator_->GetMapOutputMode(mapMode);

	bitmap_ = new ofxCvColorImage();
	bitmap_->allocate(mapMode.nXRes, mapMode.nYRes);

	ofLog(OF_LOG_NOTICE) << "Initialize Image";
	return true;
}

//--------------------------------------------------------------
bool xtionni::ImageManager::update_()
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
bool xtionni::ImageManager::finalize_()
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
void xtionni::ImageManager::updateBitmap_()
{
	if (!isBitmapUpdated_)
	{
		isBitmapUpdated_ = true;

		xn::ImageMetaData metaData;
		generator_->GetMetaData(metaData);
		bitmap_->setFromPixels((unsigned char*)metaData.RGB24Data(), bitmap_->width, bitmap_->height);
	}
}

//--------------------------------------------------------------
//GETTER
xn::ImageGenerator *xtionni::ImageManager::getGenerator() { return generator_; }

unsigned char *xtionni::ImageManager::getPixels() { updateBitmap_(); return pixels_; }
bool xtionni::ImageManager::isPixelsUpdated() { return isBitmapUpdated_; }

ofxCvColorImage *xtionni::ImageManager::getBitmap() { updateBitmap_(); return bitmap_; }
bool xtionni::ImageManager::isBitmapUpdated() { return isBitmapUpdated_; }