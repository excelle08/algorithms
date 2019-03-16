#include <node.h>
#include <heap.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    struct heap *heap;
    if (argc <= 1) {
        printf("No numbers received.\n");
        printf("Usage: %s <num1> <num2> ...\n", argv[0]);
        return 0;
    }

    heap = heap_init(argc - 1, NULL);
    if (!heap) {
        printf("Can't create heap: OOM?\n");
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        struct node n;
        n.li = atoi(argv[i]);
        heap->h_op->add(heap, n);
    }

    for (int i = 0; i < argc - 1; i++) {
        printf("%ld", heap->h_op->pop(heap).li);
        if (i < argc - 2) {
            printf(", ");
        }
    }
    printf("\n");

    heap_destroy(heap);
    return 0;
}
