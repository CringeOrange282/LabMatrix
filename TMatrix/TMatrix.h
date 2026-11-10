#pragma once
#include "MathVector.h"
#include <stdexcept>
#include <iostream>
#include <initializer_list>

template <typename T>
class Matrix : public MathVector<MathVector<T>> {
public:
	inline size_t getN() const noexcept {
		return this->size();
	}
	inline size_t getM() const noexcept {
		if (getN() == 0) { return 0; }
		return (*this)[0].size();
	}

	Matrix();
	Matrix(size_t N, size_t M);
	Matrix(std::initializer_list<std::initializer_list<T>> list);
	Matrix(const Matrix<T>& other);
	Matrix(const MathVector<MathVector<T>>& other);
	~Matrix() = default;

	Matrix<T> operator*(const Matrix<T>& other) const;
	MathVector<T> operator*(const MathVector<T>& vec) const;

	Matrix<T> Transposition() const noexcept;

	bool operator==(const Matrix<T>& other) const noexcept;
	bool operator!=(const Matrix<T>& other) const noexcept;
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const Matrix<T>& matrix) {
	for (size_t i = 0; i < matrix.getN(); ++i) {
		for (size_t j = 0; j < matrix.getM(); ++j) {
			out << matrix[i][j] << " ";
		}
		out << "\n";
	}
	return out;
}

template <typename T>
Matrix<T>::Matrix() : MathVector<MathVector<T>>(0) {}

template <typename T>
Matrix<T>::Matrix(size_t N, size_t M) : MathVector<MathVector<T>>(N) {
	for (size_t i = 0; i < N; ++i) {
		(*this)[i] = MathVector<T>(M);
	}
}

template <typename T>
Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T>> list) : MathVector<MathVector<T>>(list.size()) {
	size_t i = 0;
	for (const auto& row : list) {
		(*this)[i++] = MathVector<T>(row);
	}
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T>& other) : MathVector<MathVector<T>>(other) {}

template <typename T>
Matrix<T>::Matrix(const MathVector<MathVector<T>>& other) : MathVector<MathVector<T>>(other) {}

template <typename T>
Matrix<T> Matrix<T>::Transposition() const noexcept {
	size_t this_N = this->getN();
	size_t this_M = this->getM();
	Matrix<T> res(this_M, this_N);
	for (size_t i = 0; i < this_M; ++i) {
		for (size_t j = 0; j < this_N; ++j) {
			res[i][j] = (*this)[j][i];
		}
	}
	return res;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& other) const {
	size_t this_N = this->getN();
	size_t this_M = this->getM();
	if (this_M != other.getN()) {
		throw std::logic_error("No way dimension");
	}
	Matrix<T> otherT = other.Transposition();
	size_t otherM = other.getM();
	Matrix<T> res(this_N, otherM);
	for (size_t i = 0; i < this_N; ++i) {
		for (size_t j = 0; j < otherM; ++j) {
			res[i][j] = (*this)[i] * otherT[j];
		}
	}
	return res;
}

template <typename T>
MathVector<T> Matrix<T>::operator*(const MathVector<T>& vec) const {
	if (this->getM() != vec.size()) {
		throw std::logic_error("Matrix columns must match vector size");
	}
	MathVector<T> res(this->getN());
	for (size_t i = 0; i < this->getN(); ++i) {
		res[i] = (*this)[i] * vec;
	}
	return res;
}

template <typename T>
bool Matrix<T>::operator==(const Matrix<T>& other) const noexcept {
	if (this->getM() != other.getM() || this->getN() != other.getN()) {
		return false;
	}
	for (size_t i = 0; i < other.getN(); ++i) {
		if ((*this)[i] != other[i]) {
			return false;
		}
	}
	return true;
}

template <typename T>
bool Matrix<T>::operator!=(const Matrix<T>& other) const noexcept {
	return !((*this) == other);
}
