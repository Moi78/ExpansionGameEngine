#include "pch.h"

#include "PS_Listener.h"

PS_Listener::PS_Listener(vec3f lstnr_pos, vec3f ori) : m_pos(lstnr_pos), m_ori(ori)
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

vec3f PS_Listener::getOrientation() {
    return m_ori;
}

void PS_Listener::setOrientation(vec3f n_ori) {
    m_ori = n_ori;
}