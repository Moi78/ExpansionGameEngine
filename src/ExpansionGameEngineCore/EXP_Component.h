#ifndef EXPGE_EXP_COMPONENT_H
#define EXPGE_EXP_COMPONENT_H

#include <memory>

#include <mat4.h>
#include <vec.h>

class EXP_Component {
public:
    EXP_Component(vec3 pos, vec3 rot, vec3 scale) {
        m_pos = pos;
        m_rot = rot;
        m_scale = scale;

        m_ptrans = mat4f(1.0f);
    };

    virtual ~EXP_Component() {};

    void SetParentMatrix(mat4f& matrix);

    vec3 GetComponentPos();
    vec3 GetComponentRot();
    vec3 GetComponentScale();

    void SetComponentPos(vec3 npos);
    void SetComponentRot(vec3 nrot);
    void SetComponentScale(vec3 nscale);

    mat4f GetComponentTransform();
    void UpdateTransform();

private:
    vec3 m_pos, m_rot, m_scale;

    mat4f m_transform;
    mat4f m_ptrans;
};


#endif //EXPGE_EXP_COMPONENT_H
