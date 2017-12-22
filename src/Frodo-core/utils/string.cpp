#include "string.h"

namespace fd {
namespace util {

String::String() {

}

String::String(const char* const string) : std::string(string) {

}

String::String(const std::string& string) : std::string(string) {

}

String::String(const String& string) : std::string(string) {
	
}

String& String::Append(const char* const string) {
	append(string);

	return *this;
}

String& String::Append(const std::string& string) {
	append(string);

	return *this;
}

String& String::Append(const String& string) {
	append(string);

	return *this;
}

uint_t String::Find(const char* const string, uint_t len, uint_t offset) const {
	const char* tmpstr = c_str();

	for (uint_t i = offset; i < length() - len + 1; i++) {
		bool match = true;
		for (uint_t j = 0; j < len; j++) {
			if (tmpstr[i] != string[j]) {
				match = false;
				break;
			}
		}

		if (match) {
			return i;
		}
	}

	return ~0;
}

uint_t String::Find(const char c, uint_t offset) const {
	const char* tmpstr = c_str();

	for (uint_t i = offset; i < length(); i++) {
		if (tmpstr[i] == c) return i;
	}

	return ~0;
}

uint_t String::Find(const char* const string, uint_t offset) const {
	return Find(string, strlen(string), offset);
}

uint_t String::Find(const std::string& string, uint_t offset) const {
	return Find(string.c_str(), string.length(), offset);
}

uint_t String::Find(const String& string, uint_t offset) const {
	return Find((std::string)string, offset);
}

String& String::RemoveChars(const char* const chars, bool iterate) {
	return RemoveChars(chars, strlen(chars), iterate);
}

String& String::RemoveChars(const char* const chars, uint_t length, bool iterate) {
	for (uint_t i = 0; i < length; i++) {
		const char c = chars[i];
		uint_t index = Find(c);
		do {
			Remove(index, index+1);
			index = Find(c);
		} while (index != ~0 && iterate);

	}

	return *this;
}

String& String::RemoveChars(const std::string& chars, bool iterate) {
	return RemoveChars(chars.c_str(), chars.length(), iterate);
}

String& String::RemoveChars(const String& chars, bool iterate) {
	return RemoveChars((std::string)chars, iterate);
}

String& String::Remove(const String& string) {
	uint_t index = Find(string);

	return Remove(index, index + string.GetLength());
}

String& String::Remove(uint_t start, uint_t end) {
	erase(begin() + start, begin() + end);
	return *this;
}

String& String::RemoveBlankspace() {

	uint_t start = Find(" ");
	while (start != -1) {
		Remove(start, start + 1);
		start = Find(" ", start);
	}

	start = Find("\n");
	while (start != -1) {
		Remove(start, start + 1);
		start = Find("\n", start);
	}

	start = Find("\r");
	while (start != -1) {
		Remove(start, start + 1);
		start = Find("\r", start);
	}

	start = Find("\t");
	while (start != -1) {
		Remove(start, start + 1);
		start = Find("\t", start);
	}

	return *this;
}

String String::SubString(uint_t start, uint_t end) const {
	return substr(start, end - start);
}

uint_t String::Count(const char* const string, uint_t offset) const {
	return Count(string, strlen(string), offset);
}

uint_t String::Count(const char* const string, uint_t len, uint_t offset) const {
	const char* tmpstr = c_str();

	uint_t count = 0;

	for (uint_t i = offset; i < length() - len; i++) {
		bool match = true;
		for (uint_t j = 0; j < len; j++) {
			if (tmpstr[i] != string[j]) {
				match = false;
				break;
			}
		}

		if (match) {
			count++;
		}
	}

	return count;
}

uint_t String::Count(const std::string& string, uint_t offset) const {
	return Count(string.c_str(), string.length(), offset);
}

uint_t String::Count(const String& string, uint_t offset) const {
	return Count((std::string)string, offset);
}

}}