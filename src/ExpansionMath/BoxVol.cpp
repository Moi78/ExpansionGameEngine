#include "pch.h"

#include "BoxVol.h"

BoxVol::BoxVol(vec3f size, vec3f WorldPos) : m_size(size), m_WorldPos(WorldPos)
{
}

BoxVol::~BoxVol()
{
}

float BoxVol::getMinusX()
{
    return m_WorldPos.getX() - m_size.getX();
}

float BoxVol::getMinusY()
{
    return m_WorldPos.getY() - m_size.getY();
}

float BoxVol::getMinusZ()
{
    return m_WorldPos.getZ() - m_size.getZ();
}

vec3f BoxVol::getSize()
{
    return m_size;
}

vec3f BoxVol::getWorldPosition()
{
    return m_WorldPos;
}

float BoxVol::getX()
{
    return m_WorldPos.getX() + m_size.getX();
}

float BoxVol::getY()
{
    return m_WorldPos.getY() + m_size.getY();
}

float BoxVol::getZ()
{
    return m_WorldPos.getZ() + m_size.getZ();
}

void BoxVol::setSize(vec3f nsize)
{
    m_size = nsize;
}

void BoxVol::setWorldPosition(vec3f npos)
{
    m_WorldPos = npos;
}

