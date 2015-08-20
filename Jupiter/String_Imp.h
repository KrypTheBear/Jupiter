/**
 * Copyright (C) 2013-2015 Justin James.
 *
 * This license must be preserved.
 * Any applications, libraries, or code which make any use of any
 * component of this program must not be commercial, unless explicit
 * permission is granted from the original author. The use of this
 * program for non-profit purposes is permitted.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * In the event that this license restricts you from making desired use of this program, contact the original author.
 * Written by Justin James <justin.aj@hotmail.com>
 */

#if !defined _STRING_IMP_H_HEADER
#define _STRING_IMP_H_HEADER

/**
* @file String_Imp.h
* @brief Provides the implementations for String_Strict and String_Loose.
* Note: Modification of this file is not supported in any way.
*/

#include "String.h"

#if !defined va_copy

#if defined __INTEL_COMPILER
#pragma message("Warning: va_copy not properly defined. Assuming common implementation.")
#define va_copy(dst, src) ((void)((dst) = (src)))
#else
#error "va_copy not defined."
#endif // __INTEL_COMPILER

#endif // va_copy

#if !defined JUPITER_VSCPRINTF

#if defined _WIN32
#define JUPITER_VSCPRINTF(format, format_args) _vscprintf(format, format_args)
#define JUPITER_VSCWPRINTF(format, format_args) _vscwprintf(format, format_args)
#else // _WIN32
#define JUPITER_VSCPRINTF(format, format_args) vsnprintf(nullptr, 0, format, format_args)
#define JUPITER_VSCWPRINTF(format, format_args) vswprintf(nullptr, 0, format, format_args)
#endif // _WIN32

#endif // JUPITER_VSCPRINTF

/**
* IMPLEMENTATION:
*	String_Strict
*/

template<typename T> Jupiter::String_Strict<T>::String_Strict() : Jupiter::String_Strict<T>::String_Strict(size_t(0))
{
}

template<typename T> Jupiter::String_Strict<T>::String_Strict(size_t len)
{
	Jupiter::Shift_String_Type<T>::base = new T[len];
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	Jupiter::String_Type<T>::length = 0;
}

template<typename T> Jupiter::String_Strict<T>::String_Strict(Jupiter::String_Strict<T> &&source) : Jupiter::Shift_String_Type<T>(std::move(source))
{
}

template<typename T> Jupiter::String_Strict<T>::String_Strict(const Jupiter::String_Strict<T> &in) : Jupiter::String_Strict<T>::String_Strict(in.ptr(), in.size())
{
}

template<typename T> Jupiter::String_Strict<T>::String_Strict(const Jupiter::Readable_String<T> &in) : Jupiter::String_Strict<T>::String_Strict(in.ptr(), in.size())
{
}

template<typename T> Jupiter::String_Strict<T>::String_Strict(const std::basic_string<T> &in) : Jupiter::String_Strict<T>::String_Strict(in.data(), in.size())
{
}

template<typename T> Jupiter::String_Strict<T>::String_Strict(const T *in, size_t len) : Jupiter::String_Strict<T>::String_Strict(len)
{
	while (Jupiter::String_Type<T>::length != len)
	{
		Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = *in;
		Jupiter::String_Type<T>::length++;
		in++;
	}
}

template<typename T> Jupiter::String_Strict<T>::String_Strict(const T *in)
{
	if (in == nullptr) Jupiter::String_Type<T>::length = 0;
	else Jupiter::String_Type<T>::length = Jupiter::strlen<T>(in);

	Jupiter::Shift_String_Type<T>::base = new T[Jupiter::String_Type<T>::length];
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	for (size_t index = 0; index != Jupiter::String_Type<T>::length; index++, in++) Jupiter::String_Type<T>::str[index] = *in;
}

template<typename T> Jupiter::String_Strict<T>::String_Strict(const Jupiter::DataBuffer &in) : String_Strict(reinterpret_cast<T *>(in.getHead()), in.size() / sizeof(T))
{
}

