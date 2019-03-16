#include <node.h>
#include <heap.h>
#include <nanotiming.h>

#include <stdio.h>
#include <stdlib.h>

struct demo_args {
    unsigned int nums;
    struct node *arr;
    struct heap *heap;
};

int demo_heapsort(unsigned int nums, struct node *arr, struct heap *heap);

int heapsort_benchmark_wrapper(void *args)
{
    if (!args) {
        return -1;
    }
    struct demo_args *pars = (struct demo_args *)args;

    return demo_heapsort(pars->nums,
            pars->arr, pars->heap);
}

int demo_heapsort(unsigned int nums, struct node *arr,
        struct heap *heap)
{
    for (int i = 0; i < nums; i++) {
        heap->h_op->add(heap, arr[i]);
    }
    for (int i = 0; i < nums; i++) {
        arr[i] = heap->h_op->pop(heap);
    }
    return 0;
}

int main(int argc, char **argv)
{
    unsigned int nums = 1048576;
    struct node *arr;
    struct heap *heap;
    struct demo_args args;
    struct timespec t;

    if (argc > 1) {
        nums = atoi(argv[1]);
        if (nums == 0) {
            nums = 1048576;
        }
    }

    // allocate array
    arr = malloc(nums * sizeof(struct node));
    if (!arr) {
        printf("Out of memory!\n");
        return 1;
    }
    // Create heap
    heap = heap_init(nums * sizeof(struct node), NULL);
    if (!heap) {
        printf("Cannot allocate heap.\n");
        free(arr);
        return 2;
    }
    // Populate random numbers
    printf("Generate random numbers...\n");
    srand(time(0));
    for (int i = 0; i < nums; i++) {
        arr[i].li = rand();
    }
    printf("Benchmark\n");
    args.nums = nums;
    args.arr = arr;
    args.heap = heap;
    t = benchmark(heapsort_benchmark_wrapper, &args);

    printf("Check correctness.\n");
    for (int i = 1; i < nums; i++) {
        if (heap->h_comparator(arr[i-1], arr[i]) > 0) {
            printf("Bug: sort error at pos %d.\n", i);
        }
    }

    printf("Finished. Time usage: %lu.%09ld secs.\n",
            t.tv_sec, t.tv_nsec);

    free(arr);
    heap_destroy(heap);
    return 0;
}
