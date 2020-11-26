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

	Float4(float a = 0.0f) {
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
		m_vec = { x, y, z,w };
	}

	Float4(__m128 vec) {
		m_vec = vec;
	}

	Float4(float a = 0.0f) {
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
		return m_vec[index];
	}

private:
	__m128 m_vec;
};

#endif //__GNUC__