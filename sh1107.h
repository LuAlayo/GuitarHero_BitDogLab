#ifndef SH1107_H
#define SH1107_H

#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware_v7.h"

#define SH1107_WIDTH   128
#define SH1107_HEIGHT  128
#define SH1107_PAGES   (SH1107_HEIGHT / 8)
#define SH1107_BUF_LEN (SH1107_WIDTH * SH1107_PAGES)

static uint8_t oled_buffer[SH1107_BUF_LEN];

// Tabela de caracteres simples 5x8 (Dígitos, Letras e Símbolos)
static const uint8_t font5x8[][5] = {
    [' '] = {0x00, 0x00, 0x00, 0x00, 0x00},
    ['-'] = {0x08, 0x08, 0x08, 0x08, 0x08},
    ['|'] = {0x00, 0x00, 0x7F, 0x00, 0x00},
    [':'] = {0x00, 0x36, 0x36, 0x00, 0x00},
    ['0'] = {0x3E, 0x51, 0x49, 0x45, 0x3E},
    ['1'] = {0x00, 0x42, 0x7F, 0x40, 0x00},
    ['2'] = {0x42, 0x61, 0x51, 0x49, 0x46},
    ['3'] = {0x21, 0x41, 0x45, 0x4B, 0x31},
    ['4'] = {0x18, 0x14, 0x12, 0x7F, 0x10},
    ['5'] = {0x27, 0x45, 0x45, 0x45, 0x39},
    ['A'] = {0x7C, 0x12, 0x11, 0x12, 0x7C},
    ['B'] = {0x7F, 0x49, 0x49, 0x49, 0x36},
    ['C'] = {0x3E, 0x41, 0x41, 0x41, 0x22},
    ['D'] = {0x7F, 0x41, 0x41, 0x22, 0x1C},
    ['E'] = {0x7F, 0x49, 0x49, 0x49, 0x41},
    ['G'] = {0x3E, 0x41, 0x49, 0x49, 0x7A},
    ['H'] = {0x7F, 0x08, 0x08, 0x08, 0x7F},
    ['I'] = {0x00, 0x41, 0x7F, 0x41, 0x00},
    ['J'] = {0x20, 0x40, 0x41, 0x3F, 0x01},
    ['M'] = {0x7F, 0x02, 0x0C, 0x02, 0x7F},
    ['N'] = {0x7F, 0x04, 0x08, 0x10, 0x7F},
    ['O'] = {0x3E, 0x41, 0x41, 0x41, 0x3E},
    ['P'] = {0x7F, 0x09, 0x09, 0x09, 0x06},
    ['R'] = {0x7F, 0x09, 0x19, 0x29, 0x46},
    ['S'] = {0x46, 0x49, 0x49, 0x49, 0x31},
    ['T'] = {0x01, 0x01, 0x7F, 0x01, 0x01},
    ['U'] = {0x3F, 0x40, 0x40, 0x40, 0x3F},
    ['V'] = {0x1F, 0x20, 0x40, 0x20, 0x1F},
    ['X'] = {0x63, 0x14, 0x08, 0x14, 0x63},
};

static inline void sh1107_write_cmd(uint8_t cmd) {
    uint8_t buf[2] = {0x00, cmd};
    i2c_write_blocking(OLED_I2C_PORT, OLED_ADDR, buf, 2, false);
}

// Inicialização com os comandos validados para a BitDogLab V7
static inline void sh1107_init(void) {
    const uint8_t init_cmds[] = {
        0xAE,             // Display OFF
        0xD5, 0x50,       // Clock divide
        0x81, 0x80,       // Contraste padrão
        0xA8, 0x7F,       // Multiplex ratio 128
        0xD3, 0x00,       // Display offset: 0x00 (Validado no BIH)
        0xAD, 0x8B,       // DC-DC control
        0xA0,             // Orientação do segmento
        0xC0,             // Orientação COM
        0xD9, 0x22,       // Pre-charge
        0xDB, 0x35,       // VCOM deselect
        0xA4,             // Saída inteira normal
        0xA6,             // Display não invertido
        0xAF              // Display ON
    };
    for (size_t i = 0; i < sizeof(init_cmds); i++) {
        sh1107_write_cmd(init_cmds[i]);
    }
    memset(oled_buffer, 0, SH1107_BUF_LEN);
}

static inline void sh1107_clear(void) {
    memset(oled_buffer, 0, SH1107_BUF_LEN);
}

static inline void sh1107_draw_pixel(int x, int y, uint8_t color) {
    if (x < 0 || x >= SH1107_WIDTH || y < 0 || y >= SH1107_HEIGHT) return;
    int page = y / 8;
    int bit = y % 8;
    int idx = page * SH1107_WIDTH + x;
    if (color) oled_buffer[idx] |= (1u << bit);
    else       oled_buffer[idx] &= ~(1u << bit);
}

static inline void sh1107_draw_vline(int x, int y0, int y1, uint8_t color) {
    for (int y = y0; y <= y1; y++) sh1107_draw_pixel(x, y, color);
}

static inline void sh1107_draw_hline(int x0, int x1, int y, uint8_t color) {
    for (int x = x0; x <= x1; x++) sh1107_draw_pixel(x, y, color);
}

static inline void sh1107_fill_rect(int x, int y, int w, int h, uint8_t color) {
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            sh1107_draw_pixel(x + i, y + j, color);
        }
    }
}

static inline void sh1107_draw_char(int x, int y, char c, uint8_t color) {
    if (c < ' ' || c > 'X') return;
    for (int col = 0; col < 5; col++) {
        uint8_t line = font5x8[(uint8_t)c][col];
        for (int row = 0; row < 8; row++) {
            if (line & (1u << row)) {
                sh1107_draw_pixel(x + col, y + row, color);
            }
        }
    }
}

static inline void sh1107_draw_string(int x, int y, const char *str, uint8_t color) {
    while (*str) {
        sh1107_draw_char(x, y, *str++, color);
        x += 6; // Largura do caractere (5) + 1 espaço
    }
}

// Atualização completa por páginas conforme especificação validada no BIH
static inline void sh1107_show(void) {
    uint8_t data_chunk[33];
    data_chunk[0] = 0x40; // Prefixo de dados I2C

    for (uint8_t page = 0; page < SH1107_PAGES; page++) {
        sh1107_write_cmd(0xB0 | page); // Seleção da página
        sh1107_write_cmd(0x00);        // Coluna inferior (0x00)
        sh1107_write_cmd(0x10);        // Coluna superior (0x10)

        // Envio dos 128 bytes da página em blocos de 32 bytes
        for (int chunk = 0; chunk < 4; chunk++) {
            memcpy(&data_chunk[1], &oled_buffer[page * SH1107_WIDTH + chunk * 32], 32);
            i2c_write_blocking(OLED_I2C_PORT, OLED_ADDR, data_chunk, 33, false);
        }
    }
}

#endif // SH1107_H