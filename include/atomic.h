/*************************************************************************
	> File Name: atomic.h
	> Function:
	> Author:
	> Mail:
	> Created Time: Tue 13 Dec 2022 02:18:14 AM HKT
 ************************************************************************/
#ifndef __ATOMIC_H__
#define __ATOMIC_H__

#define ATOMIC_FETCH_AND_ADD(ptr, v) __sync_fetch_and_add(ptr, v)
#define ATOMIC_FETCH_AND_SUB(ptr, v) __sync_fetch_and_sub(ptr, v)
#define ATOMIC_FETCH_AND_OR(ptr, v) __sync_fetch_and_or(ptr, v)
#define ATOMIC_FETCH_AND_AND(ptr, v) __sync_fetch_and_and(ptr, v)
#define ATOMIC_FETCH_AND_XOR(ptr, v) __sync_fetch_and_xor(ptr, v)
#define ATOMIC_FETCH_AND_NAND(ptr, v) __sync_fetch_and_nand(ptr, v)

#define ATOMIC_ADD_AND_FETCH(ptr, v) __sync_add_and_fetch(ptr, v)
#define ATOMIC_SUB_AND_FETCH(ptr, v) __sync_sub_and_fetch(ptr, v)
#define ATOMIC_OR_AND_FETCH(ptr, v) __sync_or_and_fetch(ptr, v)
#define ATOMIC_AND_AND_FETCH(ptr, v) __sync_and_and_fetch(ptr, v)
#define ATOMIC_XOR_AND_FETCH(ptr, v) __sync_xor_and_fetch(ptr, v)
#define ATOMIC_nand_and_fetch(ptr, v) __sync_nand_and_fetch(ptr, v)

#define ATOMIC_CAS(p, o, n) __sync_val_compare_and_swap(p, o, n)
#define ATOMIC_BCAS(p, o, n) __sync_bool_compare_and_swap(p, o, n)

#define ATOMIC_TEST_AND_SET(ptr, v) __sync_lock_test_and_set(ptr, v)
#define ATOMIC_RELEASE(ptr) __sync_lock_release(ptr)

#define ATOMIC_BARRIER() __sync_synchronize()

#endif
