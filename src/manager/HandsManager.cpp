/*
Copyright (c) 2012 Yukiya Okuda
See LICENSE.txt for full license information.
*/

#include "HandsManager.h"
#include "XtionNI.h"

//--------------------------------------------------------------
xtionni::HandsManager::HandsManager(void)
{
	generator_ = NULL;
}

//--------------------------------------------------------------
xtionni::HandsManager::~HandsManager(void)
{
	finalize();
}

//--------------------------------------------------------------
bool xtionni::HandsManager::initialize(xtionni::XtionNI *xtion)
{
	return xtionni::BaseManager::initialize(xtion);
}

bool xtionni::HandsManager::update()
{
	return xtionni::BaseManager::update();
}

bool xtionni::HandsManager::finalize()
{
	return xtionni::BaseManager::finalize();
}

//--------------------------------------------------------------
bool xtionni::HandsManager::initialize_()
{
	generator_ = new xn::HandsGenerator();
	XnStatus status = xtion_->getContext()->FindExistingNode(XN_NODE_TYPE_HANDS, *generator_);
	xtionni::Util::checkError(status,"Find hands generator");

	ofLog(OF_LOG_NOTICE) << "Initialize Hands";
	return true;
}

//--------------------------------------------------------------
bool xtionni::HandsManager::update_()
{
	if (generator_->IsNewDataAvailable())
	{
		generator_->WaitAndUpdateData();
		return true;
	}
	return false;
}

//--------------------------------------------------------------
bool xtionni::HandsManager::finalize_()
{
	if (generator_ != NULL)
	{
		generator_->Unref();
		generator_->Release();
		delete generator_;
		generator_ = NULL;
	}

	return true;
}

//--------------------------------------------------------------
//GETTER
xn::HandsGenerator *xtionni::HandsManager::getGenerator() { return generator_; }