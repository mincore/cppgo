/* ===================================================
 * Copyright (C) 2017 chenshuangping All Right Reserved.
 *      Author: mincore@163.com
 *    Filename: test.cpp
 *     Created: 2017-06-17 10:30
 * Description:
 * ===================================================
 */
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <atomic>
#include "taskq.h"
#include "chan.h"

#define WRITE_COUNT 10000

static std::atomic<int> gn(0);
static thread_local int g_localn = 0;

static void Init() {
    g_localn = gn++;
    printf("init localn: %d\n", g_localn);
}

static void Exit() {
    printf("exit localn: %d\n", (int)g_localn);
}

int main(int argc, char *argv[])
{
    TaskQueue tq(4, std::bind(&Init), std::bind(&Exit));
    Chan<int> c(10);

    tq.Push( [&c] {
        for (int i=0; i<WRITE_COUNT; i++) {
            c.Write(i);
        }
    });

    int preread = -1;
    for (int i=0; i<WRITE_COUNT; i++) {
        int n = c.Read();
        assert(n == preread+1);
        preread = n;
    }

    printf("test done\n");
    return 0;
}
