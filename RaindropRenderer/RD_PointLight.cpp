#include "pch.h"
#include "RD_PointLight.h"

RD_PointLight::RD_PointLight(vec3f position, vec3f color, float brightness) {
	m_pos = position;
	m_brightness = brightness;
	m_color = color;

	m_lightIndex = 0;
}

RD_PointLight::~RD_PointLight() {

}

void RD_PointLight::SetPosition(vec3f nPos) {
	m_pos = nPos;
}

void RD_PointLight::SetBrightness(float nBrightness) {
	m_brightness = nBrightness;
}

void RD_PointLight::SetColor(vec3f nColor) {
	m_color = nColor;
}

vec3f RD_PointLight::GetColor() {
	return m_color;
}

int RD_PointLight::GetLightIndex() {
	return m_lightIndex;
}

void RD_PointLight::SetLightIndex(int nIndex) {
	m_lightIndex = nIndex;
}

vec3f RD_PointLight::GetPosition() {
	return m_pos;
}

float RD_PointLight::GetBrightness() {
	return m_brightness;
}