/*************************************************************************
	> File Name: ring.h
	> Function:
	> Author:
	> Mail:
	> Created Time: Sun 11 Dec 2022 08:01:28 PM HKT
 ************************************************************************/

#ifndef __RING_H__
#define __RING_H__

#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "util.h"

struct ring {
	struct reader {
		size_t cap;
		size_t mask;
		size_t head;
		size_t tail;
	} EXCLUSIVE_CACHE_LINE r;

	struct writer {
		size_t cap;
		size_t mask;
		size_t head;
		size_t tail;
	} EXCLUSIVE_CACHE_LINE w;

	void *data[];
};

extern struct ring *ring_init(size_t cap);
extern int ring_fini(struct ring *ring);

static INLINE size_t ring_write(struct ring *ring, void *data[], size_t nums)
{
	assert(ring && data);

	size_t head = 0;
	size_t tail = 0;
	size_t next = 0;

	do {
		head = ring->w.head;
		tail = ring->r.tail;
		BARRIER();

		nums = MIN(nums, ring->w.cap + tail - head);
		if (!nums) {
			return 0;
		}

		next = head + nums;
	} while (UNLIKELY(BCAS(&ring->w.head, head, next)));

	for (size_t i = 0; i < nums; i++) {
		ring->data[(head + i) & ring->w.mask] = data[i];
	}

	BARRIER();

	while (ring->w.tail != head) {
		PAUSE();
	}

	ring->w.tail = next;

	return nums;
}

static INLINE size_t ring_read(struct ring *ring, void *data[], size_t nums)
{
	assert(ring && data);

	size_t head = 0;
	size_t tail = 0;
	size_t next = 0;

	do {
		head = ring->r.head;
		tail = ring->w.tail;
		BARRIER();

		nums = MIN(nums, tail - head);
		if (!nums) {
			return 0;
		}

		next = head + nums;
	} while (UNLIKELY(BCAS(&ring->r.head, head, next)));

	for (size_t i = 0; i < nums; i++) {
		data[i] = ring->data[(head + i) & ring->r.mask];
	}

	BARRIER();

	while (ring->r.tail != head) {
		PAUSE();
	}

	ring->r.tail = next;

	return nums;
}

#endif