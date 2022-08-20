#include "mat4.h"

mat4<float> ProjPersp(float FOV, float ImageRatio, float nearv, float farv) {
    float mat[16] = {
            1 / (ImageRatio * tan(FOV / 2)),	0,  0,  0,
            0, 1 / tan(FOV / 2), 0,  0,
            0, 0, -((farv + nearv) / (farv - nearv)), -((2 * (farv * nearv)) / (farv - nearv)),
            0, 0, -1,  0
    };

    return mat4<float>(mat);
}

mat4<float> LookAt(vec3 pos, vec3 target, vec3 up) {
    //Dir
    vec3 cam_dir(pos - target);
    cam_dir.NormalizeVector();

    //Right
    vec3 cam_right(Cross(up, cam_dir));
    cam_right.NormalizeVector();

    //Up
    vec3 cam_up(Cross(cam_dir, cam_right));

    float mat_rot[16] = {
            cam_right.GetX(), cam_right.GetY(), cam_right.GetZ(), 0,
            cam_up.GetX(),	 cam_up.GetY(),	   cam_up.GetZ(), 0,
            cam_dir.GetX(),	cam_dir.GetY(),	  cam_dir.GetZ(), 0,
            0,				 0,				   0, 1,
    };

    float mat_trans[16] = {
            1, 0, 0, -pos.GetX(),
            0, 1, 0, -pos.GetY(),
            0, 0, 1, -pos.GetZ(),
            0, 0, 0, 1
    };

    return mat4<float>(mat_rot) * mat4<float>(mat_trans);
}

mat4<float> ProjOrtho(float right, float left, float top, float bottom, float nearv, float farv) {
    float mat[16] = {
            2 / (right - left), 0, 0, -((right + left) / (right - left)),
            0, 2 / (top - bottom), 0, -((top + bottom) / (top - bottom)),
            0, 0, -2 / (farv - nearv), -((farv + nearv) / (farv - nearv)),
            0, 0, 0, 1
    };

    return mat4<float>(mat);
}