#pragma once
#include <core/types.h>
#include <stdio.h>

namespace fd { namespace utils {

class String {
private:
	template<typename T>
	friend class List;
public:
	char* str;
	uint_t length;

	bool noDelete;

public:
	String() { str = nullptr; length = 0; }
	String(const char* string);
	String(const wchar_t* string);
	String(char* string, uint_t length, bool noCopy = false);
	String(const String& string);
	String(const String* string);
	String(String&& string);
	~String();

	String& operator=(const String& string);
	String& operator=(String&& string);

	String& Append(const char character);
	String& Append(const String& string);
	__forceinline String& operator<<(const String& string) { return Append(string); }
	__forceinline String& operator<<(const char character) { return Append(character); }

	String& RemoveChars(const String& chars, bool iterate);
	String& Remove(const String& string);
	String& Remove(uint_t start, uint_t end);
	String& RemoveBlankspace();

	String  SubString(uint_t start, uint_t end) const;

	uint_t Count(const String& string, uint_t offset = 0) const;

	inline bool IsNull() const { return (str == nullptr || length == 0); }

	char operator[](uint_t index) const;

	bool operator==(const String& string) const;
	bool operator!=(const String& string) const;
	String operator+(const String& string) const;
	__forceinline void operator+=(const String& string) { Append(string); }
	__forceinline void operator+=(const char character) { Append(character); }

	bool StartsWith(const String& string) const;
	bool EndsWith(const String& string) const;
	uint_t Find(const String& string, uint_t offset = 0) const;
	uint_t Find(const char c, uint_t offset = 0) const;

	List<String*> Split(const char delimiter) const;
	void Split(const char delimiter, List<String*>& list) const;

	inline char* operator*() const { return str; }

	inline void SetNoDelete(bool nodelete) { this->noDelete = nodelete; }

	inline wchar_t* GetWCHAR() const {
		wchar_t* tmp = new wchar_t[length + 1];
		swprintf_s(tmp, length + 1, L"%S", str);
		tmp[length] = '\0';
		return tmp;
	}
};

}}