template<typename T> Jupiter::String_Strict<T>::String_Strict(const Jupiter::Readable_String<T> &lhs, const Jupiter::Readable_String<T> &rhs) : String_Strict<T>(lhs.size() + rhs.size())
{
	const T *itr;
	const T *end;

	if (lhs.isNotEmpty())
	{
		itr = lhs.ptr();
		end = itr + lhs.size();
		*Jupiter::String_Type<T>::str = *itr;
		while (++itr != end)
			*++Jupiter::String_Type<T>::str = *itr;
		++Jupiter::String_Type<T>::str;
	}

	if (rhs.isNotEmpty())
	{
		itr = rhs.ptr();
		end = itr + rhs.size();
		*Jupiter::String_Type<T>::str = *itr;
		while (++itr != end)
			*++Jupiter::String_Type<T>::str = *itr;
		++Jupiter::String_Type<T>::str;
	}

	Jupiter::String_Type<T>::length = Jupiter::String_Type<T>::str - Jupiter::Shift_String_Type<T>::base;
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
}

// vformat()

template<> size_t inline Jupiter::String_Strict<char>::vformat(const char *format, va_list args)
{
	int minLen;
	va_list sargs;
	va_copy(sargs, args);
	minLen = JUPITER_VSCPRINTF(format, sargs);
	va_end(sargs);
	if (minLen < 0) return 0; // We simply can not work with this.

	this->setBufferSizeNoCopy(minLen + 1); // vsnprintf REQUIRES space for an additional null character.
	minLen = vsnprintf(Jupiter::String_Type<char>::str, minLen + 1, format, args);
	if (minLen < 0) return 0;
	return Jupiter::String_Type<char>::length = minLen;
}

template<> size_t inline Jupiter::String_Strict<wchar_t>::vformat(const wchar_t *format, va_list args)
{
	int minLen;
	va_list sargs;
	va_copy(sargs, args);
	minLen = JUPITER_VSCWPRINTF(format, sargs);
	va_end(sargs);
	if (minLen < 0) return 0; // We simply can not work with this.

	this->setBufferSizeNoCopy(minLen + 1); // vsnprintf REQUIRES space for an additional null character.
	minLen = vswprintf(Jupiter::String_Type<wchar_t>::str, minLen + 1, format, args);
	if (minLen < 0) return 0;
	return Jupiter::String_Type<wchar_t>::length = minLen;
}

template<typename T> size_t Jupiter::String_Strict<T>::vformat(const T *format, va_list args)
{
	return 0;
}

// avformat()

template<> size_t inline Jupiter::String_Strict<char>::avformat(const char *format, va_list args)
{
	int minLen;
	va_list sargs;
	va_copy(sargs, args);
	minLen = JUPITER_VSCPRINTF(format, sargs);
	va_end(sargs);
	if (minLen < 0) return 0; // We simply can not work with this.

	this->setBufferSize(Jupiter::String_Type<char>::length + minLen + 1); // vsnprintf REQUIRES space for an additional null character.
	minLen = vsnprintf(Jupiter::String_Type<char>::str + Jupiter::String_Type<char>::length, minLen + 1, format, args);
	if (minLen <= 0) return 0;
	Jupiter::String_Type<char>::length += minLen;
	return minLen;
}

template<> size_t inline Jupiter::String_Strict<wchar_t>::avformat(const wchar_t *format, va_list args)
{
	int minLen;
	va_list sargs;
	va_copy(sargs, args);
	minLen = JUPITER_VSCWPRINTF(format, sargs);
	va_end(sargs);
	if (minLen < 0) return 0; // We simply can not work with this.
	this->setBufferSize(minLen + Jupiter::String_Type<wchar_t>::length + 1); // vsnprintf REQUIRES space for an additional null character.

	minLen = vswprintf(Jupiter::String_Type<wchar_t>::str + Jupiter::String_Type<wchar_t>::length, minLen + 1, format, args);
	if (minLen <= 0) return 0;
	Jupiter::String_Type<wchar_t>::length += minLen;
	return minLen;
}

template<typename T> size_t Jupiter::String_Strict<T>::avformat(const T *format, va_list args)
{
	return 0;
}

template<typename T> Jupiter::String_Strict<T> Jupiter::String_Strict<T>::Format(const T *format, ...)
{
	String_Strict<T> r;
	va_list args;
	va_start(args, format);
	r.vformat(format, args);
	va_end(args);
	return r;
}

template<typename T> Jupiter::String_Strict<T> Jupiter::String_Strict<T>::substring(size_t pos) const
{
	return Jupiter::String_Strict<T>::substring(*this, pos);
}

template<typename T> Jupiter::String_Strict<T> Jupiter::String_Strict<T>::substring(size_t pos, size_t len) const
{
	return Jupiter::String_Strict<T>::substring(*this, pos, len);
}

