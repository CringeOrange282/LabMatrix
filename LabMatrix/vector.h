#pragma once

#include <iostream>
#include <stdexcept>
#include "memdata.h"

enum DirectionToShift { _Left, _Right };

template <typename T>
class Vector {
    MemData<T> _mem;
    size_t _front;
    size_t _back;
public:
    Vector(size_t size = 0);
    Vector(std::initializer_list<T>);
    Vector(T*, size_t);
    Vector(const Vector<T>&);
    Vector(Vector<T>&&);
    ~Vector() = default;

    T& operator[](size_t human_index) noexcept {
        size_t real_index = (_front + human_index) % _mem._capacity;
        return _mem._data[real_index];
    }

    const T& operator[](size_t human_index) const noexcept {
        size_t real_index = (_front + human_index) % _mem._capacity;
        return _mem._data[real_index];
    }

    template <typename Type>
    class Iterator {
        Type* p_cur;
    public:
        Iterator() : p_cur(nullptr) {}
        Iterator(Type* ptr) : p_cur(ptr) {}
        Iterator(const Iterator& ptr) {
            p_cur = ptr.p_cur;
        }

        bool operator==(const Iterator& ptr) noexcept {
            return (p_cur == ptr.p_cur);
        }
        bool operator!=(const Iterator& ptr) noexcept {
            return (p_cur != ptr.p_cur);
        }

        Iterator& operator=(const Iterator& other) {
            if (this != &other) {
                p_cur = other.p_cur;
            }
            return *this;
        }
        Iterator& operator+=(int num) {
            p_cur += num;
            return *this;
        }
        Iterator& operator-=(int num) {
            p_cur -= num;
            return *this;
        }

        Iterator operator+(int num) const {
            Iterator temp = *this;
            temp += num;
            return temp;
        }
        Iterator operator-(int num) const {
            Iterator temp = *this;
            temp -= num;
            return temp;
        }

        Iterator& operator++() {
            p_cur++;
            return *this;
        }
        Iterator operator++(int) {
            Iterator temp = *this;
            p_cur++;
            return temp;
        }

        Iterator& operator--() {
            p_cur--;
            return *this;
        }
        Iterator operator--(int) {
            Iterator temp = *this;
            p_cur--;
            return temp;
        }

        Type& operator*() {
            return *p_cur;
        }
        const Type& operator*() const {
            return *p_cur;
        }
    };

    typedef Iterator<T> iterator;
    typedef Iterator<const T> const_iterator;

    iterator begin() {
        return iterator(_mem._data + _front);
    }
    iterator end() {
        return iterator(_mem._data + _front + _mem._size);
    }
    const_iterator begin() const {
        return const_iterator(_mem._data + _front);
    }
    const_iterator end() const {
        return const_iterator(_mem._data + _front + _mem._size);
    }

    inline bool is_empty() const noexcept;
    inline bool is_full() const noexcept;

    inline size_t size() const noexcept;
    inline size_t capacity() const noexcept;
    inline T front() const;
    inline T back() const;

    inline T& front();
    inline T& back();

    void push_front(T) noexcept;
    void push_back(T) noexcept;
    void insert(T, size_t);

    void pop_front(size_t = 1);
    void pop_back(size_t = 1);
    void erase(size_t, size_t = 1);

    void push_front(std::initializer_list<T>) noexcept;
    void push_back(std::initializer_list<T>) noexcept;
    void insert(std::initializer_list<T>, size_t);

    Vector<T>& operator=(const Vector<T>&) noexcept;
    Vector<T>& operator=(Vector<T>&&) noexcept;

    template <typename Type>
    friend std::ostream& operator<<(std::ostream&, const Vector<Type>&);

    template <typename Type>
    friend std::istream& operator>>(std::istream&, Vector<Type>&);

    void shuffle() noexcept;
    void selection_sort() noexcept;
    void shrink_to_fit();

private:
    void push_elements_to_insert(size_t, DirectionToShift, size_t = 1);
    void push_elements_to_erase(size_t, DirectionToShift, size_t = 1);
    void reset_memory(size_t);
    void clear();
};

template <typename T>
inline size_t Vector<T>::size() const noexcept {
    return _mem._size;
}

template <typename T>
inline size_t Vector<T>::capacity() const noexcept {
    return _mem._capacity;
}

