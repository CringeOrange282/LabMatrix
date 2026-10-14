#pragma once

#include "TMatrix.h"
#include "MathVector.h"
#include "vector.h"
#include <initializer_list>
#include <fstream>
#include <stdexcept>
#include <cmath>

template <typename T>
class TriangleMatrix : public MathVector<MathVector<T>> {
public:
    inline size_t getN() const noexcept {
        return this->size();
    }

    TriangleMatrix();
    TriangleMatrix(size_t N); 
    TriangleMatrix(const Matrix<T>& other);
    TriangleMatrix(std::initializer_list<std::initializer_list<T>> list);
    TriangleMatrix(const MathVector<MathVector<T>>& other);
    TriangleMatrix(const TriangleMatrix& other);

    T calcDeterminant() const noexcept;

    bool operator==(const TriangleMatrix<T>& other) const noexcept;
    bool operator!=(const TriangleMatrix<T>& other) const noexcept;

    TriangleMatrix<T> operator*(const TriangleMatrix<T>& other) const;
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const TriangleMatrix<T>& other) {
    for (size_t i = 0; i < other.getN(); ++i) {
        out << other[i] << "\n";
    }
    return out;
}

template <typename T>
TriangleMatrix<T>::TriangleMatrix() : MathVector<MathVector<T>>((size_t)0) {}

template <typename T>
TriangleMatrix<T>::TriangleMatrix(size_t N) : MathVector<MathVector<T>>((size_t)0) {
    for (size_t i = 0; i < N; ++i) {
        this->push_back(MathVector<T>(N));
    }
}

template <typename T>
TriangleMatrix<T>::TriangleMatrix(std::initializer_list<std::initializer_list<T>> list)
    : MathVector<MathVector<T>>((size_t)0) {
    size_t N = list.size();
    size_t i = 0;
    for (auto row : list) {
        MathVector<T> vec(N);
        const T* row_elem = row.begin();
        size_t items_to_copy = (row.size() < N) ? row.size() : N;

        for (size_t j = i; j < items_to_copy; ++j) {
            vec[j] = row_elem[j];
        }
        this->push_back(vec);
        i++;
    }
}

template <typename T>
TriangleMatrix<T>::TriangleMatrix(const MathVector<MathVector<T>>& other)
    : MathVector<MathVector<T>>((size_t)0) {
    size_t N = other.size();
    for (size_t i = 0; i < N; ++i) {
        MathVector<T> vec(N);
        for (size_t j = i; j < N; ++j) {
            vec[j] = other[i][j];
        }
        this->push_back(vec);
    }
}

template <typename T>
TriangleMatrix<T>::TriangleMatrix(const TriangleMatrix& other) : MathVector<MathVector<T>>(other) {}

template <typename T>
bool TriangleMatrix<T>::operator==(const TriangleMatrix<T>& other) const noexcept {
    if (this->getN() != other.getN()) { return false; }
    for (size_t i = 0; i < other.getN(); ++i) {
        if ((*this)[i] != other[i]) { return false; }
    }
    return true;
}

template <typename T>
bool TriangleMatrix<T>::operator!=(const TriangleMatrix<T>& other) const noexcept {
    return !((*this) == other);
}

template <typename T>
TriangleMatrix<T>::TriangleMatrix(const Matrix<T>& other) : MathVector<MathVector<T>>((size_t)0) {
    size_t other_N = other.getN();
    if (other_N == 0) { return; }

    Matrix<double> res(other_N, other.getM());
    for (size_t i = 0; i < other_N; ++i) {
        for (size_t j = 0; j < other.getM(); ++j) {
            res[i][j] = static_cast<double>(other[i][j]);
        }
    }

    for (size_t j = 0; j < other_N - 1; j++) {
        double Op = res[j][j];
        if (std::abs(Op) < 1e-9) {
            throw std::logic_error("Division by zero (matrix is singular or requires row swapping)");
        }
        for (size_t i = j + 1; i < other_N; ++i) {
            double Mn = -(res[i][j] / Op);
            for (size_t z = j; z < other_N; ++z) {
                res[i][z] += (res[j][z] * Mn);
            }
        }
    }

    for (size_t i = 0; i < other_N; ++i) {
        MathVector<T> vec(other_N);
        for (size_t j = i; j < other_N; ++j) {
            vec[j] = static_cast<T>(res[i][j]);
        }
        this->push_back(vec);
    }
}

template <typename T>
T TriangleMatrix<T>::calcDeterminant() const noexcept {
    if (this->getN() == 0) { return T(); }
    T res = (*this)[0][0];
    for (size_t i = 1; i < this->getN(); ++i) {
        res *= (*this)[i][i];
    }
    return res;
}

template <typename T>
TriangleMatrix<T> TriangleMatrix<T>::operator*(const TriangleMatrix<T>& other) const {
    if (this->getN() != other.getN()) {
        throw std::logic_error("Different dimensions");
    }
    size_t size = other.getN();
    TriangleMatrix<T> res(size);

    for (size_t i = 0; i < size; ++i) {
        for (size_t j = i; j < size; ++j) {
            T sum = T();
            for (size_t z = i; z <= j; ++z) {
                sum += (*this)[i][z] * other[z][j];
            }
            res[i][j] = sum;
        }
    }
    return res;
}