template<typename T> Jupiter::String_Strict<T> Jupiter::String_Strict<T>::substring(const Jupiter::Readable_String<T> &in, size_t pos)
{
	return Jupiter::String_Type<T>::substring<Jupiter::String_Strict>(in, pos);
}

template<typename T> Jupiter::String_Strict<T> Jupiter::String_Strict<T>::substring(const T *in, size_t pos)
{
	return Jupiter::String_Type<T>::substring<Jupiter::String_Strict>(in, pos);
}

template<typename T> Jupiter::String_Strict<T> Jupiter::String_Strict<T>::substring(const Jupiter::Readable_String<T> &in, size_t pos, size_t len)
{
	return Jupiter::String_Type<T>::substring<Jupiter::String_Strict>(in, pos, len);
}

template<typename T> Jupiter::String_Strict<T> Jupiter::String_Strict<T>::substring(const T *in, size_t pos, size_t len)
{
	return Jupiter::String_Type<T>::substring<Jupiter::String_Strict>(in, pos, len);
}

template<typename T> Jupiter::String_Strict<T> Jupiter::String_Strict<T>::getWord(size_t pos, const T *whitespace) const
{
	return Jupiter::String_Strict<T>::getWord(*this, pos, whitespace);
}

template<typename T> Jupiter::String_Strict<T> Jupiter::String_Strict<T>::getWord(const Jupiter::Readable_String<T> &in, size_t pos, const T *whitespace)
{
	return Jupiter::Readable_String<T>::getWord<Jupiter::String_Strict>(in, pos, whitespace);
}

template<typename T> Jupiter::String_Strict<T> Jupiter::String_Strict<T>::getWord(const T *in, size_t pos, const T *whitespace)
{
	return Jupiter::Readable_String<T>::getWord<Jupiter::String_Strict>(in, pos, whitespace);
}

template<typename T> Jupiter::String_Strict<T> Jupiter::String_Strict<T>::getToken(size_t pos, const T &token) const
{
	return Jupiter::String_Strict<T>::getToken(*this, pos, token);
}

template<typename T> Jupiter::String_Strict<T> Jupiter::String_Strict<T>::getToken(size_t pos, const Jupiter::Readable_String<T> &token) const
{
	return Jupiter::String_Strict<T>::getToken(*this, pos, token);
}

template<typename T> Jupiter::String_Strict<T> Jupiter::String_Strict<T>::getToken(const Jupiter::Readable_String<T> &in, size_t pos, const T &token)
{
	return Jupiter::Readable_String<T>::getToken<Jupiter::String_Strict>(in, pos, token);
}

template<typename T> Jupiter::String_Strict<T> Jupiter::String_Strict<T>::getToken(const Jupiter::Readable_String<T> &in, size_t pos, const Jupiter::Readable_String<T> &token)
{
	return Jupiter::Readable_String<T>::getToken<Jupiter::String_Strict>(in, pos, token);
}

template<typename T> Jupiter::String_Strict<T> Jupiter::String_Strict<T>::gotoWord(size_t pos, const T *whitespace) const
{
	return Jupiter::String_Strict<T>::gotoWord(*this, pos, whitespace);
}

template<typename T> Jupiter::String_Strict<T> Jupiter::String_Strict<T>::gotoWord(const Jupiter::Readable_String<T> &in, size_t pos, const T *whitespace)
{
	return Jupiter::Readable_String<T>::gotoWord<Jupiter::String_Strict>(in, pos, whitespace);
}

template<typename T> Jupiter::String_Strict<T> Jupiter::String_Strict<T>::gotoToken(size_t pos, const T &token) const
{
	return Jupiter::String_Strict<T>::gotoToken(*this, pos, token);
}

template<typename T> Jupiter::String_Strict<T> Jupiter::String_Strict<T>::gotoToken(size_t pos, const Jupiter::Readable_String<T> &token) const
{
	return Jupiter::String_Strict<T>::gotoToken(*this, pos, token);
}

template<typename T> Jupiter::String_Strict<T> Jupiter::String_Strict<T>::gotoToken(const Jupiter::Readable_String<T> &in, size_t pos, const T &token)
{
	return Jupiter::Readable_String<T>::gotoToken<Jupiter::String_Strict>(in, pos, token);
}

