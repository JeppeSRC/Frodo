#include "string.h"

namespace fd {
namespace util {

String::String() : str() {

}

String::String(const char* const string) : str(string) {

}

String::String(const std::string& string) : str(string) {

}

String::String(const String& string) : str(string.str) {
	
}

String& String::Append(const char* const string) {
	str.append(string);

	return *this;
}

String& String::Append(const std::string& string) {
	str.append(string);

	return *this;
}

String& String::Append(const String& string) {
	str.append(string.str);

	return *this;
}

uint_t String::Find(const char* const string, uint_t length, uint_t offset) const {
	const char* tmpstr = str.c_str();

	for (uint_t i = offset; i < str.length() - length + 1; i++) {
		bool match = true;
		for (uint_t j = 0; j < length; j++) {
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
	const char* tmpstr = str.c_str();

	for (uint_t i = offset; i < str.length(); i++) {
		if (tmpstr[i] == c) return i;
	}

	return ~0;
}

uint_t String::Find(const char* const string, uint_t offset) const {
	return Find(string, strlen(string), offset);
}

uint_t String::Find(const std::string& string, uint_t offset) const {
	return Find(str.c_str(), string.length(), offset);
}

uint_t String::Find(const String& string, uint_t offset) const {
	return Find(string.str, offset);
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
	return RemoveChars(str.c_str(), chars.length(), iterate);
}

String& String::RemoveChars(const String& chars, bool iterate) {
	return RemoveChars(chars.str, iterate);
}

String& String::Remove(const String& string) {
	uint_t index = Find(string);

	return Remove(index, index + string.GetLength());
}

String& String::Remove(uint_t start, uint_t end) {
	str.erase(str.begin() + start, str.begin() + end);
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
	return str.substr(start, end - start);
}

uint_t String::Count(const char* const string, uint_t offset) const {
	return Count(string, strlen(string), offset);
}

uint_t String::Count(const char* const string, uint_t length, uint_t offset) const {
	const char* tmpstr = str.c_str();

	uint_t count = 0;

	for (uint_t i = offset; i < str.length() - length; i++) {
		bool match = true;
		for (uint_t j = 0; j < length; j++) {
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
	return Count(string.str, offset);
}

}}