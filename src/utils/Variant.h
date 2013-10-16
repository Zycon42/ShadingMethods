/**
 * @file Variant.h
 *
 * @author Jan Dušek
 * @date 2012
 */

#ifndef VARIANT_H
#define VARIANT_H

#include <typeinfo>
#include <algorithm>

/**
 * Class that can store any data.
 */
class Variant
{
public:
	/// Constructs empty variant.
	Variant() : data(nullptr) { }

	/// Constructs variant from given value.
	template <typename T>
	Variant(const T& val) : data(new Holder<T>(val)) {
	}

	/// Copy constructor
	Variant(const Variant& other) : data(other.data ? other.data->clone() : nullptr) {
	}

	/// Move constructor
	Variant(Variant&& other) : data(nullptr) {
		swap(*this, other);
	}

	/// Destructor
	~Variant() {
		delete data;
	}

	/// Swaps contents of two variants.
	friend void swap(Variant& lhs, Variant& rhs) {
		using std::swap; 
		swap(lhs.data, rhs.data);
	}

	/// Assign operator
	Variant& operator=(Variant other) {
		swap(*this, other);
		return *this;
	}

	/// Assign operator for any type.
	template <typename T>
	Variant& operator=(T other) {
		Variant v(std::move(other));
		swap(*this, v);
		return *this;
	}

	/**
	 * Gets stored value. Performs runtime type check.
	 * @return stored value
	 * @throws std::bad_cast when T is different than stored type or Variant is empty.
	 */
	template <typename T>
	T get() const {
		// type() == typeid(T) also returns false when this is empty.
		return type() == typeid(T) ? static_cast<Holder<T>*>(data)->held : throw std::bad_cast();
	}

	/// Checks if variant is empty
	bool isEmpty() const {
		return !data;
	}

	/// Gets type_info
	const std::type_info& type() const {
		return data ? data->type() : typeid(void);
	}

private:
	// internal helper types to store also type information.

	class Placeholder
	{
	public:
		virtual ~Placeholder() { }
		virtual const std::type_info& type() const = 0;
		virtual Placeholder* clone() const = 0;
	};

	template <typename T>
	class Holder : public Placeholder
	{
	public:
		Holder(const T& held) : held(held) { }

		virtual const std::type_info& type() const {
			return typeid(T);
		}

		virtual Placeholder* clone() const {
			return new Holder(held);
		}

		T held;
	};

	Placeholder* data;
};

#endif // !VARIANT_H
