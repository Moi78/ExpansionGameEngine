#include "pch.h"
#include "mat4.h"

mat4::mat4(double filler) {
	for (int i = 0; i < 4; i++) {
		for (int a = 0; a < 4; a++) {
			m_mat[i][a] = filler;
		}
	}
}

mat4::mat4(double mat[4][4]) {
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			m_mat[row][column] = mat[row][column];
		}
	}
}

mat4::~mat4() {

}

double mat4::GetValAt(int row, int column) {
	return m_mat[row][column];
}

void mat4::DBG_print_matrix() {
	for (int i = 0; i < 4; i++) {
		for (int a = 0; a < 4; a++) {
			std::cout << m_mat[i][a] << ", ";
		}

		std::cout << std::endl;
	}
}

mat4 mat4::operator*(mat4 const& a) {
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			m_mat[row][column] *= a.m_mat[row][column];
		}
	}

	return mat4(m_mat);
}