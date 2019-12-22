#include "pch.h"
#include "EXP_Camera.h"

EXP_Camera::EXP_Camera(EXP_Game* gameinstance, vec3f pos, vec3f rot, vec3f scale, vec3f dir, float FOV, float Near, float Far) : EXP_Component(pos, rot, scale), RD_Camera(gameinstance->GetRenderer()->GetShader(), gameinstance->GetRenderer(), FOV, Near, Far, pos, dir) {

}

EXP_Camera::~EXP_Camera() {

}