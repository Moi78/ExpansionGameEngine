#include "pch.h"
#include "mat4.h"

mat4::mat4(float ival) {
	memset(m_mat, 0, sizeof(m_mat));

	/*
	* Making identity matrix

	ival,	0.0f,	0.0f,	0.0f,
	0.0f,	ival,	0.0f,	0.0f,
	0.0f,	0.0f,	ival,	0.0f,
	0.0f,	0.0f,	0.0f,	ival,

	*/

	for (int i = 0; i < 16; i += 5) {
		m_mat[i] = ival;
	}
}

mat4::~mat4() {

}

float mat4::GetValAt(int row, int column) {
	return m_mat[4 * row + column];
}

void mat4::DBG_print_matrix() {
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			std::cout << m_mat[4 * x + y] << ", ";
		}

		std::cout << std::endl;
	}
}

void mat4::operator*(float a) {
	for (int i = 0; i < 16; i++) {
		m_mat[i] *= a;
	}
}