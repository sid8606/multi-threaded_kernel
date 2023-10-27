#include "idt.h"
#include "kernel.h"
#include "memory/memory.h"
#include "config.h"

struct idt_desc idt_descriptors[PEACHOS_TOTAL_INTERRUPTS];
struct idtr_desc idtr_descriptor;
/*
   struct int_desc {
   uint16_t offset_1;          // offset bits 0..15
   uint16_t selector;          // a code segment selector in GDT
   uint8_t zero;               // unused set to 0
   uint8_t type_attributes;    // gate type, dpl, and p fields
   uint16_t offset_2           // offset bits 16..31

   };
*/

/*
   struct idtr_dec {
   uint16_t limit; // Size of descriptor table -1
   uint32_t base;  // Base address of the start of the interrupt descriptor table 
   }
*/
extern void idt_load(struct idtr_desc* ptr);

void idt_zero()
{
	print("Devide by zero error\n");
}

void idt_set(int interrupt_no, void* address)
{
	struct idt_desc* desc = &idt_descriptors[interrupt_no];

	desc->offset_1 = (uint32_t) address & 0x0000ffff;
	desc->selector = KERNEL_CODE_SELECTOR;
	desc->zero = 0x00;
	desc->type_attributes = 0xEE;
	desc->offset_2 = (uint32_t) address >> 16;
}

void idt_init()
{
    memset(idt_descriptors, 0, sizeof(idt_descriptors));
	idtr_descriptor.limit = sizeof(idt_descriptors) - 1;
	idtr_descriptor.base = (uint32_t) idt_descriptors;

	idt_set(0, idt_zero);

    // load the interrupt descriptor table
    idt_load(&idtr_descriptor);

}
