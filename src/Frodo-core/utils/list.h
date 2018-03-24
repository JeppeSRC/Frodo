#pragma once

#include <vector>
#include <core/types.h>

namespace fd {
namespace utils {

template<typename T>
class List : private std::vector<T> {
private:

public:
	List(uint_t reserved = 0) : std::vector<T>(reserved) { }

	inline void Push_back(const T& item) { std::vector<T>::push_back(item); }
	inline void Remove(const T& item) { for (uint_t i = 0; i < std::vector<T>::size(); i++) { if (std::vector<T>::at(i) == item) std::vector<T>::erase(std::vector<T>::begin() + i); } }
	inline void RemoveIndex(uint_t index) { std::vector<T>::erase(std::vector<T>::begin() + index); }

	inline void Reserve(uint_t count) { std::vector<T>::reserve(count); }
	inline void Resize(uint_t count) { std::vector<T>::resize(count); }

	inline uint_t Find(const T& item) const {
		for (uint_t i = 0; i < std::vector<T>::size(); i++) {
			if (std::vector<T>::at(i) == item) return i;
		}

		return ~0;
	}

	template<typename K = T>
	inline uint_t Find(bool(*CMP_FUNCTION)(const T& item, K& other), K& other) const {
		for (uint_t i = 0; i < std::vector<T>::size(); i++) {
			if (CMP_FUNCTION(std::vector<T>::at(i), other)) return i;
		}

		return ~0;
	}

	inline T& operator[](uint_t index) { return std::vector<T>::at(index); }
	inline const T& operator[](uint_t index) const { return std::vector<T>::at(index); }

	inline void Clear() { std::vector<T>::clear(); }

	inline uint_t GetSize() const { return std::vector<T>::size(); }
	inline uint_t GetSizeInBytes() const { return std::vector<T>::size() * sizeof(T); }
	inline T*	  GetData() { return  std::vector<T>::data(); }
	inline T*	  GetData() const { return (T*)&std::vector<T>::at(0); }
};

}}