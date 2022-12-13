/**********************************************************************
    > File Name: main.c
    > Function:
    > Author:
    > Mail:
    > Created Time: Sun Dec 11 19:57:20 HKT 2022
**********************************************************************/

#define _GNU_SOURCE
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sched.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sysinfo.h>

#include "util.h"
#include "ring.h"

#define TEST_N 1024
#define TEST_MAX 10000000000000UL

struct thread_counter {
    uint64_t total;
    int nums;
    uint64_t *counter[];
} EXCLUSIVE_CACHE_LINE;

struct task_info {
    int pthread_number;
    pthread_t thid;
    struct ring *ring;
    pthread_barrier_t *writer_wait;
    pthread_barrier_t *reader_wait;
    struct timespec ts;
    struct thread_counter *tc;
};

static char *s_str[TEST_N] = {
    "00000000000000000000",
    "11111111111111111111",
    "22222222222222222222",
    "33333333333333333333",
    "44444444444444444444",
    "55555555555555555555",
    "66666666666666666666",
    "77777777777777777777",
    "88888888888888888888",
    "99999999999999999999",
    "aaaaaaaaaaaaaaaaaaaa",
    "bbbbbbbbbbbbbbbbbbbb",
    "cccccccccccccccccccc",
    "dddddddddddddddddddd",
    "eeeeeeeeeeeeeeeeeeee",
    "ffffffffffffffffffff",
};

static void string_init(void)
{
    for (int i = 0; i < TEST_N; i++) {
        s_str[i] = s_str[i & 0xf];
    }
}

static void *write_task(void *arg)
{
    uint64_t nums = 0;
    uint64_t total = 0;
    struct task_info *info = (struct task_info *)arg;

    pthread_barrier_wait(info->writer_wait);
    clock_gettime(CLOCK_MONOTONIC, &info->ts);

    while (total < TEST_MAX) {
        nums = MIN((uint64_t)TEST_N, TEST_MAX - total);
        total += ring_write(info->ring, (void **)s_str, nums);
    }

    pthread_exit(NULL);
}

static void *read_task(void *arg)
{
    uint64_t nums = 0;
    uint64_t total = 0;
    uint64_t counter = 0;
    struct task_info *info = (struct task_info *)arg;

    UNUSED(nums);
    UNUSED(total);
    UNUSED(counter);
    UNUSED(info);

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    int ret = 0;
    int cpu_nums = get_nprocs();
    int writer_nums = (cpu_nums >> 1);
    int reader_nums = cpu_nums - writer_nums;
    struct ring *ring = NULL;
    pthread_barrier_t reader_wait;
    pthread_barrier_t writer_wait;
    struct task_info *info = NULL;
    struct thread_counter *tc;

    string_init();

    ring = ring_init(1024 * 1024 * 256);
    if (!ring) {
        return EXIT_FAILURE;
    }
    pthread_barrier_init(&reader_wait, NULL, cpu_nums - writer_nums);
    pthread_barrier_init(&writer_wait, NULL, writer_nums);

    info = malloc(cpu_nums * sizeof(struct task_info));
    if (UNLIKELY(!info)) {
        goto _cleanup;
    }

    ret = posix_memalign((void **)&tc, CACHE_LINE, sizeof(*tc) + reader_nums * sizeof(uint64_t *));
    if (UNLIKELY(ret)) {
        goto _cleanup;
    }

    tc->total = reader_nums * TEST_MAX;
    tc->nums = reader_nums;
    memset(tc->counter, 0, reader_nums * sizeof(uint64_t *));

    for (int i = 0; i < cpu_nums; i++) {
        info[i].pthread_number = i;
        info[i].ring = ring;
        info[i].writer_wait = &writer_wait;
        info[i].reader_wait = &reader_wait;
        info[i].tc = (i < writer_nums ? NULL : tc);
    }

    for (int i = 0; i < writer_nums; i++) {
        cpu_set_t set;
        CPU_ZERO(&set);
        CPU_SET(i, &set);
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setaffinity_np(&attr, sizeof(set), &set);
        pthread_create(&info[i].thid, &attr, write_task, &info[i]);
        pthread_attr_destroy(&attr);
    }

    for (int i = writer_nums; i < cpu_nums; i++) {
        cpu_set_t set;
        CPU_ZERO(&set);
        CPU_SET(i, &set);
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setaffinity_np(&attr, sizeof(set), &set);
        pthread_create(&info[i].thid, &attr, read_task, &info[i]);
        pthread_attr_destroy(&attr);
    }

    for (int i = 0; i < cpu_nums; i++) {
        pthread_join(info[i].thid, NULL);
    }

    return EXIT_SUCCESS;

_cleanup:
    ring_fini(ring);

    pthread_barrier_destroy(&reader_wait);
    pthread_barrier_destroy(&writer_wait);

    if (info) {
        free(info);
    }
    if (tc) {
        free(tc);
    }

    return EXIT_FAILURE;
}
