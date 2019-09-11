#ifndef _VOLUME_H__
#define _VOLUME_H__

#include "pch.h"

#ifdef EXPANSIONMATH_EXPORTS
#define EXPANSIONMATH_API __declspec(dllexport)
#else
#define EXPANSIONMATH_API __declspec(dllimport)
#endif

#include "vec3.h"
#include "BoxVol.h"

bool EXPANSIONMATH_API pointInVolume(BoxVol volume, vec3f pointWorldPos);

#endif
