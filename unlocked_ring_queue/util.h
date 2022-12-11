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
#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#define BARRIER() __sync_synchronize()
#define PAUSE() asm volatile("pause":::"memory")
#define EXCLUSIVE_CACHE_LINE __attribute__((aligned(CACHE_LINE)))
#define INLINE __inline__ __attribute__((always_inline))
#define BCAS(p, o, n) __sync_bool_compare_and_swap(p, o, n)
#define MIN(first, second) \
	({ \
		typeof(first) _first = (first); \
		typeof(second) _second = (second); \
		_first > _second ? _second : _first; \
	})
#define MAX(first, second) \
	({
		typeof(first) _first = (first); \
		typeof(second) _second = (second); \
		_first > _second ? _first : _second; \
	})

#endif

