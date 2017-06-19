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
#include "taskq.h"
#include "chan.h"

#define WRITE_COUNT 10000

int main(int argc, char *argv[])
{
    TaskQueue tq(1);
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
