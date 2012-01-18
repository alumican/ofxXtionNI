/*
Copyright (c) 2012 Yukiya Okuda
See LICENSE for full license information.
*/

#include "AudioManager.h"
#include "XtionNI.h"

//--------------------------------------------------------------
xtionni::AudioManager::AudioManager(void)
{
	generator_ = NULL;
}

//--------------------------------------------------------------
xtionni::AudioManager::~AudioManager(void)
{
	finalize();
}

//--------------------------------------------------------------
bool xtionni::AudioManager::initialize(xtionni::XtionNI *xtion)
{
	return xtionni::BaseManager::initialize(xtion);
}

bool xtionni::AudioManager::update()
{
	return xtionni::BaseManager::update();
}

bool xtionni::AudioManager::finalize()
{
	return xtionni::BaseManager::finalize();
}

//--------------------------------------------------------------
bool xtionni::AudioManager::initialize_()
{
	generator_ = new xn::AudioGenerator();
	XnStatus status = xtion_->getContext()->FindExistingNode(XN_NODE_TYPE_AUDIO, *generator_);
	xtionni::Util::checkError(status, "Find audio generator");
	
	ofLog(OF_LOG_NOTICE) << "Initialize Audio";
	return true;
}

//--------------------------------------------------------------
bool xtionni::AudioManager::update_()
{
	if (generator_->IsNewDataAvailable())
	{
		generator_->WaitAndUpdateData();
		return true;
	}
	return false;
}

//--------------------------------------------------------------
bool xtionni::AudioManager::finalize_()
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
xn::AudioGenerator *xtionni::AudioManager::getGenerator() { return generator_; }