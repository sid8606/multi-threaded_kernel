#include <stdbool.h>
#include "heap.h"
#include "memory/memory.h"

bool heap_table_validate(void *ptr, void *end, struct heap_table *table)
{
	size_t table_size = (size_t)(end - ptr);
	int toatl_blocks = table_size / HEAP_BLOCK_SIZE;

	if (toatl_blocks != table->total) {
		return -EINVARG;
	}
	return OK;
}

bool heap_validate_alignment(void *ptr)
{
	return (unsigned int)ptr % HEAP_BLOCK_SIZE == 0;

}

int create_heap(struct heap *heap, void *ptr, void *end, struct heap_table *table)
{
	int res = OK;


	if(!heap_validate_alignment(ptr) || !heap_validate_alignment(end)) {
		return -EINVARG;
	}

	res = heap_table_validate(ptr,end,table);
	if (res < 0) {
		res = -EINVARG;
		goto out;
	}

	memset(heap, 0, sizeof(struct heap));

	heap->table = table;
	heap->saddr = ptr;

	size_t table_size = sizeof(HEAP_BLOCK_TABLE_ENTRY) * table->total;
	memset(table->entries, HEAP_BLOCK_TABLE_ENTRY_FREE, table_size);

out:
	return res;
}

int heap_align_value_to_upper(uint32_t value)
{
	if ((value % HEAP_BLOCK_SIZE) == 0) {
		return value;
	}

	value -= (value % HEAP_BLOCK_SIZE);
	value +=HEAP_BLOCK_SIZE;

	return value;
}


void * heap_block_to_address(struct heap *heap, uint32_t start_block)
{
	return (void *)(heap->saddr + (start_block * HEAP_BLOCK_SIZE));

}

uint32_t address_to_heap_block(struct heap *heap, void *address)
{
	return (uint32_t)(address - heap->saddr) / HEAP_BLOCK_SIZE;
}

bool heap_get_entry_type(HEAP_BLOCK_TABLE_ENTRY entry)
{
	return entry && 0x0f;
}
int heap_get_start_block(struct heap* heap, uint32_t total_blocks)
{
	int sb = -1;
	int cb = 0;
	struct heap_table *table = heap->table;

	for (int i = 0; i < table->total; i++) {
		if (heap_get_entry_type(table->entries[i]) != HEAP_BLOCK_TABLE_ENTRY_FREE) {
			sb = -1;
			cb = 0;
			continue;
		}

		if (sb == -1) {
			sb = i;
		}
		cb++;

		if (cb == total_blocks) {
			break;
		}
	}

	if (sb == -1) {
		return -ENOMEM;
	}
	return sb;
}
void heap_mark_blocks_taken(struct heap* heap, uint32_t start_block, uint32_t total_blocks)
{
	uint32_t end_block = start_block + total_blocks - 1;
	struct heap_table *table = heap->table;


	HEAP_BLOCK_TABLE_ENTRY entry = HEAP_BLOCK_IS_FIRST | HEAP_BLOCK_TABLE_ENTRY_TAKEN;

	if (total_blocks > 1) {
		entry |=HEAP_BLOCK_HAS_NEXT;
	}

	for (int i = start_block; i <= end_block; i++) {
		table->entries[i] = entry;
		entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN;
		if (i != end_block -1 ) {
		entry |=HEAP_BLOCK_HAS_NEXT;	
		}
	}
}
void* heap_kalloc_blocks(struct heap *heap, uint32_t total_blocks) {
	
	int start_block;
	void* address = NULL;

	start_block = heap_get_start_block(heap, total_blocks);
	if (start_block < 0) {
		goto out;
	}
	address = heap_block_to_address(heap, start_block);

	//Mark the blocks as the taken
	heap_mark_blocks_taken(heap, start_block, total_blocks);

out:
	return address;
}

void* heap_malloc(struct heap *heap, size_t size)
{
	size_t aligned_size = heap_align_value_to_upper(size);
	uint32_t total_blocks = aligned_size / HEAP_BLOCK_SIZE;

	return heap_kalloc_blocks(heap, total_blocks);



}

void heap_mark_blocks_free(struct heap *heap, uint32_t start_block)
{
	struct heap_table *table = heap->table;

	for (int i = start_block; i < (int)table->total; i++) {
		HEAP_BLOCK_TABLE_ENTRY entry = table->entries[i];
		table->entries[i] = HEAP_BLOCK_TABLE_ENTRY_FREE;
		if (!(entry & HEAP_BLOCK_HAS_NEXT)) {
			break;
		}
	}
}

void heap_free(struct heap *heap, void *ptr)
{
	heap_mark_blocks_free(heap, address_to_heap_block(heap, ptr));
}
