#pragma once
#ifndef _VECTOR_GENERIC_H__
#define _VECTOR_GENERIC_H__

#include <valarray>
#include <array>
#include <cstdarg>

template<int __size, typename T>
class GenericVector {
public:
	GenericVector(T&&... elems) {
		static_assert(sizeof...(elems) == __size, "Not enough arguments in ctor.");

		va_list args;
		va_start(args, elems);

		for (int i = 0; i < __size; i++) {
			m_elems[i] = va_arg(args, elems);
		}
		va_end(args);
	}

	T GetIDX(int idx) {
		static_assert(idx < __size && "Bad given index.");
		return m_elems[idx];
	}

	void SetIDX(int idx, T value) {
		static_assert(idx < __size && "Bad given index.");
		m_elems[idx] = value;
	}

	void SetAll(T&&... elems) {
		static_assert(sizeof...(elems) == __size, "Not enough arguments given.");

		va_list args;
		va_start(args, elems);

		for (int i = 0; i < __size; i++) {
			m_elems[i] = va_arg(args, elems);
		}
		va_end(args);
	}

	float DotProduct(const GenericVector<__size, T> a) {
		std::valarray<T> fa = m_elems;
		std::valarray<T> fb = a.m_elems;

		return (fa * fb).sum();
	}

	float GetLength() {
		std::valarray<T> fa = m_elems;
		fa *= fa;
		float sum = fa.sum();

		return std::sqrtf(sum);
	}

	void NormalizeVector() {
		float ln = GetLength();

		for (int i = 0; i < __size; i++) {
			m_elems[i] /= ln;
		}
	}

	GenericVector<__size, T> GetNormalized() {
		GenericVector<__size, T> vec = GenericVector<__size, T>(this);
		vec.NormalizeVector();

		return vec;
	}

	T* GetPTR() {
		return m_elems.data();
	}

	bool operator==(GenericVector<__size, T> const& a) {
		for (int i = 0; i < __size; i++) {
			if (m_elems[i] != a.m_elems[i]) return false;
		}

		return true;
	}

	bool operator!=(GenericVector<__size, T> const& a) {
		for (int i = 0; i < __size; i++) {
			if (m_elems[i] == a.m_elems[i]) return false;
		}

		return true;
	}

	GenericVector<__size, T> operator+(GenericVector<__size, T> const& a) {
		const GenericVector<__size, T> nvec;

		std::valarray<T> fa = m_elems;
		std::valarray<T> fb = a.m_elems;

		nvec.m_elems = fa + fb;
		return nvec;
	}

	GenericVector<__size, T> operator+(T const& a) {
		const GenericVector<__size, T> nvec;

		std::valarray<T> fa = m_elems;

		nvec.m_elems = fa + a;
		return nvec;
	}

	GenericVector<__size, T> operator-(GenericVector<__size, T> const& a) {
		const GenericVector<__size, T> nvec;

		std::valarray<T> fa = m_elems;
		std::valarray<T> fb = a.m_elems;

		nvec.m_elems = fa - fb;
		return nvec;
	}

	GenericVector<__size, T> operator-(T const& a) {
		const GenericVector<__size, T> nvec;

		std::valarray<T> fa = m_elems;

		nvec.m_elems = fa - a;
		return nvec;
	}

	GenericVector<__size, T> operator*(GenericVector<__size, T> const& a) {
		const GenericVector<__size, T> nvec;

		std::valarray<T> fa = m_elems;
		std::valarray<T> fb = a.m_elems;

		nvec.m_elems = fa * fb;
		return nvec;
	}

	GenericVector<__size, T> operator*(T const& a) {
		const GenericVector<__size, T> nvec;

		std::valarray<T> fa = m_elems;

		nvec.m_elems = fa * a;
		return nvec;
	}

	void operator+=(T const& a) {
		for (int i = 0; i < __size; i++) {
			m_elems[i] += a;
		}
	}

	void operator+=(GenericVector<__size, T> const& a) {
		for (int i = 0; i < __size; i++) {
			m_elems[i] += a.m_elems[i];
		}
	}

protected:
	std::array<T, __size> m_elems;
};

#endif //_VECTOR_GENERIC_H__