#include "pch.h"
#include "rotator.h"

rotator::rotator(float angle, vec3f axis) {
	m_angle = angle;
	m_axis = axis;
}

rotator::~rotator() {

}

float rotator::GetAngle() {
	return m_angle;
}

vec3f rotator::GetAxis() {
	return m_axis;
}