#include <limine.h>
#include <stddef.h>
#include <monitor/monitor.h>

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

// Halt and catch fire function.
static void hcf(void) {
    asm ("cli");
    for (;;) {
        asm ("hlt");
    }
}

void monitor_write(const char msg[], int size) {
    if (framebuffer_request.response == NULL
        || framebuffer_request.response->framebuffer_count < 1) {
            hcf();
    }

    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];
    struct flanterm_context *ft_ctx = flanterm_fb_simple_init(
        framebuffer->address, framebuffer->width, framebuffer->height, framebuffer->pitch
    );

    flanterm_write(ft_ctx, msg, size);
}