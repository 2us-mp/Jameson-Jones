// kernel.c - minimal freestanding VGA text output

typedef unsigned short uint16_t;
typedef unsigned int   size_t;
typedef unsigned char  uint8_t;

static volatile uint16_t* const VGA_BUFFER = (uint16_t*)0xB8000;
static const uint8_t VGA_COLOR_LIGHT_GREY_ON_BLACK = 0x07;

void kmain(void) {
    const char* message = "Hello World";
    size_t i = 0;

    while (message[i] != '\0') {
        VGA_BUFFER[i] = ((uint16_t)VGA_COLOR_LIGHT_GREY_ON_BLACK << 8) | (uint16_t)message[i];
        i++;
    }

    for (;;) {
        __asm__ __volatile__("hlt");
    }
}
