/**
 * @file Helpers.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef HELPERS_H
#define HELPERS_H

#include <map>
#include <vector>

/**
 * Utility class that allows std::map initialization.
 * Required because !!!!STUPID!!!! MSVC11 doesn't support initializer lists
 */
template <typename K, typename T>
class create_map 
{
public:
	create_map(const K& key, const T& val) {
		map[key] = val;
	}

	create_map<K, T>& operator()(const K& key, const T& val) {
		map[key] = val;
		return *this;
	}

	operator std::map<K, T>() {
		return map;
	}
private:
	std::map<K, T> map;
};

/**
 * Utility class that allows std::vector initialization.
 * Required because !!!!STUPID!!!! MSVC11 doesn't support initializer lists
 */
template <typename T>
class create_vector
{
public:
	create_vector(const T& val) {
		vec.push_back(val);
	}

	create_vector<T>& operator()(const T& val) {
		vec.push_back(val);
		return *this;
	}

	operator std::vector<T>() {
		return vec;
	}
private:
	std::vector<T> vec;
};

#endif // !HELPERS_H