template<typename T> Jupiter::String_Strict<T> Jupiter::String_Strict<T>::gotoToken(const Jupiter::Readable_String<T> &in, size_t pos, const Jupiter::Readable_String<T> &token)
{
	return Jupiter::Readable_String<T>::gotoToken<Jupiter::String_Strict>(in, pos, token);
}

// tokenize

template<typename T> typename Jupiter::Readable_String<T>::TokenizeResult<Jupiter::String_Strict> Jupiter::String_Strict<T>::tokenize(const T &separator) const
{
	return Jupiter::String_Strict<T>::tokenize(*this, separator);
}

template<typename T> typename Jupiter::Readable_String<T>::TokenizeResult<Jupiter::String_Strict> Jupiter::String_Strict<T>::tokenize(const Jupiter::Readable_String<T> &separator) const
{
	return Jupiter::String_Strict<T>::tokenize(*this, separator);
}

template<typename T> typename Jupiter::Readable_String<T>::TokenizeResult<Jupiter::String_Strict> Jupiter::String_Strict<T>::tokenize(const T *separator, size_t separator_size) const
{
	return Jupiter::String_Strict<T>::tokenize(*this, separator, separator_size);
}

template<typename T> typename Jupiter::Readable_String<T>::TokenizeResult<Jupiter::String_Strict> Jupiter::String_Strict<T>::tokenize(const Jupiter::Readable_String<T> &in, const T &token)
{
	return Jupiter::Readable_String<T>::tokenize<Jupiter::String_Strict>(in, token);
}

template<typename T> typename Jupiter::Readable_String<T>::TokenizeResult<Jupiter::String_Strict> Jupiter::String_Strict<T>::tokenize(const Jupiter::Readable_String<T> &in, const Jupiter::Readable_String<T> &separator)
{
	return Jupiter::Readable_String<T>::tokenize<Jupiter::String_Strict>(in, separator);
}

template<typename T> typename Jupiter::Readable_String<T>::TokenizeResult<Jupiter::String_Strict> Jupiter::String_Strict<T>::tokenize(const Jupiter::Readable_String<T> &in, const T *separator, size_t separator_size)
{
	return Jupiter::Readable_String<T>::tokenize<Jupiter::String_Strict>(in, separator, separator_size);
}

// Operators

template<typename T> inline Jupiter::String_Strict<T> Jupiter::String_Strict<T>::operator+(const Jupiter::String_Strict<T> &rhs) const
{
	return Jupiter::String_Strict<T>::operator+(reinterpret_cast<const Jupiter::Readable_String<T> &>(rhs));
}

template<typename T> inline Jupiter::String_Strict<T> Jupiter::String_Strict<T>::operator+(const Jupiter::Readable_String<T> &rhs) const
{
	return Jupiter::operator+(*this, rhs);
}

#if defined JUPITER_STRING_STRICT_OPERATOR_PLUS
template<typename T> static inline Jupiter::String_Strict<T> Jupiter::operator+(const Jupiter::Readable_String<T> &lhs, const Jupiter::Readable_String<T> &rhs)
{
	return Jupiter::String_Strict<T>(lhs, rhs);
}
#endif // JUPITER_STRING_STRICT_OPERATOR_PLUS

template<typename T> const Jupiter::String_Strict<T> Jupiter::String_Strict<T>::empty = Jupiter::String_Strict<T>();

// Jupiter::DataBuffer specialization

template<> struct _Jupiter_DataBuffer_partial_specialization_impl<Jupiter::String_Strict>
{
	template<typename Y> static void push(Jupiter::DataBuffer *buffer, const Jupiter::String_Strict<Y> *data)
	{
		_Jupiter_DataBuffer_partial_specialization_impl<Jupiter::Readable_String>::push<Y>(buffer, data);
	};

	template<typename Y> static Jupiter::String_Strict<Y> interpret(uint8_t *&head)
	{
		size_t size_ = *reinterpret_cast<size_t *>(head);
		head += sizeof(size_t);
		Jupiter::String_Strict<Y> r = Jupiter::String_Strict<Y>(reinterpret_cast<Y *>(head), size_);
		head += size_;
		return r;
	}
};

/**
* IMPLEMENTATION:
*	String_Loose
*/

template<typename T> Jupiter::String_Loose<T>::String_Loose() : Jupiter::String_Loose<T>::String_Loose(Jupiter::String_Loose<T>::start_size)
{
}

