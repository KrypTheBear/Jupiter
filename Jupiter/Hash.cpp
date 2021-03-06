/**
 * Copyright (C) 2015-2016 Jessica James.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Written by Jessica James <jessica.aj@outlook.com>
 */

#include "Hash.h"

/** Constants */
constexpr uint32_t JUPITER_FNV_1_32_OFFSET_BASIS = 2166136261UL;
constexpr uint64_t JUPITER_FNV_1_64_OFFSET_BASIS = 14695981039346656037ULL;
constexpr uint32_t JUPITER_FNV_1_32_PRIME = 16777619UL;
constexpr uint64_t JUPITER_FNV_1_64_PRIME = 1099511628211ULL;

/** Fowler-Noll-Vo */

uint64_t Jupiter::fnv1(const uint8_t *data, const uint8_t *end)
{
	uint64_t hash = JUPITER_FNV_1_64_OFFSET_BASIS;

	while (data != end)
	{
		hash = hash * JUPITER_FNV_1_64_PRIME;
		hash = hash ^ *data;
		++data;
	}

	return hash;
}

uint64_t Jupiter::fnv1a(const uint8_t *data, const uint8_t *end)
{
	uint64_t hash = JUPITER_FNV_1_64_OFFSET_BASIS;

	while (data != end)
	{
		hash = hash ^ *data;
		hash = hash * JUPITER_FNV_1_64_PRIME;
		++data;
	}

	return hash;
}

uint32_t Jupiter::fnv1_32(const uint8_t *data, const uint8_t *end)
{
	uint32_t hash = JUPITER_FNV_1_32_OFFSET_BASIS;

	while (data != end)
	{
		hash = hash * JUPITER_FNV_1_32_PRIME;
		hash = hash ^ *data;
		++data;
	}

	return hash;
}

uint32_t Jupiter::fnv1a_32(const uint8_t *data, const uint8_t *end)
{
	uint32_t hash = JUPITER_FNV_1_32_OFFSET_BASIS;

	while (data != end)
	{
		hash = hash ^ *data;
		hash = hash * JUPITER_FNV_1_32_PRIME;
		++data;
	}

	return hash;
}