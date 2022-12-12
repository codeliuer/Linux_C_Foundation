/**********************************************************************
    > File Name: main.c
    > Function:
    > Author:
    > Mail:
    > Created Time: Sun Dec 11 19:57:20 HKT 2022
**********************************************************************/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#include "util.h"
#include "ring.h"

#define N 1024

static char *s_str[N] = {
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
};

static void *write_task(void *arg)
{
    struct ring *ring = (struct ring *)arg;

    for (int i = 0; i < 10; i++) {

    }
    int nums = ring_write(ring, s_str, N);

    pthread_exit(NULL);
}

static void *read_task(void *arg)
{
    struct ring *ring = (struct ring *)arg;

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    UNUSED(s_str);

    struct ring *ring = NULL;
    pthread_t read_id[4] = {0};
    pthread_t write_id[4] = {0};

    for (int i = 10; i < 1024; i++) {
        s_str[i] = s_str[i % 10];
    }

    ring = ring_init(1024 * 1024 * 128);

    for (int i = 0; i < 4; i++) {
        pthread_create(&write_id[i], NULL, write_task, ring);
        pthread_create(&read_id[i], NULL, read_task, ring);
    }

    for (int i = 0; i < 4; i++) {
        pthread_join(write_id[i], NULL);
        pthread_join(read_id[i], NULL);
    }

    ring_fini(ring);

    return EXIT_SUCCESS;
}
