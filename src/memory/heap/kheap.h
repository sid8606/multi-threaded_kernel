#ifndef KHEAP_H
#define KHEAP_H
#include "heap.h"
#include "kernel.h"

void kheap_init();
void* kalloc(size_t size);
void kfree(void *ptr);

#endif
