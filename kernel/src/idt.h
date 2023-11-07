#include <stdint.h>

#define IDT_MAX_DESCRIPTORS 			256

// definition of an interrupt gate
struct idt_entry_struct {
    uint16_t isr_low;
    uint16_t kernel_cs;
    uint8_t ist;
    uint8_t attributes;
    uint16_t isr_mid;
    uint32_t isr_high;
    uint32_t reserved
}__attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

// IDT descriptor pointer
struct idt_ptr_struct {
    uint16_t limit;
    uint64_t base;
}__attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

__attribute__((aligned(0x10)))
static idt_entry_t idt[IDT_MAX_DESCRIPTORS];

static idt_ptr_t idt_ptr;

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);
void idt_init(void);

