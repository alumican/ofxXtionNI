/*
Copyright (c) 2012 Yukiya Okuda
See LICENSE for full license information.
*/

#pragma once
#ifndef __XTIONNI_CONSTANT_H__ 
#define __XTIONNI_CONSTANT_H__

namespace xtionni
{
	//Limb type
	typedef enum SkeletonLimb {
		LIMB_NECK,
		LIMB_LEFT_SHOULDER,
		LIMB_LEFT_UPPER_ARM,
		LIMB_LEFT_LOWER_ARM,
		LIMB_RIGHT_SHOULDER,
		LIMB_RIGHT_UPPER_ARM,
		LIMB_RIGHT_LOWER_ARM,
		LIMB_LEFT_UPPER_TORSO,
		LIMB_RIGHT_UPPER_TORSO,
		LIMB_LEFT_LOWER_TORSO,
		LIMB_LEFT_UPPER_LEG,
		LIMB_LEFT_LOWER_LEG,
		LIMB_RIGHT_LOWER_TORSO,
		LIMB_RIGHT_UPPER_LEG,
		LIMB_RIGHT_LOWER_LEG,
		LIMB_HIP
	} SkeletonLimb;

	//Event type
	typedef enum EventType {
		USER_NEW,
		USER_LOST,
		GESTURE_RECOGNIZED,
		GESTURE_PROGRESS
	} EventType;

	//Gesture moiton type
	typedef enum GestureType {
		GESTURE_NONE,
		GESTURE_CLICK,
		GESTURE_WAVE,
		GESTURE_RAISE_HAND
	} GestureType;
}

#endif