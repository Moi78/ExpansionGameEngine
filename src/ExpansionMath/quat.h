#pragma once

#include "mat4.h"
#include "vec.h"

class Quat;
static float Dot(Quat a, Quat b);

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

		return mat_a * mat_b;
	}
	
	Quat operator*(const Quat& q) {
		const float w = (m_quat[0] * q.m_quat[0]) - (m_quat[1] * q.m_quat[1]) - (m_quat[2] * q.m_quat[2]) - (m_quat[3] * q.m_quat[3]);
		const float x = (m_quat[1] * q.m_quat[0]) + (m_quat[0] * q.m_quat[1]) + (m_quat[2] * q.m_quat[3]) - (m_quat[3] * q.m_quat[2]);
		const float y = (m_quat[0] * q.m_quat[2]) - (m_quat[1] * q.m_quat[3]) + (m_quat[2] * q.m_quat[0]) + (m_quat[3] * q.m_quat[1]);
		const float z = (m_quat[0] * q.m_quat[3]) + (m_quat[1] * q.m_quat[2]) - (m_quat[2] * q.m_quat[1]) + (m_quat[3] * q.m_quat[0]);

		return Quat(w, x, y, z);
	}

    Quat operator*(const float x) {
        return Quat(m_quat[0] * x, m_quat[1] * x, m_quat[2] * x, m_quat[3] * x);
    }

    Quat operator+(const Quat& q) {
        return Quat(m_quat[0] + q.m_quat[0], m_quat[1] + q.m_quat[1], m_quat[2] + q.m_quat[2], m_quat[3] + q.m_quat[3]);
    }

	void Conjugate() {
		for(int i = 1; i < 4; i++) {
			m_quat[i] *= -1.0f;
		}
	}

	Quat GetConjugate() {
		return Quat(m_quat[0], -m_quat[1], -m_quat[2], -m_quat[3]);
	}

    Quat GetInverse() {
        float sumSquared = 0;
        for(int i = 0; i < 4; i++) {
            sumSquared += m_quat[i] * m_quat[i];
        }

        return Quat(m_quat[0] / sumSquared, -m_quat[1] / sumSquared, -m_quat[2] / sumSquared, -m_quat[3] / sumSquared);
    }

    static Quat FromEuler(vec3& rot) {
        // Taken from https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

        float yaw = rot.GetZ() * 3.141592f / 180.0f;
        float pitch = rot.GetY() * 3.141592f / 180.0f;
        float roll = rot.GetX() * 3.141592f / 180.0f;

        float cr = cos(roll * 0.5f);
        float sr = sin(roll * 0.5f);

        float cp = cos(pitch * 0.5f);
        float sp = sin(pitch * 0.5f);

        float cy = cos(yaw * 0.5f);
        float sy = sin(yaw * 0.5f);

        return Quat(
            cr * cp * cy + sr * sp * sy,
            sr * cp * cy - cr * sp * sy,
            cr * sp * cy + sr * cp * sy,
            cr * cp * sy - sr * sp * cy
        );
    }

private:
	float m_quat[4];
};

static Quat Slerp(Quat a, Quat b, float t) {
    float cosTheta = Dot(a, b);
    if(cosTheta < 1) {
        float theta = std::acos(cosTheta);
        float invSinTheta = 1 / std::sin(theta);

        float q1_fac = std::sin((1 - t) * theta) * invSinTheta;
        float q2_fac = std::sin(t * theta) * invSinTheta;

        Quat q1 = a * q1_fac;
        Quat q2 = b * q2_fac;

        return q1 + q2;
    } else {
        return a;
    }
}

static float Dot(Quat a, Quat b) {
    return  a.GetW() * b.GetW() +
            a.GetX() * b.GetX() +
            a.GetY() * b.GetY() +
            a.GetZ() * b.GetZ();
}