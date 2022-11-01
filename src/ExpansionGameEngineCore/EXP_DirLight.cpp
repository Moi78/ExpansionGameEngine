#include "EXP_DirLight.h"

EXP_DirLight::EXP_DirLight(vec3 dir, vec3 color, float brightness) :
EXP_Component(vec3(), vec3(), vec3()), RD_DirLight(dir, brightness, color) {

}