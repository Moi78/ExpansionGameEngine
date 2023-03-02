#pragma once

#include "mat4.h"
#include "vec.h"

class Quat {
public:
	Quat(float w, float x, float y, float z) {
		m_quat[0] = w;
		m_quat[1] = x;
		m_quat[2] = y;
		m_quat[3] = z;
	}

	~Quat() {
		
	}

	float GetW() { return m_quat[0]; }
	float GetX() { return m_quat[1]; }
	float GetY() { return m_quat[2]; }
	float GetZ() { return m_quat[3]; }

	void SetW(float nw) { m_quat[0] = nw; }
	void SetX(float nx) { m_quat[1] = nx; }
	void SetY(float ny) { m_quat[2] = ny; }
	void SetZ(float nz) { m_quat[3] = nz; }

	float GetRealPart() {
		return m_quat[0];
	}
	
	vec3 GetImagPart() {
		return vec3(m_quat[1], m_quat[2], m_quat[3]);
	}

	mat4f ToMat4() {
		float a[16] = {
			 m_quat[0],  m_quat[3], -m_quat[2], m_quat[1],
			-m_quat[3],  m_quat[0],  m_quat[1], m_quat[2],
			 m_quat[2], -m_quat[1],  m_quat[0], m_quat[3],
			-m_quat[1], -m_quat[2], -m_quat[3], m_quat[0],
		};

		float b[16] = {
			 m_quat[0],  m_quat[3], -m_quat[2], -m_quat[1],
			-m_quat[3],  m_quat[0],  m_quat[1], -m_quat[2],
			 m_quat[2], -m_quat[1],  m_quat[0], -m_quat[3],
			 m_quat[1],  m_quat[2],  m_quat[3],  m_quat[0],
		};
		
		mat4f mat_a(a);
		mat4f mat_b(b);

		return mat_b * mat_a;
	}
	
	Quat operator*(const Quat& q) {
		const float w = (m_quat[0] * q.m_quat[0]) - (m_quat[1] * q.m_quat[1]) - (m_quat[2] * q.m_quat[2]) - (m_quat[3] * q.m_quat[3]);
		const float x = (m_quat[1] * q.m_quat[0]) + (m_quat[0] * q.m_quat[1]) + (m_quat[2] * q.m_quat[3]) - (m_quat[3] * q.m_quat[2]);
		const float y = (m_quat[0] * q.m_quat[2]) - (m_quat[1] * q.m_quat[3]) + (m_quat[2] * q.m_quat[0]) + (m_quat[3] * q.m_quat[1]);
		const float z = (m_quat[0] * q.m_quat[3]) + (m_quat[1] * q.m_quat[2]) - (m_quat[2] * q.m_quat[1]) + (m_quat[3] * q.m_quat[0]);

		return Quat(w, x, y, z);
	}

	void Conjugate() {
		for(int i = 0; i < 3; i++) {
			m_quat[i] *= -1.0f;
		}
	}

	Quat GetConjugate() {
		return Quat(m_quat[0], -m_quat[1], -m_quat[2], -m_quat[3]);
	}

private:
	float m_quat[4];
};