template <typename T>
inline T Vector<T>::front() const {
    if (_mem._size == 0) throw std::logic_error("empty vector");
    return _mem._data[_front];
}

template <typename T>
inline T Vector<T>::back() const {
    if (_mem._size == 0) throw std::logic_error("empty vector");
    return _mem._data[_back];
}

template <typename T>
inline bool Vector<T>::is_empty() const noexcept {
    return _mem.is_empty();
}

template <typename T>
inline bool Vector<T>::is_full() const noexcept {
    return _mem.is_full();
}

template <typename T>
inline T& Vector<T>::front() {
    if (_mem._size == 0) throw std::logic_error("empty vector");
    return _mem._data[_front];
}

template <typename T>
inline T& Vector<T>::back() {
    if (_mem._size == 0) throw std::logic_error("empty vector");
    return _mem._data[_back];
}

template <typename T>
Vector<T>::Vector(size_t size) : _mem(MemData<T>(size)), _front(0), _back(size == 0 ? 0 : size - 1) {
}

template <typename T>
Vector<T>::Vector(std::initializer_list<T> vec) : _mem(MemData<T>(vec)), _front(0), _back(_mem.size() - 1) {}

template <typename T>
Vector<T>::Vector(T* data, size_t size) : _mem(MemData<T>(data, size)), _front(0), _back(_mem.size() - 1) {}

template <typename T>
Vector<T>::Vector(const Vector<T>& other) : _mem(other._mem._size), _front(0), _back(other._mem._size - 1) {
    _mem._size = other._mem._size;
    for (size_t i = 0; i < other._mem._size; i++) {
        size_t other_index = (other._front + i) % other._mem._capacity;
        _mem._data[i] = other._mem._data[other_index];
    }
}

template <typename T>
Vector<T>::Vector(Vector<T>&& other) : _mem(std::move(other._mem)), _front(other._front), _back(other._back) {}

template <typename T>
void Vector<T>::push_front(T elem) noexcept {
    if (is_empty()) {
        _front = 0;
        _back = 0;
        _mem._data[_front] = elem;
        _mem._size = 1;
        return;
    }
    if (is_full()) {
        reset_memory(_mem._size + 1);
        _front = _mem._capacity - 1;
        _mem._data[_front] = elem;
        _mem._size++;
        return;
    }
    _front = (_front == 0) ? _mem._capacity - 1 : _front - 1;
    _mem._data[_front] = elem;
    _mem._size++;
}

template <typename T>
void Vector<T>::push_back(T elem) noexcept {
    if (is_empty()) {
        _front = 0;
        _back = 0;
        _mem._data[_front] = elem;
        _mem._size = 1;
        return;
    }
    if (is_full()) {
        reset_memory(_mem._size + 1);
        _back = _mem._size;
        _mem._data[_back] = elem;
        _mem._size++;
        return;
    }
    _back = (_back == _mem._capacity - 1) ? 0 : _back + 1;
    _mem._data[_back] = elem;
    _mem._size++;
}

template <typename T>
void Vector<T>::insert(T elem, size_t human_index) {
    if (human_index > _mem._size) {
        throw std::logic_error("index out of range");
    }
    if (is_full()) {
        reset_memory(_mem._size + 1);
    }
    if (is_empty() && human_index != 0) {
        throw std::logic_error("index out of range");
    }
    if (human_index == 0) {
        push_front(elem);
        return;
    }
    if (human_index == _mem._size) {
        push_back(elem);
        return;
    }
    DirectionToShift direction;
    if (human_index < _mem._size / 2) direction = _Left;
    else direction = _Right;

    push_elements_to_insert(human_index, direction);
    size_t real_index = (_front + human_index) % _mem._capacity;
    _mem._data[real_index] = elem;
    _mem._size++;
}

template <typename T>
void Vector<T>::pop_front(size_t count) {
    if (_mem._size < count) {
        throw std::logic_error("index out of range");
    }
    _front = (_front + count) % _mem._capacity;
    _mem._size -= count;
    if (calculate_capacity(_mem._size) < _mem._capacity) {
        reset_memory(_mem._size);
    }
}

