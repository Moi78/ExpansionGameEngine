#include "pch.h"
#include "RD_DirLight.h"

RD_DirLight::RD_DirLight(vec3f dir, vec3f color, float brightness) : m_dir(dir), m_color(color), m_brightness(brightness) {

}

RD_DirLight::~RD_DirLight() {

}

void RD_DirLight::SetLightBrightness(float nBrightness) {
	m_brightness = nBrightness;
}

void RD_DirLight::SetLightColor(vec3f nColor) {
	m_color = nColor;
}

void RD_DirLight::SetLightDir(vec3f nDir) {
	m_dir = nDir;
}

float RD_DirLight::GetBrightness() {
	return m_brightness;
}

vec3f RD_DirLight::GetLightColor() {
	return m_color;
}

vec3f RD_DirLight::GetLightDir() {
	return m_dir;
}