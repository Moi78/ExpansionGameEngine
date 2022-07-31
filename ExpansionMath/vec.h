#ifndef EXPGE_VEC_H
#define EXPGE_VEC_H

#include <array>
#include <cassert>

template<int size_>
class vec {
public:
    vec(std::initializer_list<float> elems) {
        assert(elems.size() == size_);

        std::move(elems.begin(), elems.begin() + size_, m_vec.begin());
    }

    float* GetData() {
        return m_vec.data();
    }

protected:
    std::array<float, size_> m_vec;
};

class vec3 : public vec<3> {
public:
    vec3(float x, float y, float z) : vec({x, y, z}) {}

    float GetX() { return m_vec[0]; }
    float GetY() { return m_vec[1]; }
    float GetZ() { return m_vec[2]; }

    void SetX(float nx) { m_vec[0] = nx; }
    void SetY(float ny) { m_vec[1] = ny; }
    void SetZ(float nz) { m_vec[2] = nz; }
};

class vec2 : public vec<2> {
public:
    vec2(float x, float y) : vec({x, y}) {}

    float GetX() { return m_vec[0]; }
    float GetY() { return m_vec[1]; }

    void SetX(float nx) { m_vec[0] = nx; }
    void SetY(float ny) { m_vec[1] = ny; }
};

#endif //EXPGE_VEC_H