template<typename T> Jupiter::String_Loose<T>::String_Loose(Jupiter::String_Loose<T> &&source) : Jupiter::Shift_String_Type<T>(std::move(source))
{
	Jupiter::String_Loose<T>::strSize = source.strSize;
	source.strSize = 0;
}

template<typename T> Jupiter::String_Loose<T>::String_Loose(size_t len)
{
	if (len > Jupiter::String_Loose<T>::start_size) Jupiter::String_Loose<T>::strSize = len;
	else Jupiter::String_Loose<T>::strSize = Jupiter::String_Loose<T>::start_size;

	Jupiter::Shift_String_Type<T>::base = new T[Jupiter::String_Loose<T>::strSize];
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	Jupiter::String_Type<T>::length = 0;
}

template<typename T> Jupiter::String_Loose<T>::String_Loose(const Jupiter::String_Loose<T> &in)
{
	Jupiter::String_Loose<T>::strSize = in.strSize;
	Jupiter::Shift_String_Type<T>::base = new T[Jupiter::String_Loose<T>::strSize];
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
	for (Jupiter::String_Type<T>::length = 0; Jupiter::String_Type<T>::length != in.length; Jupiter::String_Type<T>::length++)
		Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = in.get(Jupiter::String_Type<T>::length);
}

template<typename T> Jupiter::String_Loose<T>::String_Loose(const Jupiter::Readable_String<T> &in) : Jupiter::String_Loose<T>::String_Loose(in.ptr(), in.size())
{
}

template<typename T> Jupiter::String_Loose<T>::String_Loose(const std::basic_string<T> &in) : Jupiter::String_Loose<T>::String_Loose(in.data(), in.size())
{
}

template<typename T> Jupiter::String_Loose<T>::String_Loose(const T *in, size_t len) : Jupiter::String_Loose<T>::String_Loose(len)
{
	while (Jupiter::String_Type<T>::length != len)
	{
		Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = *in;
		Jupiter::String_Type<T>::length++;
		in++;
	}
}

template<typename T> Jupiter::String_Loose<T>::String_Loose(const T *in)
{
	if (in == nullptr)
	{
		Jupiter::String_Loose<T>::strSize = Jupiter::String_Loose<T>::start_size;
		Jupiter::Shift_String_Type<T>::base = new T[Jupiter::String_Loose<T>::strSize];
		Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
		Jupiter::String_Type<T>::length = 0;
	}
	else
	{
		Jupiter::String_Type<T>::length = Jupiter::strlen<T>(in);

		Jupiter::String_Loose<T>::strSize = getPowerTwo32(Jupiter::String_Type<T>::length);
		if (Jupiter::String_Loose<T>::strSize < Jupiter::String_Loose<T>::start_size) Jupiter::String_Loose<T>::strSize = Jupiter::String_Loose<T>::start_size;

		Jupiter::Shift_String_Type<T>::base = new T[Jupiter::String_Loose<T>::strSize];
		Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
		for (Jupiter::String_Type<T>::length = 0; *in != 0; Jupiter::String_Type<T>::length++, in++) Jupiter::String_Type<T>::str[Jupiter::String_Type<T>::length] = *in;
	}
}

template<typename T> Jupiter::String_Loose<T>::String_Loose(const Jupiter::DataBuffer &in) : String_Loose(reinterpret_cast<T *>(in.getHead()), in.size() / sizeof(T))
{
}

template<typename T> Jupiter::String_Loose<T>::String_Loose(const Jupiter::Readable_String<T> &lhs, const Jupiter::Readable_String<T> &rhs) : String_Loose<T>(lhs.size() + rhs.size())
{
	const T *itr;
	const T *end;

	if (lhs.isNotEmpty())
	{
		itr = lhs.ptr();
		end = itr + lhs.size();
		*Jupiter::String_Type<T>::str = *itr;
		while (++itr != end)
			*++Jupiter::String_Type<T>::str = *itr;
		++Jupiter::String_Type<T>::str;
	}

	if (rhs.isNotEmpty())
	{
		itr = rhs.ptr();
		end = itr + rhs.size();
		*Jupiter::String_Type<T>::str = *itr;
		while (++itr != end)
			*++Jupiter::String_Type<T>::str = *itr;
		++Jupiter::String_Type<T>::str;
	}

	Jupiter::String_Type<T>::length = Jupiter::String_Type<T>::str - Jupiter::Shift_String_Type<T>::base;
	Jupiter::String_Type<T>::str = Jupiter::Shift_String_Type<T>::base;
}

