#pragma once

#include <immintrin.h>

#if !defined(__GNUC__)

class Float4 {
public:
	Float4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) {
		m_vec = { x, y, z,w };
	}

	Float4(__m128 vec) {
		m_vec = vec;
	}

	void LoadConstant(float a) {
		m_vec = _mm_load_ss(&a);
	}

	float* GetPointer() {
		return &m_vec.m128_f32[0];
	}

	Float4 operator*(const Float4& a) {
		__m128 res = _mm_mul_ps(m_vec, a.m_vec);

		return Float4(res);
	}

	float Get(unsigned index) {
		return m_vec.m128_f32[index];
	}

private:
	__m128 m_vec;
};

#else

class Float4 {
public:
	Float4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) {
		m_vec[0] = x;
		m_vec[1] = y;
		m_vec[2] = z;
		m_vec[3] = w;
	}

	Float4(__m128 vec) {
		m_vec = vec;
	}

	void LoadConstant(float a) {
		m_vec = _mm_load_ss(&a);
	}

	float* GetPointer() {
        _mm_store1_ps(m_mem_mirror.data(), m_vec);
		return m_mem_mirror.data();
	}

	Float4 operator*(const Float4& a) {
		__m128 res = _mm_mul_ps(m_vec, a.m_vec);

		return Float4(res);
	}

	float Get(unsigned index) {
		return m_vec[index];
	}

private:
	__m128 m_vec;
    std::array<float, 4> m_mem_mirror;
};

#endif //__GNUC__