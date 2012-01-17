/*
Copyright (c) 2012 Yukiya Okuda
See LICENSE.txt for full license information.
*/

#pragma once
#ifndef __XTIONNI_HANDSMANAGER_H__ 
#define __XTIONNI_HANDSMANAGER_H__

#include "BaseManager.h"

namespace xtionni
{
	class HandsManager : BaseManager
	{
		public:
			HandsManager(void);
			~HandsManager(void);

			//----------------------------------------
			bool initialize(xtionni::XtionNI *xtion);
			bool update();
			bool finalize();

			xn::HandsGenerator *getGenerator();

		private:
			//----------------------------------------
			bool initialize_();
			bool update_();
			bool finalize_();

			//----------------------------------------
			xn::HandsGenerator *generator_;
	};
}

#endif