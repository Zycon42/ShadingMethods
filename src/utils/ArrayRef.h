/**
 * @file ArrayRef.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef ARRAY_REF_H
#define ARRAY_REF_H

#include <vector>
#include <cstddef>

/**
 * Convenient class for passing non-const references to arrays.
 * Use this class to pass arrays to functions, it's intended to
 * pass by value, don't worry its just pointer and size.
 */
template <typename T>
class ArrayRef
{
public:
	typedef T value_type;
	typedef T* pointer;
	typedef size_t size_type;

	ArrayRef(pointer ptr, size_type n) : ptr(ptr), n(n) { }
	ArrayRef(std::vector<T>& vec) : ptr(vec.data()), n(vec.size()) { }

	pointer data() {
		return ptr;
	}

	const pointer data() const {
		return ptr;
	}

	size_type size() const {
		return n;
	}

	value_type& operator[](size_type i) {
		assert(i < n);
		return ptr[i];
	}

	const value_type& operator[](size_type i) const {
		assert(i < n);
		return ptr[i];
	}
private:
	T* ptr;
	size_type n;
};

/**
 * Convenient class for passing const references to arrays.
 * Use this class to pass arrays to functions, it's intended to
 * pass by value, don't worry its just pointer and size.
 */
template <typename T>
class ConstArrayRef
{
public:
	typedef T value_type;
	typedef T* pointer;
	typedef size_t size_type;

	ConstArrayRef(const pointer ptr, size_type n) : ptr(ptr), n(n) { }
	ConstArrayRef(const std::vector<T>& vec) : ptr(vec.data()), n(vec.size()) { }
	ConstArrayRef(ConstArrayRef<T>& other) : ptr(other.data()), n(other.size()) { }

	const pointer data() const {
		return ptr;
	}

	size_type size() const {
		return n;
	}

	const value_type& operator[](size_type i) const {
		assert(i < n);
		return ptr[i];
	}
private:
	const T* ptr;
	size_type n;
};

#endif // !ARRAY_REF_H
