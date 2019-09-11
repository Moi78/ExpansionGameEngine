#include "pch.h"

#include "PS_Listener.h"

PS_Listener::PS_Listener(vec3f lstnr_pos) : m_pos(lstnr_pos)
{
}

void PS_Listener::setPosition(vec3f n_pos)
{
    m_pos = n_pos;
}

vec3f PS_Listener::getPosition()
{
    return m_pos;
}
