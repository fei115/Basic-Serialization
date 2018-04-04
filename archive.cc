#include "archive.h"
#include "serializable.h"
#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <cstdint>
#include <algorithm>

using namespace std;

void Archive::write(const void* buffer, size_t length)
{
	_stream.write((const char *)buffer, length);
}

void Archive::read(void* buffer, size_t length)
{
	_stream.read((char*)buffer, length);
}

Archive::Archive(): ignore_endianness(false) {}

Archive::Archive(const string& str): ignore_endianness(false)
{
	/* purposely not add null terminator. 
	 * str is simply an array of bytes.
	 */
	write(str.c_str(), str.length());
}

Archive::Archive(const char* buf, const uint32_t size): ignore_endianness(false)
{
	write(buf, size);
}

Archive::Archive(const vector<char>& buf): ignore_endianness(false)
{
	const char* arr = buf.data();
	write(arr, buf.size());
}

string Archive::str() const
{
	return _stream.str();
}

vector<char> Archive::bytes() const
{
	string str = _stream.str();
	vector<char> vec(str.begin(), str.end());
	return vec;
}

void Archive::arr(char* buf) const
{
	vector<char> v = bytes();
	copy(v.begin(), v.end(), buf);
}

uint32_t Archive::size() const
{
	return _stream.str().length();
}

Archive::~Archive() {}

Archive& Archive::operator<<(string val)
{
	uint32_t len = val.length();
	*this << len;
	_stream.write(val.c_str(), len);
	return *this;
}

Archive& Archive::operator>>(string& val)
{
	uint32_t length;
	*this >> length;
	char buffer[length + 1];
	read(buffer, length);
	buffer[length] = '\0';
	val = buffer;
	return *this;
}

Archive& Archive::operator<<(const Serializable& val)
{
	Archive a;
	val.serialize(a);
	vector<char> bytes = a.bytes();
	write(bytes.data(), bytes.size());
	return *this;
}

Archive& Archive::operator>>(Serializable& val)
{
	val.deserialize(*this);
	return *this;
}

