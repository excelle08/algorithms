/**
 * node.h - Defines a basic data structure node
 * 
 * Works as an alternative of Template
 */

struct node {
    union {
        int i;
        unsigned int ui;
        long li;
        unsigned long uli;
        float f;
        double ff;
        void *p;
    };
};
