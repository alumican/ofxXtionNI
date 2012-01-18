/*
Copyright (c) 2012 Yukiya Okuda
See LICENSE for full license information.
*/

#pragma once
#ifndef __XTIONNI_AUDIOMANAGER_H__ 
#define __XTIONNI_AUDIOMANAGER_H__

#include "BaseManager.h"

namespace xtionni
{
	class AudioManager : BaseManager
	{
		public:
			AudioManager(void);
			~AudioManager(void);

			//----------------------------------------
			bool initialize(xtionni::XtionNI *xtion);
			bool update();
			bool finalize();

			xn::AudioGenerator *getGenerator();

		private:
			//----------------------------------------
			bool initialize_();
			bool update_();
			bool finalize_();

			//----------------------------------------
			xn::AudioGenerator *generator_;
	};
}

#endif