template<typename T> bool Jupiter::String_Loose<T>::setBufferSize(size_t len)
{
	return Jupiter::Shift_String_Type<T>::setBufferSize(Jupiter::String_Loose<T>::strSize = getPowerTwo32(len));
}

template<typename T> bool Jupiter::String_Loose<T>::setBufferSizeNoCopy(size_t len)
{
	return Jupiter::Shift_String_Type<T>::setBufferSizeNoCopy(Jupiter::String_Loose<T>::strSize = getPowerTwo32(len));
}

// vformat()

template<> size_t inline Jupiter::String_Loose<char>::vformat(const char *format, va_list args)
{
	int minLen;
	va_list sargs;
	va_copy(sargs, args);
	minLen = JUPITER_VSCPRINTF(format, sargs);
	va_end(sargs);
	if (minLen < 0) return 0; // We simply can not work with this.

	this->setBufferSizeNoCopy(minLen + 1); // vsnprintf REQUIRES space for an additional null character.
	minLen = vsnprintf(Jupiter::String_Type<char>::str, minLen + 1, format, args);
	if (minLen < 0) return 0;
	return Jupiter::String_Type<char>::length = minLen;
}

template<> size_t inline Jupiter::String_Loose<wchar_t>::vformat(const wchar_t *format, va_list args)
{
	int minLen;
	va_list sargs;
	va_copy(sargs, args);
	minLen = JUPITER_VSCWPRINTF(format, sargs);
	va_end(sargs);
	if (minLen < 0) return 0; // We simply can not work with this.

	this->setBufferSizeNoCopy(minLen + 1); // vsnprintf REQUIRES space for an additional null character.
	minLen = vswprintf(Jupiter::String_Type<wchar_t>::str, minLen + 1, format, args);
	if (minLen < 0) return 0;
	return Jupiter::String_Type<wchar_t>::length = minLen;
}

template<typename T> size_t Jupiter::String_Loose<T>::vformat(const T *format, va_list args)
{
	return 0;
}

// avformat()

template<> size_t inline Jupiter::String_Loose<char>::avformat(const char *format, va_list args)
{
	int minLen;
	va_list sargs;
	va_copy(sargs, args);
	minLen = JUPITER_VSCPRINTF(format, sargs);
	va_end(sargs);
	if (minLen < 0) return 0; // We simply can not work with this.

	this->setBufferSize(Jupiter::String_Type<char>::length + minLen + 1); // vsnprintf REQUIRES space for an additional null character.
	minLen = vsnprintf(Jupiter::String_Type<char>::str + Jupiter::String_Type<char>::length, minLen + 1, format, args);
	if (minLen <= 0) return 0;
	Jupiter::String_Type<char>::length += minLen;
	return minLen;
}

template<> size_t inline Jupiter::String_Loose<wchar_t>::avformat(const wchar_t *format, va_list args)
{
	int minLen;
	va_list sargs;
	va_copy(sargs, args);
	minLen = JUPITER_VSCWPRINTF(format, sargs);
	va_end(sargs);
	if (minLen < 0) return 0; // We simply can not work with this.
	this->setBufferSize(minLen + Jupiter::String_Type<wchar_t>::length + 1); // vsnprintf REQUIRES space for an additional null character.

	minLen = vswprintf(Jupiter::String_Type<wchar_t>::str + Jupiter::String_Type<wchar_t>::length, minLen + 1, format, args);
	if (minLen <= 0) return 0;
	Jupiter::String_Type<wchar_t>::length += minLen;
	return minLen;
}

template<typename T> size_t Jupiter::String_Loose<T>::avformat(const T *format, va_list args)
{
	return 0;
}

template<typename T> Jupiter::String_Loose<T> Jupiter::String_Loose<T>::Format(const T *format, ...)
{
	String_Loose<T> r;
	va_list args;
	va_start(args, format);
	r.vformat(format, args);
	va_end(args);
	return r;
}

template<typename T> Jupiter::String_Loose<T> Jupiter::String_Loose<T>::substring(size_t pos) const
{
	return Jupiter::String_Loose<T>::substring(*this, pos);
}

template<typename T> Jupiter::String_Loose<T> Jupiter::String_Loose<T>::substring(size_t pos, size_t length) const
{
	return Jupiter::String_Loose<T>::substring(*this, pos, length);
}

