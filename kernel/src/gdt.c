#include <gdt.h>

extern void gdt_flush(uint64_t);

static void init_gdt();
static void gdt_set_gate(int32_t, uint32_t, uint32_t, uint8_t, uint8_t);

gdt_entry_t gdt_entries[9];
gdt_ptr_t gdt_ptr;

void init_descriptor_tables() {
    init_gdt();
}

static void init_gdt() {
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 9) - 1;
    gdt_ptr.base = (uint64_t)&gdt_entries;


    gdt_set_gate(0, 0, 0, 0, 0);

    /* Ring0 - 16-bit Code Segment */
    gdt_set_gate(1, 0, 0xFFFF, 0x9A, 0xCF);
    /* Ring0 - 16-bit Data Segment */
    gdt_set_gate(2, 0, 0xFFFF, 0x92, 0xCF);

    /* Ring0 - 32-bit Code Segment */
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    /* Ring0 - 32-bit Data Segment */
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0x92, 0xCF);

    /* Kernel - 64-bit Code Segment */
    gdt_set_gate(5, 0, 0, 0x9A, 0x20);
    /* Kernel - 64-bit Data Segment */
    gdt_set_gate(6, 0, 0, 0x92, 0);

    /* User - 64-bit Code Segment */
    gdt_set_gate(7, 0, 0, 0xFA, 0x20);
    /* User - 64-bit Data Segment */
    gdt_set_gate(8, 0, 0, 0xF2, 0);

    gdt_flush((uint64_t)&gdt_ptr);
}

static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran){
    gdt_entries[num].base_low    = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high   = (base >> 24) & 0xFF;
    
    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity |= gran * 0xF0;
    gdt_entries[num].access = access;
}