template <typename T>
void Vector<T>::pop_back(size_t count) {
    if (_mem._size < count) {
        throw std::logic_error("index out of range");
    }
    _back = (_back - count + _mem._capacity) % _mem._capacity;
    _mem._size -= count;
    if (calculate_capacity(_mem._size) < _mem._capacity) {
        reset_memory(_mem._size);
    }
}

template <typename T>
void Vector<T>::erase(size_t human_index, size_t count) {
    if (human_index > _mem._size - 1) throw std::logic_error("index out of range");
    if (_mem._size < count) throw std::logic_error("index out of range");
    if (human_index + count > _mem._size) throw std::logic_error("index out of range");
    if (human_index == 0) {
        pop_front(count);
        return;
    }
    if (human_index == _mem._size - 1) {
        pop_back(count);
        return;
    }
    size_t left_count = human_index;
    size_t right_count = _mem._size - human_index - count;
    DirectionToShift direction = (right_count <= left_count) ? _Left : _Right;

    push_elements_to_erase(human_index, direction, count);
    _mem._size -= count;

    if (calculate_capacity(_mem._size) < _mem._capacity) {
        reset_memory(_mem._size);
    }
}

template <typename T>
void Vector<T>::push_front(std::initializer_list<T> other) noexcept {
    if (is_empty()) {
        _front = 0;
        _back = other.size() - 1;
    }
    if (_mem._size + other.size() > _mem._capacity) {
        reset_memory(_mem._size + other.size());
        _front = 0;
        _back = _mem._size + other.size() - 1;
    }
    _front = (_front - other.size() + _mem._capacity) % _mem._capacity;
    size_t index = _front;
    for (T element : other) {
        _mem._data[index] = element;
        index = (index + 1) % _mem._capacity;
    }
    _mem._size += other.size();
}

template <typename T>
void Vector<T>::push_back(std::initializer_list<T> other) noexcept {
    if (is_empty()) {
        _front = 0;
        _back = -1;
    }
    if (_mem._size + other.size() > _mem._capacity) {
        reset_memory(_mem._size + other.size());
        _front = 0;
        _back = _mem._size - 1;
    }
    size_t index = _back != -1 ? _back + 1 : 0;
    _back = (_back + other.size()) % _mem._capacity;
    for (T element : other) {
        _mem._data[index] = element;
        index = (index + 1) % _mem._capacity;
    }
    _mem._size += other.size();
}

template <typename T>
void Vector<T>::insert(std::initializer_list<T> other, size_t human_index) {
    if (human_index > _mem._size) throw std::logic_error("index out of range");
    if (_mem._size + other.size() > _mem._capacity) reset_memory(_mem._size + other.size());
    if (is_empty() && human_index != 0) throw std::logic_error("index out of range");
    if (human_index == 0) {
        push_front(other);
        return;
    }
    if (human_index == _mem._size) {
        push_back(other);
        return;
    }
    DirectionToShift direction = (human_index < _mem._size / 2) ? _Left : _Right;
    push_elements_to_insert(human_index, direction, other.size());
    size_t real_index = (_front + human_index) % _mem._capacity;
    for (T element : other) {
        _mem._data[real_index] = element;
        real_index = (real_index + 1) % _mem._capacity;
    }
    _mem._size += other.size();
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other) noexcept {
    if (this == &other) return *this;
    _front = other._front;
    _back = other._back;
    _mem.set_memory(other._mem._size);
    _mem._size = other._mem._size;
    _mem._capacity = other._mem._capacity;
    for (size_t i = 0; i < _mem._size; i++) _mem._data[i] = other._mem._data[i];
    return (*this);
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector<T>&& other) noexcept {
    if (this == &other) return *this;
    _mem.set_memory(other._mem._size);
    _mem._size = other._mem._size;
    _mem._capacity = other._mem._capacity;
    _front = 0;
    _back = _mem._size - 1;
    size_t old_index = other._front;
    for (size_t i = 0; i < _mem._size; i++) {
        _mem._data[i] = other._mem._data[old_index++];
        if (old_index == other._mem._capacity) old_index = 0;
    }
    other._mem._size = 0;
    other._mem._capacity = 0;
    other._mem._data = nullptr;
    return (*this);
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& vec) {
    if (vec.size() == 0) { os << "{ }"; return os; }
    os << "{ ";
    for (size_t i = 0; i < vec.size() - 1; i++) os << vec[i] << ", ";
    os << vec[vec.size() - 1] << " }";
    return os;
}

