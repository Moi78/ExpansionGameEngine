#include "pch.h"
#include "mat4.h"

mat4::mat4() {
	memset(m_mat, 0, sizeof(m_mat));
}

mat4::mat4(float mat[4][4]) {
	memcpy(m_mat, mat, sizeof(m_mat));
}

mat4::~mat4() {

}

float mat4::GetValAt(int row, int column) {
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
	

	return mat4(m_mat);
}