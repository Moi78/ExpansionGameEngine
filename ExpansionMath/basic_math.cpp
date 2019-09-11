#include "pch.h"

#include "basic_math.h"

bool isBetweenf(float inf, float toCompare, float sup)
{
    return (inf <= toCompare && sup >= toCompare);
}

bool isBetweeni(int inf, int toCompare, int sup)
{
    return (inf <= toCompare && sup >= toCompare);
}
