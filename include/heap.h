/**
 * heap.h - The Binary Heap data structure
 */

#define HEAP_MIN_CAP        16

struct heap_operations;

struct heap {
    struct node *h_arr;
    unsigned int h_capacity;
    unsigned int h_size;
    struct heap_operations *h_op;
    int (*h_comparator)(struct node a, struct node b);
};

struct heap_operations {
    int (*add)(struct heap *this, struct node elem);
    struct node (*peek)(struct heap *this);
    struct node (*pop)(struct heap *this);
    long (*index)(struct heap *this, struct node elem);
    struct node (*find_at)(struct heap *this, unsigned int i);
    int (*remove)(struct heap *this, struct node elem);
    int (*contains)(struct heap *this, struct node elem);
    int (*__expand)(struct heap *this);
};

/*
 * Constructor and destructor
 */
struct heap * heap_init(unsigned int initial_cap, 
                        int (*comparator)(struct node a, struct node b));
void heap_destroy(struct heap *h);

/*
 * Heap operation functions
 */
int heap_add(struct heap *this, struct node elem);
struct node heap_peek(struct heap *this);
struct node heap_pop(struct heap *this);
long heap_index(struct heap *this, struct node elem);
struct node heap_find_at(struct heap *this, unsigned int i);
int heap_remove(struct heap *this, struct node elem);
int heap_contains(struct heap *this, struct node elem);
int heap_expand(struct heap *this); 

/*
 * Other utilities
 */

inline unsigned int heap_left_idx(unsigned int i)
{
    return (2 * i + 1);
}

inline unsigned int heap_right_idx(unsigned int i)
{
    return (2 * i + 2);
}

inline unsigned int heap_parent_idx(unsigned int i)
{
    if (i == 0) {
        return 0;
    }
    return (i - 1) / 2;
}

inline void swap(struct node *a, struct node *b)
{
    struct node tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

int default_minheap_comparator(struct node a, struct node b);
int default_maxheap_comparator(struct node a, struct node b);

