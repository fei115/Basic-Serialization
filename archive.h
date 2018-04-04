#ifndef ARCHIVE_H
#define ARCHIVE_H

#include "serializable.h"
#include <string>
#include <sstream>
#include <cstdint>
#include <climits>
#include <vector>
#include <list>
#include <iostream>

using namespace std;

class Archive

{
	private:
		stringstream _stream;
		bool ignore_endianness;

	private:
		/**
		 *	Writes the content in `buffer` to _stream. 
		 */
		void write(const void* buffer, size_t length);
		/**
		 *	Reads `length` bytes of content from _stream, and store it into buffer. 
		 */
		void read(void* buffer, size_t length);
		/**
		 * If the curren system is using small-endian, swap bytes.
		 * Otherwise, return the given value.
		 */
		template <typename T>
		T swap_endian(T u)
		{
			if (ignore_endianness) return u;

			const int i = 1;
			bool is_bigendian = (*(char*)&i) == 0;

			if (is_bigendian) return u;

			static_assert(CHAR_BIT == 8, "CHAR_BIT != 8");
			union
			{
				T u;
				unsigned char u8[sizeof(T)];
			} source, dest;

			source.u = u;
			for (size_t i = 0; i < sizeof(T); i++)
			{
				dest.u8[i] = source.u8[sizeof(T) - i - 1];
			}
			return dest.u;
		}

	public:
		/**
		 * Use this constructor when storing an object. i.e obj -> bytes
		 */
		Archive();

		/**
		 * Use these constructors when loading an object. i.e bytes -> obj
		 */
		 Archive(const string& str);
		 Archive(const char* buf, const uint32_t size);
		 Archive(const vector<char>& buf);

		/**
		 * Return a copy of all the stored content.
		 */
		string str() const;
		vector<char> bytes() const;
		void arr(char* buf) const;

		/**
		 * Return the size of result
		 */
		 uint32_t size() const;

		~Archive();

	public:
	/**
	 * Plain old data types
	 */
	#define ARCHIVE_FOR_POD(type) \
		Archive& operator<<(type val) \
		{ \
			val = swap_endian(val); \
			write(&val, sizeof(type)); \
			return *this; \
		} \
		Archive& operator>>(type& val) \
		{ \
			read(&val, sizeof(type)); \
			val = swap_endian(val); \
			return *this; \
		}

		ARCHIVE_FOR_POD(char);
		ARCHIVE_FOR_POD(short);
		ARCHIVE_FOR_POD(int);
		ARCHIVE_FOR_POD(long);
		ARCHIVE_FOR_POD(double);
		ARCHIVE_FOR_POD(float);
		ARCHIVE_FOR_POD(uint16_t);
		ARCHIVE_FOR_POD(uint32_t);

	public:
	/**
	 * Complex object types
	 */
	 	Archive& operator<<(string val);
		Archive& operator>>(string &val);

		Archive& operator<<(const Serializable& val);
		Archive& operator>>(Serializable& val);
	
		template<typename T>
		class NoSwap
		{
			public:
				T v;
				NoSwap(T val): v(val) {}
		};

		template<typename T>
		Archive& operator<<(NoSwap<T> val)
		{
			ignore_endianness = true;
			*this << val.v;
			ignore_endianness = false;
			return *this;
		}

		template<typename T>
		Archive& operator>>(NoSwap<T>& val)
		{
			ignore_endianness = true;
			T v;
			*this >> v;
			val.v = v;
			ignore_endianness = false;
			return *this;
		}
	
	public:
	/**
	 * Array of POD
	 */
		template <class T, size_t N>
		Archive& operator<<(T (&v)[N])
		{
			uint32_t length = N;
			*this << length;
			for(uint32_t i = 0; i < length; i++)
			{
				*this << v[i];
			}
			return *this;
		}

		template <class T, size_t N>
		Archive& operator>>(T (&v)[N])
		{
			uint32_t length;
			*this >> length;
			for(uint32_t i = 0; i < length; i++)
			{
				*this >> v[i];
			}
			return *this;
		}
	
	public:
	/**
	 * Standard template library(STL) container types
	 */
	#define ARCHIVE_FOR_STL(type) \
		template<typename T> \
		Archive& operator<<(const type<T>& c) \
		{ \
			uint32_t length = c.size(); \
			*this << length; \
			for(typename type<T>::const_iterator it = c.begin(); it != c.end(); it++) \
			{ \
				*this << *it; \
			} \
			return *this; \
		} \
		\
		template<typename T> \
		Archive& operator>>(type<T>& c) \
		{ \
			uint32_t length; \
			*this >> length; \
			for(uint32_t i = 0; i < length; i++) \
			{ \
				T value; \
				*this >> value; \
				c.push_back(value); \
			} \
			return *this; \
		}

		ARCHIVE_FOR_STL(vector);
		ARCHIVE_FOR_STL(list);
};

#endif