template<typename T> Jupiter::String_Loose<T> Jupiter::String_Loose<T>::substring(const Jupiter::Readable_String<T> &in, size_t pos)
{
	return Jupiter::String_Type<T>::substring<Jupiter::String_Loose>(in, pos);
}

template<typename T> Jupiter::String_Loose<T> Jupiter::String_Loose<T>::substring(const T *in, size_t pos)
{
	return Jupiter::String_Type<T>::substring<Jupiter::String_Loose>(in, pos);
}

template<typename T> Jupiter::String_Loose<T> Jupiter::String_Loose<T>::substring(const Jupiter::Readable_String<T> &in, size_t pos, size_t len)
{
	return Jupiter::String_Type<T>::substring<Jupiter::String_Loose>(in, pos, len);
}

template<typename T> Jupiter::String_Loose<T> Jupiter::String_Loose<T>::substring(const T *in, size_t pos, size_t len)
{
	return Jupiter::String_Type<T>::substring<Jupiter::String_Loose>(in, pos, len);
}

template<typename T> Jupiter::String_Loose<T> Jupiter::String_Loose<T>::getWord(size_t pos, const T *whitespace) const
{
	return Jupiter::String_Loose<T>::getWord(*this, pos, whitespace);
}

template<typename T> Jupiter::String_Loose<T> Jupiter::String_Loose<T>::getWord(const Jupiter::Readable_String<T> &in, size_t pos, const T *whitespace)
{
	return Jupiter::Readable_String<T>::getWord<Jupiter::String_Loose>(in, pos, whitespace);
}

template<typename T> Jupiter::String_Loose<T> Jupiter::String_Loose<T>::getWord(const T *in, size_t pos, const T *whitespace)
{
	return Jupiter::Readable_String<T>::getWord<Jupiter::String_Loose>(in, pos, whitespace);
}

template<typename T> Jupiter::String_Loose<T> Jupiter::String_Loose<T>::getToken(size_t pos, const T &token)
{
	return Jupiter::String_Loose<T>::getToken(*this, pos, token);
}

template<typename T> Jupiter::String_Loose<T> Jupiter::String_Loose<T>::getToken(size_t pos, const Jupiter::Readable_String<T> &token)
{
	return Jupiter::String_Loose<T>::getToken(*this, pos, token);
}

template<typename T> Jupiter::String_Loose<T> Jupiter::String_Loose<T>::getToken(const Jupiter::Readable_String<T> &in, size_t pos, const T &token)
{
	return Jupiter::Readable_String<T>::getToken<Jupiter::String_Loose>(in, pos, token);
}

template<typename T> Jupiter::String_Loose<T> Jupiter::String_Loose<T>::getToken(const Jupiter::Readable_String<T> &in, size_t pos, const Jupiter::Readable_String<T> &token)
{
	return Jupiter::Readable_String<T>::getToken<Jupiter::String_Loose>(in, pos, token);
}

template<typename T> Jupiter::String_Loose<T> Jupiter::String_Loose<T>::gotoWord(size_t pos, const T *whitespace) const
{
	return Jupiter::String_Loose<T>::gotoWord(*this, pos, whitespace);
}

template<typename T> Jupiter::String_Loose<T> Jupiter::String_Loose<T>::gotoWord(const Jupiter::Readable_String<T> &in, size_t pos, const T *whitespace)
{
	return Jupiter::Readable_String<T>::gotoWord<Jupiter::String_Loose>(in, pos, whitespace);
}

template<typename T> Jupiter::String_Loose<T> Jupiter::String_Loose<T>::gotoToken(size_t pos, const T &token)
{
	return Jupiter::String_Loose<T>::gotoToken(*this, pos, token);
}

template<typename T> Jupiter::String_Loose<T> Jupiter::String_Loose<T>::gotoToken(size_t pos, const Jupiter::Readable_String<T> &token)
{
	return Jupiter::String_Loose<T>::gotoToken(*this, pos, token);
}

template<typename T> Jupiter::String_Loose<T> Jupiter::String_Loose<T>::gotoToken(const Jupiter::Readable_String<T> &in, size_t pos, const T &token)
{
	return Jupiter::Readable_String<T>::gotoToken<Jupiter::String_Loose>(in, pos, token);
}

