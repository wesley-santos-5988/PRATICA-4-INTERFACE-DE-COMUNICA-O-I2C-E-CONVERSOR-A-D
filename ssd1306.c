#include "ssd1306.h"
#include "font.h"
#include <string.h>

void ssd1306_init(ssd1306_t *ssd, uint8_t width, uint8_t height, bool external_vcc, uint8_t address, i2c_inst_t *i2c) {
    ssd->width = width;
    ssd->height = height;
    ssd->pages = height / 8U;
    ssd->address = address;
    ssd->i2c_port = i2c;
    ssd->external_vcc = external_vcc;
    ssd->bufsize = ssd->pages * ssd->width + 1;
    ssd->ram_buffer = calloc(ssd->bufsize, 1);
    ssd->ram_buffer[0] = 0x40; // Control byte for data
    ssd->port_buffer[0] = 0x80; // Control byte for command
}

static void ssd1306_command(ssd1306_t *ssd, uint8_t command) {
    ssd->port_buffer[1] = command;
    i2c_write_blocking(ssd->i2c_port, ssd->address, ssd->port_buffer, 2, false);
}

void ssd1306_config(ssd1306_t *ssd) {
    ssd1306_command(ssd, 0xAE); // Display OFF
    ssd1306_command(ssd, 0x20); // Set Memory Addressing Mode
    ssd1306_command(ssd, 0x00); // Horizontal Addressing Mode
    
    // --- COMANDOS ADICIONADOS PARA CORRIGIR A INVERSÃO DA TELA ---
    ssd1306_command(ssd, 0xA1); // Inverte o display horizontalmente (Segment Remap)
    ssd1306_command(ssd, 0xC8); // Inverte o display verticalmente (COM Output Scan Direction)
    // -------------------------------------------------------------

    ssd1306_command(ssd, 0x21); // Set Column Address
    ssd1306_command(ssd, 0x00); // Column Start Address
    ssd1306_command(ssd, ssd->width - 1); // Column End Address
    ssd1306_command(ssd, 0x22); // Set Page Address
    ssd1306_command(ssd, 0x00); // Page Start Address
    ssd1306_command(ssd, ssd->pages - 1); // Page End Address
    ssd1306_command(ssd, 0x8D); // Charge Pump
    ssd1306_command(ssd, ssd->external_vcc ? 0x10 : 0x14);
    ssd1306_command(ssd, 0xAF); // Display ON
}

void ssd1306_send_data(ssd1306_t *ssd) {
    i2c_write_blocking(ssd->i2c_port, ssd->address, ssd->ram_buffer, ssd->bufsize, false);
}

void ssd1306_fill(ssd1306_t *ssd, bool value) {
    memset(ssd->ram_buffer + 1, value ? 0xFF : 0x00, ssd->bufsize - 1);
}

void ssd1306_draw_pixel(ssd1306_t *ssd, uint8_t x, uint8_t y, bool value) {
    if (x >= ssd->width || y >= ssd->height) return;
    uint16_t index = (y / 8) * ssd->width + x + 1;
    if (value) ssd->ram_buffer[index] |= (1 << (y % 8));
    else       ssd->ram_buffer[index] &= ~(1 << (y % 8));
}

void ssd1306_draw_char(ssd1306_t *ssd, uint8_t x, uint8_t y, uint8_t scale, char c) {
    if (c < 32 || c > 126) return; // Fora do limite da fonte ASCII
    uint16_t font_idx = (c - 32) * 5;
    for (uint8_t i = 0; i < 5; i++) {
        uint8_t line = font[font_idx + i];
        for (uint8_t j = 0; j < 8; j++) {
            if (line & 0x1) {
                if (scale == 1) ssd1306_draw_pixel(ssd, x + i, y + j, true);
                else {
                    for (uint8_t sy = 0; sy < scale; sy++) {
                        for (uint8_t sx = 0; sx < scale; sx++) {
                            ssd1306_draw_pixel(ssd, x + (i * scale) + sx, y + (j * scale) + sy, true);
                        }
                    }
                }
            }
            line >>= 1;
        }
    }
}

void ssd1306_draw_string(ssd1306_t *ssd, uint8_t x, uint8_t y, uint8_t scale, const char *str) {
    uint8_t start_x = x;
    while (*str) {
        if (*str == '\n') {
            y += 8 * scale;
            x = start_x;
        } else {
            ssd1306_draw_char(ssd, x, y, scale, *str);
            x += 6 * scale; // Largura do caractere + espaço
        }
        str++;
    }
}