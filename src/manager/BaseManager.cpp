/*
Copyright (c) 2012 Yukiya Okuda
See LICENSE.txt for full license information.
*/

#include "BaseManager.h"

xtionni::BaseManager::BaseManager(void)
{
	isInitialized_ = false;
	xtion_ = NULL;
}

xtionni::BaseManager::~BaseManager(void)
{
}

//--------------------------------------------------------------
bool xtionni::BaseManager::initialize(xtionni::XtionNI *xtion)
{
	if (isInitialized_) return true;
	xtion_ = xtion;
	isUpdated_ = false;
	bool result = initialize_();
	isInitialized_ = result;
	return result;
}

bool xtionni::BaseManager::update()
{
	bool result = update_();
	isUpdated_ = result;
	return result;
}

bool xtionni::BaseManager::finalize()
{
	if (!isInitialized_) return true;
	bool result = finalize_();
	isInitialized_ = !result;
	xtion_ = NULL;
	return result;
}

//--------------------------------------------------------------
bool xtionni::BaseManager::isInitialized()
{
	return isInitialized_;
}

bool xtionni::BaseManager::isUpdated()
{
	return isUpdated_;
}

//--------------------------------------------------------------
bool xtionni::BaseManager::initialize_()
{
	return true;
}

bool xtionni::BaseManager::update_()
{
	return true;
}

bool xtionni::BaseManager::finalize_()
{
	return true;
}