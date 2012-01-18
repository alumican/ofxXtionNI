/*
Copyright (c) 2012 Yukiya Okuda
See LICENSE for full license information.
*/

#pragma once
#ifndef __XTIONNI_BASEMANAGER_H__ 
#define __XTIONNI_BASEMANAGER_H__

#include <iostream>
#include "ofMain.h"
#include "XnCppWrapper.h"
#include "util/Util.h"

namespace xtionni
{
	class XtionNI;

	class BaseManager
	{
		public:
			BaseManager(void);
			~BaseManager(void);

			//----------------------------------------
			virtual bool initialize(xtionni::XtionNI *xtion);
			virtual bool update();
			virtual bool finalize();

			bool isInitialized();
			bool isUpdated();

		private:
			//----------------------------------------
			bool isInitialized_;
			bool isUpdated_;

		protected:
			//----------------------------------------
			virtual bool initialize_();
			virtual bool update_();
			virtual bool finalize_();

			//----------------------------------------
			xtionni::XtionNI *xtion_;
	};
}

#endif