#pragma once

#include <string>
#include <core/types.h>

namespace fd {
namespace util {

class String {
private:
	std::string str;

	uint_t Find(const char* const string, uint_t length, uint_t offset) const;
public:
	String();
	String(const char* const string);
	String(const std::string& string);
	String(const String& string);

	String& Append(const char* const string);
	String& Append(const std::string& string);
	String& Append(const String& string);

	inline void operator+=(const char* const string) { Append(string); }
	inline void operator+=(const std::string& string) { Append(string); }
	inline void operator+=(const String& string) { Append(string); }

	uint_t Find(const char c, uint_t offset = 0) const;
	uint_t Find(const char* const string, uint_t offset = 0) const;
	uint_t Find(const std::string& string, uint_t offset = 0) const;
	uint_t Find(const String& string, uint_t offset = 0) const;

	inline String operator+(const char* const string) const { return String(*this).Append(string); }
	inline String operator+(const std::string& string) const { return String(*this).Append(string); }
	inline String operator+(const String& string) const { return String(*this).Append(string); }

	inline void operator=(const char* const string) { str = string; }
	inline void operator=(const std::string& string) { str = string; }
	inline void operator=(const String& string) { str = string.str; }

	String& RemoveChars(const char* const chars, bool iterate);
	String& RemoveChars(const char* const chars, uint_t length, bool iterate);
	String& RemoveChars(const std::string& chars, bool iterate);
	String& RemoveChars(const String& chars, bool iterate);
	String& Remove(const String& string);
	String& Remove(uint_t start, uint_t end);
	String& RemoveBlankspace();

	String SubString(uint_t start, uint_t end) const;

	uint_t Count(const char* const string, uint_t offset = 0) const;
	uint_t Count(const char* const string, uint_t length, uint_t offset = 0) const;
	uint_t Count(const std::string& string, uint_t offset = 0) const;
	uint_t Count(const String& string, uint_t offset = 0) const;

	char operator[](uint_t i) const { return str.c_str()[i]; }

	inline uint_t GetLength() const { return (uint_t)str.length(); }

	inline const char* operator*() const { return str.c_str(); }
};

}}