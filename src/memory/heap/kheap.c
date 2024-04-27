#include "kheap.h"


struct heap_table kernel_heap_table;
struct heap kernel_heap;

void kheap_init()
{
	int total_table_entries = HEAP_SIZE_BYTES / HEAP_BLOCK_SIZE;
	kernel_heap_table.entries = (HEAP_BLOCK_TABLE_ENTRY*)HEAP_TABLE_ADDRESS;
	kernel_heap_table.total = total_table_entries;

	void * end = (void*)(HEAP_ADDRESS + HEAP_SIZE_BYTES);
	int res = create_heap(&kernel_heap, (void*)(HEAP_ADDRESS), end, &kernel_heap_table);

	if (res > 0) {
		print("Failed to create heap\n");
	}
}

void* kalloc(size_t size)
{
	return heap_malloc(&kernel_heap, size);
}

void kfree(void *ptr)
{
	heap_free(&kernel_heap, ptr);
}
