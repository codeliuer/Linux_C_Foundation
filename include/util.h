/*************************************************************************
	> File Name: util.h
	> Function:
	> Author:
	> Mail:
	> Created Time: Sun 11 Dec 2022 08:22:16 PM HKT
 ************************************************************************/
#ifndef __UTIL_H__
#define __UTIL_H__

#define CACHE_LINE 64

#define UNUSED(v) ((void) (v))

#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#define INLINE __inline__ __attribute__((always_inline))

#define PAUSE() asm volatile("pause":::"memory")

#define EXCLUSIVE_CACHE_LINE __attribute__((aligned(CACHE_LINE)))

#define MIN(first, second) \
	({ \
		typeof(first) _first = (first); \
		typeof(second) _second = (second); \
		(_first > _second) ? _second : _first; \
	})
#define MAX(first, second) \
	({ \
		typeof(first) _first = (first); \
		typeof(second) _second = (second); \
		(_first > _second) ? _first : _second; \
	})

#endif
