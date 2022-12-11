/*************************************************************************
	> File Name: ring.c
	> Function:
	> Author:
	> Mail:
	> Created Time: Sun 11 Dec 2022 07:57:45 PM HKT
 ************************************************************************/

#include <assert.h>

#include "ring.h"

struct ring *ring_init(size_t cap)
{
	assert(cap && !(cap & (cap - 1)));

	int ret = 0;
	struct ring *ring = NULL;
	size_t size = cap * sizeof(void *);

	ret = posix_memalign((void **)&ring, CACHE_LINE, sizeof(struct ring) + size);
	if (UNLIKELY(ret)) {
		return NULL;
	}

	ring->r.cap = ring.w.cap = cap;
	ring->r.mask = ring.w.mask = cap - 1;
	ring->r.head = ring.w.head = 0;
	ring->w.tail = ring.w.tail = 0;
	memset(ring->data, 0, size);

	return ring;
}

int ring_fini(struct ring *ring)
{
	assert(ring);

	if (ring->w.head != ring->w.tail
		|| ring->w.tail != ring->r.head
		|| ring->r.head != ring->r.tail) {
			return -1;
	}

	free(ring);

	return 0;
}