template <typename T>
std::istream& operator>>(std::istream& is, Vector<T>& vec) {
    size_t new_size;
    is >> new_size;

    if (!vec.is_empty()) {
        vec.pop_front(vec.size());
    }

    for (size_t i = 0; i < new_size; i++) {
        T element;
        is >> element;
        vec.push_back(element);
    }
    return is;
}

template <typename T>
void Vector<T>::push_elements_to_insert(size_t human_index, DirectionToShift direction, size_t count) {
    size_t real_index = (_front + human_index) % _mem._capacity;
    size_t real_new_index;
    if (direction == _Left) {
        for (size_t i = _front; i != real_index + 1; i = (i + 1) % _mem._capacity) {
            real_new_index = (i - count + _mem._capacity) % _mem._capacity;
            _mem._data[real_new_index] = _mem._data[i];
        }
        _front = (_front - count + _mem._capacity) % _mem._capacity;
        return;
    }
    if (direction == _Right) {
        size_t end_index = (real_index == 0) ? _mem._capacity - 1 : real_index - 1;
        for (size_t i = (_back + count) % _mem._capacity; i != end_index; i = (i - 1 + _mem._capacity) % _mem._capacity) {
            real_new_index = (i - count + _mem._capacity) % _mem._capacity;
            _mem._data[i] = _mem._data[real_new_index];
        }
        _back = (_back + count) % _mem._capacity;
        return;
    }
}

template <typename T>
void Vector<T>::push_elements_to_erase(size_t human_index, DirectionToShift direction, size_t count) {
    size_t left_count = human_index;
    size_t right_count = _mem._size - human_index - count;
    if (direction == _Left) {
        size_t copy_start_index = (_front + human_index + count) % _mem._capacity;
        size_t remove_start_index = (_front + human_index) % _mem._capacity;
        for (size_t i = 0; i < right_count; ++i) {
            size_t copy_index = (copy_start_index + i) % _mem._capacity;
            size_t remove_index = (remove_start_index + i) % _mem._capacity;
            _mem._data[remove_index] = _mem._data[copy_index];
        }
        _back = (_back - count + _mem._capacity) % _mem._capacity;
        return;
    }
    if (direction == _Right) {
        for (size_t i = left_count; i > 0; --i) {
            size_t copy_index = (_front + i - 1) % _mem._capacity;
            size_t remove_index = (_front + i - 1 + count) % _mem._capacity;
            _mem._data[remove_index] = _mem._data[copy_index];
        }
        _front = (_front + count) % _mem._capacity;
        return;
    }
}

template <typename T>
void Vector<T>::reset_memory(size_t new_size) {
    _mem.reset_memory(new_size, _front);
    _front = 0;
    _back = _mem._size - 1;
}

template <typename T>
void Vector<T>::shuffle() noexcept {
    if (_mem._size <= 1) return;
    T tmp;
    for (size_t i = _mem._size - 1; i > 0; --i) {
        size_t j = rand() % (i + 1);
        tmp = (*this)[i];
        (*this)[i] = (*this)[j];
        (*this)[j] = tmp;
    }
}

template <typename T>
void Vector<T>::selection_sort() noexcept {
    if (_mem._size <= 1) return;
    for (size_t i = 0; i < _mem._size - 1; i++) {
        size_t min_index = i;
        for (size_t j = i + 1; j < _mem._size; j++) {
            if ((*this)[j] < (*this)[min_index]) min_index = j;
        }
        if (min_index != i) {
            T temp = (*this)[i];
            (*this)[i] = (*this)[min_index];
            (*this)[min_index] = temp;
        }
    }
}

template <typename T>
void Vector<T>::shrink_to_fit() {
    if (_mem._size == _mem._capacity) return;
    size_t new_capacity = (_mem._size == 0) ? MEM_STEP : _mem._size;
    T* new_data = new T[new_capacity];
    for (size_t i = 0; i < _mem._size; i++) {
        size_t real_index = (_front + i) % _mem._capacity;
        new_data[i] = _mem._data[real_index];
    }
    _mem.set_memory(_mem._size);
    delete[] _mem._data;
    _mem._data = new_data;
    _mem._capacity = new_capacity;
    _front = 0;
    _back = (_mem._size == 0) ? 0 : _mem._size - 1;
}
