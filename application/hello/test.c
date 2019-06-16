/**
 * test.c - A simple test program to demonstrate nanotiming func
 * This is essentially an a + b program
 */
#include <nanotiming.h>
#include <stdio.h>
#include <stdlib.h>

struct test_args {
    int a;
    int b;
    int c;
};

int test(int a, int b)
{
    return a + b;
}

int test_wrapper(void *args)
{
    struct test_args *pars = (struct test_args *)args;
    if (!pars) {
        return -1;
    }
    pars->c = test(pars->a, pars->b);

    return 0;
}

int main(int argc, char **argv)
{
    struct test_args args = {
        .a = atoi(argv[1]),
        .b = atoi(argv[2]),
    };
    struct timespec time;

    time = benchmark_mt(test_wrapper, &args, 262144);

    printf("a + b = %d + %d = %d\n", args.a, args.b, args.c);
    printf("Time used: %ld.%09ld secs.\n", time.tv_sec, time.tv_nsec);
    return 0;
}
