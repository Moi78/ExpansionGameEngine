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

class vec3 : public vec<3> {
public:
    vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : vec({x, y, z}) {}

    float GetX() { return m_vec[0]; }
    float GetY() { return m_vec[1]; }
    float GetZ() { return m_vec[2]; }

    void SetX(float nx) { m_vec[0] = nx; }
    void SetY(float ny) { m_vec[1] = ny; }
    void SetZ(float nz) { m_vec[2] = nz; }

    void SetAll(float x, float y, float z) {
        m_vec[0] = x;
        m_vec[1] = y;
        m_vec[2] = z;
    }

    vec3 NormalizedVector() {
        vec3 copy(m_vec[0], m_vec[1], m_vec[2]);
        copy.NormalizeVector();

        return copy;
    }

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

    virtual vec3 operator*(const float& a) {
        vec3 v(0.0, 0.0, 0.0);
        for(int i = 0; i < 3; i++) {
            v.GetData()[i] = m_vec[i] * a;
        }

        return v;
    }

    virtual void operator*=(const float& a) {
        for(auto& c : m_vec) {
            c *= a;
        }
    }

    float operator[](const int idx) {
        return m_vec[idx];
    }
};

class vec2 : public vec<2> {
public:
    vec2(float x = 0.0f, float y = 0.0f) : vec({x, y}) {}

    float GetX() { return m_vec[0]; }
    float GetY() { return m_vec[1]; }

    void SetX(float nx) { m_vec[0] = nx; }
    void SetY(float ny) { m_vec[1] = ny; }

    void SetAll(float x, float y) {
        m_vec[0] = x;
        m_vec[1] = y;
    }

    vec2 NormalizedVector() {
        vec2 copy(m_vec[0], m_vec[1]);
        copy.NormalizeVector();

        return copy;
    }

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

    virtual vec2 operator*(const float& a) {
        vec2 v(0.0, 0.0);
        for(int i = 0; i < 2; i++) {
            v.GetData()[i] = m_vec[i] * a;
        }

        return v;
    }

    virtual void operator*=(const float& a) {
        for(auto& c : m_vec) {
            c *= a;
        }
    }

    float operator[](const int idx) {
        return m_vec[idx];
    }
};

class vec4 : public vec<4> {
public:
    vec4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) : vec({x, y, z, w}) {}
    vec4(vec3 xyz, float w) : vec({xyz.GetX(), xyz.GetY(), xyz.GetZ(), w}) {}
    vec4(std::array<float, 4> xyzw) : vec({xyzw[0], xyzw[1], xyzw[2], xyzw[3]}) {}

    float GetX() { return m_vec[0]; }
    float GetY() { return m_vec[1]; }
    float GetZ() { return m_vec[2]; }
    float GetW() { return m_vec[3]; }

    void SetX(float nx) { m_vec[0] = nx; }
    void SetY(float ny) { m_vec[1] = ny; }
    void SetZ(float nz) { m_vec[2] = nz; }
    void SetW(float nw) { m_vec[3] = nw; }

    void SetAll(float x, float y, float z, float w) {
        m_vec[0] = x;
        m_vec[1] = y;
        m_vec[2] = z;
        m_vec[3] = w;
    }

    vec4 NormalizedVector() {
        vec4 copy(m_vec[0], m_vec[1], m_vec[2], m_vec[3]);
        copy.NormalizeVector();

        return copy;
    }

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

    virtual vec4 operator*(const float& a) {
        vec4 v(0.0, 0.0, 0.0, 0.0);
        for(int i = 0; i < 4; i++) {
            v.GetData()[i] = m_vec[i] * a;
        }

        return v;
    }

    virtual void operator*=(const float& a) {
        for(auto& c : m_vec) {
            c *= a;
        }
    }

    float& operator[](const int idx) {
        return m_vec[idx];
    }

    vec3 ToVec3() {
        return vec3(m_vec[0], m_vec[1], m_vec[2]);
    }
};

vec3 Cross(vec3 a, vec3 b);
vec3 Lerp(vec3& a, vec3& b, float t);

#endif //EXPGE_VEC_H
