#ifndef EXPGE_EXP_CAMERA_H
#define EXPGE_EXP_CAMERA_H

#include <memory>

#include "EXP_Game.h"
#include "EXP_Component.h"

#include <RD_Camera.h>

#include <mat4.h>
#include <vec.h>

class EXP_Camera : public RD_Camera, public EXP_Component {
public:
    EXP_Camera(EXP_Game* game, vec3 pos, vec3 ypr, float FOV, float near, float far);
    ~EXP_Camera() {};

    void SetParentMatrix(mat4f& transf) override;

    void AddYaw(float y);
    void AddPitch(float p);
    void AddRoll(float r);

private:
    void ComputeYPR();

    vec3 m_ypr;
};


#endif //EXPGE_EXP_CAMERA_H
