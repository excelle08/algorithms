/**
 * heap.c - The Heap data structure
 *
 */

#include <node.h>
#include <heap.h>
#include <stdio.h>
#include <stdlib.h>

struct heap_operations heap_ops = {
    .add = heap_add,
    .peek = heap_peek,
    .pop = heap_pop,
    .index = heap_index,
    .find_at = heap_find_at,
    .remove = heap_remove,
    .contains = heap_contains,
    .__expand = heap_expand
};

struct heap * heap_init(unsigned int initial_cap,
        int (*comparator)(struct node a, struct node b))
{
    struct heap *h = NULL;
    struct node *arr = NULL;
    initial_cap = (initial_cap > HEAP_MIN_CAP) ? initial_cap : HEAP_MIN_CAP;
    h = malloc(sizeof(struct heap));
    if (!h) {
        // Out of memory?
        return NULL;
    }
    arr = malloc(initial_cap * sizeof(struct node));
    if (!arr) {
        free(h);
        return NULL;
    }

    // Assemble the struct
    // 1. If user provided a custom comparator then use it.
    //    Otherwise use the default min heap comparator.
    if (comparator) {
        h->h_comparator = comparator;
    } else {
        h->h_comparator = default_minheap_comparator;
    }

    // 2. Assemble operations
    h->h_op = &heap_ops;

    // 3. Put values
    h->h_size = 0;
    h->h_capacity = initial_cap;
    h->h_arr = arr;

    return h;
}

void heap_destroy(struct heap *h)
{
    h->h_capacity = 0;
    h->h_size = 0;
    free(h->h_arr);
    free(h);
}

int heap_add(struct heap *this, struct node elem)
{
    if (this->h_size >= this->h_capacity) {
        // If full then try expand this heap
        if (!this->h_op->__expand(this)) {
            // Expansion failed...
            return -1;
        }
    }
    
    // First insert the element at the end,
    // then compare with its parents to see if such
    // insertion violates the heap's property.
    // If so, promote the inserted value as parent.
    //
    unsigned int i = this->h_size;
    this->h_size += 1;
    this->h_arr[i] = elem;

    while (i > 0) {
        unsigned int parent = heap_parent_idx(i);
        // Assume min heap:
        // If new element < parent ?
        // Float the new element up.
        if ( this->h_comparator(this->h_arr[i], this->h_arr[parent]) < 0 ) {
            swap(&this->h_arr[i], &this->h_arr[parent]);
            i = parent;
        } else {
            break;
        }
    }

    return 0;
}

struct node heap_peek(struct heap *this)
{
    if (this->h_size == 0) {
        struct node nul = {
            .i = 0
        };
        return nul;
    }
    return this->h_arr[0];
}

struct node heap_pop(struct heap *this)
{
    struct node retval = {
        .li = 0
    };
    if (this->h_size > 0) {
        retval = this->h_arr[0];
        this->h_op->remove(this, retval);
    }
    return retval;
}

long heap_index(struct heap *this, struct node elem)
{
    long idx = -1;
    for (unsigned int i = 0; i < this->h_size; i ++) {
        if (this->h_comparator(elem, this->h_arr[i]) == 0) {
            idx = i;
            break;
        }
    }
    return idx;
}

struct node heap_find_at(struct heap *this, unsigned int i)
{
    struct node retval = {
        .li = 0
    };
    if (i >= this->h_size) {
        return retval;
    }
    retval = this->h_arr[i];
    return retval;
}

static inline struct node *parent(struct heap *this, unsigned int i)
{
    if (i == 0) {
        return NULL;
    }
    unsigned int p_i = heap_parent_idx(i);
    if (p_i >= this->h_size) {
        return NULL;
    }
    return &this->h_arr[p_i];
}

static inline struct node *left(struct heap *this, unsigned int i)
{
    unsigned int left_i = heap_left_idx(i);
    if (left_i >= this->h_size) {
        return NULL;
    }
    return &this->h_arr[left_i];
}

static inline struct node *right(struct heap *this, unsigned int i)
{
    unsigned int right_i = heap_right_idx(i);
    if (right_i >= this->h_size) {
        return NULL;
    }
    return &this->h_arr[right_i];
}

static inline struct node *smaller_one(struct heap *this, struct node *a,
        struct node *b)
{
    if (!a && !b) {
        return NULL;
    }
    if (!a) {
        return b;
    }
    if (!b) {
        return a;
    }
    if (this->h_comparator(*a, *b) < 0) {
        return a;
    } else {
        return b;
    }
}

static inline unsigned int smaller_child_idx(struct heap *this, unsigned int i)
{
    unsigned int left = heap_left_idx(i);
    unsigned int right = heap_right_idx(i);
    unsigned int max = this->h_size - 1;
    if (left > max) {
        return 0;
    }
    if (right > max) {
        return left;
    }
    if (this->h_comparator(this->h_arr[left], this->h_arr[right]) < 0) { 
        return left;
    } else {
        return right;
    }
}

inline void p_heap(struct heap *this)
{
    printf(">>> ");
    for (int i = 0; i < this->h_size; i ++)
    {
        printf("%ld ", this->h_arr[i].li);
    }
    printf("\n");
}

int heap_remove(struct heap *this, struct node elem)
{
    long i = this->h_op->index(this, elem);
    if (i < 0) {
        // If the element does not exist?
        return -1;
    }
    // Opt: if it's the very last element, directly remove it
    if (i == this->h_size - 1) {
        this->h_size -= 1;
        this->h_arr[i].li = 0;
        return 0;
    }
    // Do the thing
    // 1. Swap arr[i] with arr[last]. The old arr[i] will be
    // put in the last slot and disposed.
    swap(&this->h_arr[i], &this->h_arr[this->h_size - 1]);
    this->h_size -= 1;
    // 2. Compare new arr[i] with its parent
    struct node *n_parent, *n_cur, *n_left, *n_right, *n_small;
    n_parent = parent(this, i);
    n_cur = &this->h_arr[i];
    while (n_parent && this->h_comparator(*n_cur, *n_parent) < 0) {
        // (Assume min heap)
        // If it's smaller than its parent, we should float it up.
        swap(n_cur, n_parent);
        n_cur = n_parent;
        i = heap_parent_idx(i);
        n_parent = parent(this, i);
    }
    n_left = left(this, i);
    n_right = right(this, i);
    while ((n_small = smaller_one(this, n_left, n_right)) &&
            this->h_comparator(*n_small, *n_cur) < 0)
    {
        // If the smaller child is less than this element, 
        // we should sink this element down.
        i = smaller_child_idx(this, i);
        swap(n_cur, n_small);
        n_cur = n_small;
        n_left = left(this, i);
        n_right = right(this, i);
    }

    return 0;
}

int heap_contains(struct heap *this, struct node elem)
{
    return this->h_op->index(this, elem) >= 0;
}

int heap_expand(struct heap *this)
{
    if (this->h_size <= this->h_capacity * 3 / 4) {
        // Not necessary if the heap is not filled up
        // for 75%!
        return -1;
    }
    if (realloc(this->h_arr, this->h_capacity * 2) == NULL) {
        return -1;
    } 
    return 0;
}

int default_minheap_comparator(struct node a, struct node b)
{
    return a.li - b.li;
}

int default_maxheap_comparator(struct node a, struct node b)
{
    return b.li - a.li;
}
