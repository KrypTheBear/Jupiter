/**
 * Copyright (C) 2014 Justin James.
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

#if !defined _READABLE_STRING_H_HEADER
#define _READABLE_STRING_H_HEADER

/**
 * @file Readable_String.h
 * @brief Defines several basic accessive and comparative virtual functions for strings.
 */

#include <cwchar> // wchar_t
#include <cstdio> // FILE
#include <string> // std::basic_string<T> type

namespace Jupiter
{

	/**
	* @brief Provides the basis for String classes by providing implementations for operators, comparative operations, and defining abstract functions.
	* Note: This is an abstract type.
	*
	* @param T Element type which the String will store. Defaults to char.
	*/
	template<typename T = char> class Readable_String
	{
	public:

		/**
		* @brief Fetches an element from the string.
		*
		* @param index Index of the element to return.
		* @return The element located at the specified index.
		*/
		virtual T &get(size_t index) const = 0;

		/**
		* @brief Returns the number of elements in the String.
		*
		* @return Number of elements in the string.
		*/
		virtual size_t size() const = 0;

		/**
		* @brief Returns a pointer to the underlying string of elements.
		*
		* @return Pointer to the underlying string of elements.
		*/
		virtual const T *ptr() const = 0;

		/**
		* @brief Checks if the string contains an element with the specified value.
		*
		* @param value Value of the element to search for.
		* @return True if a match is found, false otherwise.
		*/
		bool contains(const T &value) const;

		/**
		* @brief Compares another string against the String.
		*
		* @param in String to compare against.
		* @return 0 if the strings are equal, negative if the first mismatched character is greater in the String, or positive if it's less.
		*/
		int compare(const Readable_String<T> &in) const;
		int compare(const std::basic_string<T> &in) const;
		int compare(const T *in) const;
		int compare(const T &in) const;
		int compare(const std::nullptr_t) const;

		/**
		* @brief Checks if the strings are equal.
		* Note: Case sensitive.
		*
		* @param in String to compare against.
		* @return True if the contents of the strings are equal, false otherwise.
		*/
		bool equals(const Readable_String<T> &in) const;
		bool equals(const std::basic_string<T> &in) const;
		bool equals(const T *in) const;
		bool equals(const T &in) const;
		bool equals(const std::nullptr_t) const;

		/**
		* @brief Checks if the strings are equal.
		* Note: Case insensitive. Returns false for any type other than char and wchar_t.
		*
		* @param in String to compare against.
		* @return True if the contents of the strings are equal, false otherwise.
		*/
		bool equalsi(const Readable_String<T> &in) const;
		bool equalsi(const std::basic_string<T> &in) const;
		bool equalsi(const T *in) const;
		bool equalsi(const T &in) const;
		bool equalsi(const std::nullptr_t) const;

		/**
		* @brief Checks if the String matches a wildcard format.
		* Note: Case sensitive.
		*
		* @param format Format that the string is compared against.
		* @return True if the String matches the wildcard format, false otherwise.
		*/
		bool match(const Readable_String<T> &format) const;
		bool match(const std::basic_string<T> &format) const;
		bool match(const T *format) const;

		/**
		* @brief Checks if the CString matches a wildcard format.
		* Note: Case insensitive. Returns false for any type other than char and wchar_t.
		*
		* @param format Format that the string is compared against.
		* @return True if the CString matches the wildcard format, false otherwise.
		*/
		bool matchi(const Readable_String<T> &format) const;
		bool matchi(const std::basic_string<T> &format) const;
		bool matchi(const T *format) const;

		/**
		* @brief Counts the number of token deliminated words.
		*
		* @param whitespace A string of tokens used to deliminate words.
		* @return Number of words found.
		*/
		unsigned int wordCount(const T *whitespace) const;

		/**
		* @brief Interprets the string as an integer.
		* Note: This returns 0 on any value string type other than char.
		*
		* @param base Base of the string representation.
		* @return Integer representation of the string.
		*/
		int asInt(int base = 0) const;

		/**
		* @brief Interprets the string as an integer.
		* Note: This returns 0 on any value string type other than char.
		*
		* @param base Base of the string representation.
		* @return Integer representation of the string.
		*/
		unsigned int asUnsignedInt(int base = 0) const;

		/**
		* @brief Outputs the string to a FILE stream.
		*
		* @param out Stream to output to.
		* @return Number of elements written successfully.
		*/
		size_t print(FILE *out) const;
		size_t print(std::basic_ostream<T> &out) const;

		/**
		* @brief Outputs the string and a newline to a FILE stream
		*
		* @param out Stream to output to.
		* @param Number of elements written successfully.
		*/
		size_t println(FILE *out) const;
		size_t println(std::basic_ostream<T> &out) const;

		/** Access operator */
		inline T &operator[](size_t index) const { return this->get(index); };
	};
}

#include "Readable_String_Imp.h"

#endif // _READABLE_STRING_H_HEADER