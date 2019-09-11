#include "pch.h"

#include "volumes.h"
#include "basic_math.h"

bool pointInVolume(BoxVol volume, vec3f pointWorldPos)
{
    return (isBetweenf(volume.getMinusX(), pointWorldPos.getX(), volume.getX()) && isBetweenf(volume.getMinusY(), pointWorldPos.getY(), volume.getY()) && isBetweenf(volume.getMinusZ(), pointWorldPos.getZ(), volume.getZ()));
}
