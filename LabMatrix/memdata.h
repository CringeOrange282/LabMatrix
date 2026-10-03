#pragma once

#include <initializer_list>

#define MEM_STEP 15

inline int calculate_capacity(size_t size) {
	return size == 0 ? MEM_STEP : (size - 1) / MEM_STEP * MEM_STEP + MEM_STEP;
}

template <typename T>
class Vector;

template <typename T>
class MemData {
	size_t _size;
	size_t _capacity;
	T* _data;

public:

	MemData(size_t size = 0);
	MemData(std::initializer_list<T>);
	MemData(T*, size_t);
	MemData(const MemData&);
	MemData(MemData&&);
	~MemData();

	inline bool is_empty() const noexcept;
	inline bool is_full() const noexcept;

	inline size_t size() const noexcept;
	inline size_t capacity() const noexcept;
	inline const T* const data() const noexcept;

	void set_memory(size_t) noexcept;
	void reset_memory(size_t size, size_t start_index = 0) noexcept;
	void clear_memory() noexcept;

	MemData& operator=(const MemData&) noexcept;
	MemData& operator=(MemData&&) noexcept;

	template <typename T>
	friend class Vector;
};

template <typename T>
inline bool MemData<T>::is_empty() const noexcept {
	return _size == 0;
}

template <typename T>
inline bool MemData<T>::is_full() const noexcept {
	return _size == _capacity;
}

template <typename T>
inline size_t MemData<T>::size() const noexcept {
	return _size;
}

template <typename T>
inline size_t MemData<T>::capacity() const noexcept {
	return _capacity;
}

template <typename T>
inline const T* const MemData<T>::data() const noexcept {
	return _data;
}

template <typename T>
MemData<T>::MemData(size_t size) : _size(0), _capacity(calculate_capacity(size)), _data(new T[_capacity]) {}

template <typename T>
MemData<T>::MemData(std::initializer_list<T> data) : _size(data.size()), _capacity(calculate_capacity(_size)), _data(data.size() == 0 ? nullptr : new T[_capacity]) {

	for (size_t i = 0; i < _size; i++) {
		_data[i] = *(data.begin() + i);
	}

}

template <typename T>
MemData<T>::MemData(T* data, size_t size) : _size(size), _capacity(calculate_capacity(_size)), _data(new T[_capacity]) {

	for (size_t i = 0; i < _capacity; i++) {
		_data[i] = data[i];
	}

}
template <typename T>
MemData<T>::MemData(const MemData& m2) : _size(m2._size), _capacity(calculate_capacity(_size)), _data(new T[_capacity]) {

	for (size_t i = 0; i < _capacity; i++) {
		_data[i] = m2._data[i];
	}

}

template <typename T>
MemData<T>::MemData(MemData&& m2) : _size(m2._size), _capacity(m2._capacity), _data(m2._data) {
	m2._data = nullptr;
	m2._size = 0;
	m2._capacity = MEM_STEP;
}

template <typename T>
MemData<T>::~MemData() {
	delete[]_data;
}

template <typename T>
void MemData<T>::set_memory(size_t size) noexcept {
	if (_capacity == calculate_capacity(size)) {
		_size = 0;
		return;
	}

	delete[]_data;
	_size = 0;
	_capacity = calculate_capacity(size);
	_data = new T[_capacity];
}

template <typename T>
void MemData<T>::reset_memory(size_t size, size_t start_index) noexcept {
	if (_capacity == calculate_capacity(size)) {
		return;
	}

	size_t new_capacity = calculate_capacity(size);
	T* new_data = new T[new_capacity];

	for (size_t i = 0; i < (_size <= size ? _size : size); i++) {
		new_data[i] = _data[start_index++];
		if (start_index >= _capacity) start_index = 0;
	}

	_capacity = new_capacity;
	delete[] _data;
	_data = new_data;
}

template <typename T>
void MemData<T>::clear_memory() noexcept {
	delete[]_data;
	_data = new T[MEM_STEP];;
	_size = 0;
	_capacity = MEM_STEP;
}

template <typename T>
MemData<T>& MemData<T>::operator=(const MemData<T>& other) noexcept {
	if (&other == this) return *this;

	delete[]_data;
	_size = other._size;
	_capacity = calculate_capacity(_size);
	_data = new T[_capacity];

	for (size_t i = 0; i < _capacity; i++) {
		_data[i] = other._data[i];
	}

	return *this;
}

template <typename T>
MemData<T>& MemData<T>::operator=(MemData<T>&& other) noexcept {
	if (&other == this) return *this;

	delete[]_data;
	_size = other._size;
	_capacity = other._capacity;
	_data = other._data;
	other._data = nullptr;

	return *this;
}
