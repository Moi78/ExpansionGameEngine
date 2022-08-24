#ifndef EXPGE_VEC_H
#define EXPGE_VEC_H

#include <array>
#include <cassert>
#include <cmath>

template<int size_>
class vec {
public:
    vec(std::initializer_list<float> elems) {
        assert(elems.size() == size_);

        std::move(elems.begin(), elems.begin() + size_, &m_vec[0]);
    }

    float* GetData() {
        return &m_vec[0];
    }

    float GetLength() {
        float lsquared = 0;
        for(int i = 0; i < size_; i++) {
            lsquared += pow(m_vec[i], 2);
        }

        return sqrt(lsquared);
    }

    void NormalizeVector() {
        float len = GetLength();
        for(int i = 0; i < size_; i++) {
            m_vec[i] /= len;
        }
    }

protected:
    float m_vec[size_];
};

class vec4 : public vec<4> {
public:
    vec4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) : vec({x, y, z, w}) {}

    float GetX() { return m_vec[0]; }
    float GetY() { return m_vec[1]; }
    float GetZ() { return m_vec[2]; }
    float GetW() { return m_vec[3]; }

    void SetX(float nx) { m_vec[0] = nx; }
    void SetY(float ny) { m_vec[1] = ny; }
    void SetZ(float nz) { m_vec[2] = nz; }
    void SetW(float nw) { m_vec[3] = nw; }

    virtual vec4 operator-(const vec4& a) {
        vec4 v(0.0, 0.0, 0.0, 0.0);
        for(int i = 0; i < 4; i++) {
            v.GetData()[i] = m_vec[i] - a.m_vec[i];
        }

        return v;
    }

    virtual vec4 operator+(const vec4& a) {
        vec4 v(0.0, 0.0, 0.0, 0.0);
        for(int i = 0; i < 4; i++) {
            v.GetData()[i] = m_vec[i] + a.m_vec[i];
        }

        return v;
    }
};

class vec3 : public vec<3> {
public:
    vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : vec({x, y, z}) {}

    float GetX() { return m_vec[0]; }
    float GetY() { return m_vec[1]; }
    float GetZ() { return m_vec[2]; }

    void SetX(float nx) { m_vec[0] = nx; }
    void SetY(float ny) { m_vec[1] = ny; }
    void SetZ(float nz) { m_vec[2] = nz; }

    virtual vec3 operator-(const vec3& a) {
        vec3 v(0.0, 0.0, 0.0);
        for(int i = 0; i < 3; i++) {
            v.GetData()[i] = m_vec[i] - a.m_vec[i];
        }

        return v;
    }

    virtual vec3 operator+(const vec3& a) {
        vec3 v(0.0, 0.0, 0.0);
        for(int i = 0; i < 3; i++) {
            v.GetData()[i] = m_vec[i] + a.m_vec[i];
        }

        return v;
    }
};

class vec2 : public vec<2> {
public:
    vec2(float x = 0.0f, float y = 0.0f) : vec({x, y}) {}

    float GetX() { return m_vec[0]; }
    float GetY() { return m_vec[1]; }

    void SetX(float nx) { m_vec[0] = nx; }
    void SetY(float ny) { m_vec[1] = ny; }

    virtual vec2 operator-(const vec2& a) {
        vec2 v(0.0, 0.0);
        for(int i = 0; i < 2; i++) {
            v.GetData()[i] = m_vec[i] - a.m_vec[i];
        }

        return v;
    }

    virtual vec2 operator+(const vec2& a) {
        vec2 v(0.0, 0.0);
        for(int i = 0; i < 2; i++) {
            v.GetData()[i] = m_vec[i] + a.m_vec[i];
        }

        return v;
    }
};

vec3 Cross(vec3 a, vec3 b);

#endif //EXPGE_VEC_H