template<typename T> Jupiter::String_Loose<T> Jupiter::String_Loose<T>::gotoToken(const Jupiter::Readable_String<T> &in, size_t pos, const Jupiter::Readable_String<T> &token)
{
	return Jupiter::Readable_String<T>::gotoToken<Jupiter::String_Loose>(in, pos, token);
}

// Operators

template<typename T> inline Jupiter::String_Loose<T> Jupiter::String_Loose<T>::operator+(const Jupiter::String_Loose<T> &rhs) const
{
	return Jupiter::String_Loose<T>::operator+(reinterpret_cast<const Jupiter::Readable_String<T> &>(rhs));
}

template<typename T> inline Jupiter::String_Loose<T> Jupiter::String_Loose<T>::operator+(const Jupiter::Readable_String<T> &rhs) const
{
	return Jupiter::operator+(*this, rhs);
}

// tokenize

template<typename T> typename Jupiter::Readable_String<T>::TokenizeResult<Jupiter::String_Loose> Jupiter::String_Loose<T>::tokenize(const T &separator)
{
	return Jupiter::String_Loose<T>::tokenize(*this, separator);
}

template<typename T> typename Jupiter::Readable_String<T>::TokenizeResult<Jupiter::String_Loose> Jupiter::String_Loose<T>::tokenize(const Jupiter::Readable_String<T> &separator)
{
	return Jupiter::String_Loose<T>::tokenize(*this, separator);
}

template<typename T> typename Jupiter::Readable_String<T>::TokenizeResult<Jupiter::String_Loose> Jupiter::String_Loose<T>::tokenize(const T *separator, size_t separator_size)
{
	return Jupiter::String_Loose<T>::tokenize(*this, separator, separator_size);
}

template<typename T> typename Jupiter::Readable_String<T>::TokenizeResult<Jupiter::String_Loose> Jupiter::String_Loose<T>::tokenize(const Jupiter::Readable_String<T> &in, const T &separator)
{
	return Jupiter::Readable_String<T>::tokenize<Jupiter::String_Loose>(in, separator);
}

template<typename T> typename Jupiter::Readable_String<T>::TokenizeResult<Jupiter::String_Loose> Jupiter::String_Loose<T>::tokenize(const Jupiter::Readable_String<T> &in, const Jupiter::Readable_String<T> &separator)
{
	return Jupiter::Readable_String<T>::tokenize<Jupiter::String_Loose>(in, separator);
}

template<typename T> typename Jupiter::Readable_String<T>::TokenizeResult<Jupiter::String_Loose> Jupiter::String_Loose<T>::tokenize(const Jupiter::Readable_String<T> &in, const T *separator, size_t separator_size)
{
	return Jupiter::Readable_String<T>::tokenize<Jupiter::String_Loose>(in, separator, separator_size);
}

#if !defined JUPITER_STRING_STRICT_OPERATOR_PLUS
#if !defined DISABLE_DEFAULT_JUPITER_STRING_OPERATOR_PLUS
template<typename T> static inline Jupiter::String_Loose<T> Jupiter::operator+(const Jupiter::Readable_String<T> &lhs, const Jupiter::Readable_String<T> &rhs)
{
	return Jupiter::String_Loose<T>(lhs, rhs);
}
#endif // DISABLE_DEFAULT_JUPITER_STRING_OPERATOR_PLUS
#endif // JUPITER_STRING_STRICT_OPERATOR_PLUS

template<typename T> const Jupiter::String_Loose<T> Jupiter::String_Loose<T>::empty = Jupiter::String_Loose<T>();

// Jupiter::DataBuffer specialization

template<> struct _Jupiter_DataBuffer_partial_specialization_impl<Jupiter::String_Loose>
{
	template<typename Y> static void push(Jupiter::DataBuffer *buffer, const Jupiter::String_Loose<Y> *data)
	{
		_Jupiter_DataBuffer_partial_specialization_impl<Jupiter::Readable_String>::push<Y>(buffer, data);
	};

	template<typename Y> static Jupiter::String_Loose<Y> interpret(uint8_t *&head)
	{
		size_t size_ = *reinterpret_cast<size_t *>(head);
		head += sizeof(size_t);
		Jupiter::String_Loose<Y> r = Jupiter::String_Loose<Y>(reinterpret_cast<T *>(head), size_);
		head += size_;
		return r;
	}
};

#endif // _STRING_IMP_H_HEADER