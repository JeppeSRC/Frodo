#include "string.h"
#include "list.h"
#include <string>
#include <core/log/log.h>

namespace fd { namespace utils {

String::String(const char* string) {
	if (!string) {
		length = 0;
		str = nullptr;
		return;
	}
	length = strlen(string);
	str = new char[length + 1];
	str[length] = '\0';
	memcpy(str, string, length);

	noDelete = false;
}

String::String(const wchar_t* string) {
	if (!string) {
		length = 0;
		str = nullptr;
		return;
	}

	length = wcslen(string);

	str = new char[length + 1];

	sprintf(str, "%S", string);

	str[length] = '\0';

	noDelete = false;
}

String::String(char* string, uint_t length, bool noCopy) {
	if (!string) {
		this->length = 0;
		str = nullptr;
		return;
	}
	if (noCopy) {
		this->length = length;
		str = string;
		noDelete = true;
	} else {
		this->length = length;
		str = new char[length + 1];
		str[length] = '\0';
		memcpy(str, string, length);

		noDelete = false;
	}
}

String::String(const String& string) {
	this->length = string.length;

	str = new char[length + 1];
	str[length] = 0;
	memcpy(str, string.str, length);

	noDelete = false;
}

String::String(const String* string) {
	this->length = string->length;

	str = new char[length + 1];
	str[length] = 0;
	memcpy(str, string->str, length);

	noDelete = false;
}

String::String(String&& string) {
	this->length = 0;
	this->str = nullptr;
	*this = std::move(string);

	noDelete = false;
}

String::~String() {
	if (!noDelete) delete[] str;
	str = nullptr;
}

String& String::operator=(const String& string) {
	if (this != &string) {
		delete[] str;
		length = string.length;

		str = new char[length + 1];
		str[length] = 0;
		memcpy(str, string.str, length);
	}

	noDelete = false;

	return *this;
}

String& String::operator=(String&& string) {
	if (this != &string) {
		delete[] str;
		length = string.length;

		str = string.str;

		string.length = 0;
		string.str = nullptr;
	}

	noDelete = false;

	return *this;
}

String& String::Append(const char character) {
	char* tmpstr = str;
	str = new char[length + 2];
	str[length+1] = 0;
	memcpy(str, tmpstr, length);
	str[length] = character;

	length++;

	delete[] tmpstr;

	return *this;
}

String& String::Append(const String& string) {
	uint_t newlen = length + string.length;
	char* tmpstr = str;
	str = new char[newlen + 1];
	str[newlen] = 0;
	memcpy(str, tmpstr, length);
	memcpy(str + length, string.str, string.length);
	length = newlen;

	delete[] tmpstr;

	return *this;
}

String& String::RemoveChars(const String& chars, bool iterate) {

	uint_t start = 0;

	uint_t failCount = 0;

	do {
		for (uint_t c = 0; c < chars.length; c++) {
			start = Find(chars[c]);
			if (start != (uint_t)-1) {
				Remove(start, start + 1);
				continue;
			}
			failCount++;
		}

		if (failCount >= chars.length) break;
		else failCount = 0;
	} while (iterate);

	return *this;
}

String& String::Remove(const String& string) {
	uint_t index = Find(string);
	if (index == (uint_t)-1) return *this;
	return Remove(index, index + string.length);
}
//TODO:
String& String::Remove(uint_t start, uint_t end) {
	uint_t len = end - start;
//	FD_ASSERT(len > length);
	uint_t newlen = length - len;

	char* tmp = str;

	str = new char[newlen + 1];
	memcpy(str, tmp, start);
	memcpy(str + start, tmp + start + len, newlen - start);

	length = newlen;
	str[length] = '\0';

	delete[] tmp;

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
	return String(str + start, end - start);
}

uint_t String::Count(const String& string, uint_t offset) const {
	uint_t total = length - string.length+1;
	if (total <= 0) return 0;
	uint_t res = 0;
	for (uint_t i = offset; i < total; i++) {
		bool match = true;
		for (uint_t j = 0; j < string.length; j++) {
			if (str[i + j] != string[j]) {
				match = false;
				break;
			}
		}

		if (match)res++;
	}

	return res;
}

char String::operator[](uint_t index) const {
	return str[index];
}

bool String::operator==(const String& string) const {
	if (length != string.length) return false;

	for (uint_t i = 0; i < length; i++) {
		if (str[i] != string.str[i]) return false;
	}

	return true;
}

bool String::operator!=(const String& string) const {
	if (length != string.length) return true;

	for (uint_t i = 0; i < length; i++) {
		if (str[i] == string.str[i]) return false;
	}

	return true;
}

String String::operator+(const String& string) const {
	return String(this).Append(string);
}

bool String::StartsWith(const String& string) const {
	if (length < string.length) return false;

	for (uint_t i = 0; i < string.length; i++)
		if (str[i] != string.str[i]) return false;

	return true;
}

bool String::EndsWith(const String& string) const {
	if (length < string.length) return false;

	for (uint_t i = 0; i < string.length; i++)
		if (str[length - i - 1] != string.str[string.length - i - 1]) return false;

	return true;
}

uint_t String::Find(const String& string, uint_t offset) const {
	if (length + offset < string.length) return (uint_t)-1;
	for (uint_t i = offset; i < length; i++) {
		bool match = true;
		for (uint_t j = 0; j < string.length; j++) {

			if (str[i + j] != string.str[j]) {
				match = false;
				break;
			}
		}

		if (match) {
			return i;
		}
	}

	return (uint_t)-1;
}

uint_t String::Find(const char c, uint_t offset) const {
	for (uint_t i = offset; i < length; i++) {
		if (str[i] == c) return i;
	}

	return (uint_t)-1;
}

List<String*> String::Split(const char delimiter) const {

	List<String*> list(128, 32);

	Split(delimiter, list);

	return list;
}

void String::Split(const char delimiter, List<String*>& list) const {
	uint_t lastindex = 0;

	for (uint_t i = 0; i < length; i++) {
		if (str[i] == delimiter) {

			list << new String(str + lastindex, i - lastindex);
			lastindex = i + 1;
		}
	}

	if (lastindex < length)
		list << new String(str + lastindex, length - lastindex);

}

}}