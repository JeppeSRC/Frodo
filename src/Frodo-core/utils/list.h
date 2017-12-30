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

	inline void Push_back(const T& item) { push_back(item); }
	inline void Remove(const T& item) { for (uint_t i = 0; i < size(); i++) { if (at(i) == item) erase(begin() + i); } }
	inline void RemoveIndex(uint_t index) { erase(begin() + index); }

	inline void Reserve(uint_t count) { reserve(count); }
	inline void Resize(uint_t count) { resize(count); }

	inline uint_t Find(const T& item) const {
		for (uint_t i = 0; i < size(); i++) {
			if (at(i) == item) return i;
		}

		return ~0;
	}

	template<typename K = T>
	inline uint_t Find(bool(*CMP_FUNCTION)(const T& item, K& other), K& other) const {
		for (uint_t i = 0; i < size(); i++) {
			if (CMP_FUNCTION(at(i), other)) return i;
		}

		return ~0;
	}

	inline T& operator[](uint_t index) { return at(index); }
	inline const T& operator[](uint_t index) const { return at(index); }

	inline void Clear() { clear(); }

	inline uint_t GetSize() const { return size(); }
	inline uint_t GetSizeInBytes() const { return size() * sizeof(T); }
	inline T*	  GetData() { return  data(); }
	inline T*	  GetData() const { return (T*)&at(0); }
};

}}