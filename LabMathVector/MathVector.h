#pragma once
#include "vector.h"
#include <stdexcept>
#include <initializer_list>

template <typename T>
class MathVector : protected Vector<T> {
public:
    using Vector<T>::size;
    using Vector<T>::resize;

    explicit MathVector(size_t s = 0) : Vector<T>(s) {
        this->resize(s);
        for (size_t i = 0; i < s; ++i) {
            (*this)[i] = T();
        }
    }


    MathVector(std::initializer_list<T> data) : Vector<T>(data) {}
    MathVector(const Vector<T>& other) : Vector<T>(other) {}
    ~MathVector() = default;

    MathVector<T>& operator=(const MathVector<T>& other) noexcept {
        if (this != &other) {
            this->Vector<T>::operator=(other);
        }
        return *this;
    }

    T& operator[](size_t index) {
        return this->Vector<T>::operator[](index);
    }

    const T& operator[](size_t index) const {
        return this->Vector<T>::operator[](index);
    }

    MathVector<T>& operator*=(const double& value) noexcept {
        for (size_t i = 0; i < this->size(); ++i) {
            (*this)[i] *= value;
        }
        return *this;
    }

    MathVector<T> operator*(const double& value) const noexcept {
        MathVector<T> res(*this);
        res *= value;
        return res;
    }

    MathVector<T> operator+(const MathVector<T>& other) const {
        MathVector<T> res(*this);
        res += other;
        return res;
    }

    MathVector<T> operator-(const MathVector<T>& other) const {
        MathVector<T> res(*this);
        res -= other;
        return res;
    }

    double operator*(const MathVector<T>& other) const {
        if (this->size() != other.size()) {
            throw std::logic_error("different dimension");
        }
        double res = 0.0;
        for (size_t i = 0; i < this->size(); ++i) {
            res += static_cast<double>((*this)[i] * other[i]);
        }
        return res;
    }

    MathVector<T>& operator+=(const MathVector<T>& other) {
        if (this->size() != other.size()) {
            throw std::logic_error("different size");
        }
        for (size_t i = 0; i < this->size(); ++i) {
            (*this)[i] += other[i];
        }
        return *this;
    }

    MathVector<T>& operator-=(const MathVector<T>& other) {
        if (this->size() != other.size()) {
            throw std::logic_error("different size");
        }
        for (size_t i = 0; i < this->size(); ++i) {
            (*this)[i] -= other[i];
        }
        return *this;
    }

    bool operator==(const MathVector<T>& other) const noexcept {
        if (this->size() != other.size()) return false;
        for (size_t i = 0; i < this->size(); ++i) {
            if ((*this)[i] != other[i]) return false;
        }
        return true;
    }

    bool operator!=(const MathVector<T>& other) const noexcept {
        return !(*this == other);
    }

    template <class friendT>
    friend MathVector<friendT> operator*(const double& value, const MathVector<friendT>& other) noexcept;
};

template <typename T>
MathVector<T> operator*(const double& value, const MathVector<T>& other) noexcept {
    MathVector<T> res(other);
    res *= value;
    return res;
}
