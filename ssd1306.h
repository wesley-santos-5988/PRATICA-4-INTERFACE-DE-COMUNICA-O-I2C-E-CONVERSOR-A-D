#ifndef SSD1306_H
#define SSD1306_H

#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Estrutura que guarda as configurações do display
typedef struct {
    uint8_t width, height, pages, address;
    i2c_inst_t *i2c_port;
    bool external_vcc;
    uint8_t *ram_buffer;
    size_t bufsize;
    uint8_t port_buffer[2];
} ssd1306_t;

// Declaração das funções
void ssd1306_init(ssd1306_t *ssd, uint8_t width, uint8_t height, bool external_vcc, uint8_t address, i2c_inst_t *i2c);
void ssd1306_config(ssd1306_t *ssd);
void ssd1306_send_data(ssd1306_t *ssd);
void ssd1306_fill(ssd1306_t *ssd, bool value);
void ssd1306_draw_pixel(ssd1306_t *ssd, uint8_t x, uint8_t y, bool value);
void ssd1306_draw_char(ssd1306_t *ssd, uint8_t x, uint8_t y, uint8_t scale, char c);
void ssd1306_draw_string(ssd1306_t *ssd, uint8_t x, uint8_t y, uint8_t scale, const char *str);

#endif