#pragma once
#include "vector.h"
#include <stdexcept>

template <typename T>
class MathVector : public Vector<T> {
public:
    explicit MathVector(size_t s = 0);
    MathVector(std::initializer_list<T> data);
    MathVector(const Vector<T>& other);
    ~MathVector() = default;

    MathVector<T>& operator=(const MathVector<T>& other) noexcept;

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    MathVector<T> operator*(const double& value) const noexcept;
    MathVector<T>& operator*=(const double& value) noexcept;

    MathVector<T> operator+(const MathVector<T>& other) const;
    MathVector<T> operator-(const MathVector<T>& other) const;
    double operator*(const MathVector<T>& other) const;

    MathVector<T>& operator+=(const MathVector<T>& other);
    MathVector<T>& operator-=(const MathVector<T>& other);

    bool operator==(const MathVector<T>& other) const noexcept;
    bool operator!=(const MathVector<T>& other) const noexcept;

    template <class friendT>
    friend MathVector<friendT> operator*(const double& value, const MathVector<friendT>& other) noexcept;
};

template <typename T>
MathVector<T> operator*(const double& value, const MathVector<T>& other) noexcept {
    MathVector<T> res(other);
    res *= value;
    return res;
}

template <typename T>
MathVector<T>::MathVector(size_t s) : Vector<T>(0) {
    for (size_t i = 0; i < s; ++i) {
        this->push_back(T());
    }
}

template <typename T>
MathVector<T>::MathVector(std::initializer_list<T> data) : Vector<T>(data) {}

template <typename T>
MathVector<T>::MathVector(const Vector<T>& other) : Vector<T>(other) {}

template <typename T>
MathVector<T>& MathVector<T>::operator=(const MathVector<T>& other) noexcept {
    if (this != &other) {
        this->Vector<T>::operator=(other);
    }
    return *this;
}

template <typename T>
T& MathVector<T>::operator[](size_t index) {
    return this->Vector<T>::operator[](index);
}

template <typename T>
const T& MathVector<T>::operator[](size_t index) const {
    return this->Vector<T>::operator[](index);
}

template <typename T>
MathVector<T> MathVector<T>::operator*(const double& value) const noexcept {
    MathVector<T> res(*this);
    res *= value;
    return res;
}

template <typename T>
MathVector<T>& MathVector<T>::operator*=(const double& value) noexcept {
    for (size_t i = 0; i < this->size(); ++i) {
        (*this)[i] *= value;
    }
    return *this;
}

template <typename T>
MathVector<T> MathVector<T>::operator+(const MathVector<T>& other) const {
    if (this->size() != other.size()) {
        throw std::logic_error("different size");
    }
    MathVector<T> res(*this);
    for (size_t i = 0; i < this->size(); ++i) {
        res[i] = (*this)[i] + other[i];
    }
    return res;
}

template <typename T>
MathVector<T> MathVector<T>::operator-(const MathVector<T>& other) const {
    if (this->size() != other.size()) {
        throw std::logic_error("different size");
    }
    MathVector<T> res(*this);
    for (size_t i = 0; i < this->size(); ++i) {
        res[i] = (*this)[i] - other[i];
    }
    return res;
}

template <typename T>
double MathVector<T>::operator*(const MathVector<T>& other) const {
    if (this->size() != other.size()) {
        throw std::logic_error("different dimension");
    }
    double res = 0.0;
    for (size_t i = 0; i < this->size(); ++i) {
        res += static_cast<double>((*this)[i] * other[i]);
    }
    return res;
}

template <typename T>
MathVector<T>& MathVector<T>::operator+=(const MathVector<T>& other) {
    if (this->size() != other.size()) {
        throw std::logic_error("different size");
    }
    for (size_t i = 0; i < this->size(); ++i) {
        (*this)[i] += other[i];
    }
    return *this;
}

template <typename T>
MathVector<T>& MathVector<T>::operator-=(const MathVector<T>& other) {
    if (this->size() != other.size()) {
        throw std::logic_error("different size");
    }
    for (size_t i = 0; i < this->size(); ++i) {
        (*this)[i] -= other[i];
    }
    return *this;
}

template <typename T>
bool MathVector<T>::operator==(const MathVector<T>& other) const noexcept {
    if (this->size() != other.size()) return false;
    for (size_t i = 0; i < this->size(); ++i) {
        if ((*this)[i] != other[i]) return false;
    }
    return true;
}

template <typename T>
bool MathVector<T>::operator!=(const MathVector<T>& other) const noexcept {
    return !(*this == other);
}
