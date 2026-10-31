#ifndef RECORDE_FLASH_H
#define RECORDE_FLASH_H

// Guarda o recorde na ULTIMA página/setor da flash, longe do programa,
// para sobreviver a reset, desligar a placa e até a regravar o .uf2.

#include <stdint.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/flash.h"
#include "hardware/flash.h"

#define RECORDE_FLASH_OFFSET (PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE)
#define RECORDE_MAGIC        0x47445247u   // "GDRG" - indica que há um recorde válido

typedef struct {
    uint32_t magic;
    uint32_t valor;
    uint32_t valor_inv;   // ~valor: checagem extra contra dado corrompido
} registro_recorde_t;

// Lê o recorde salvo; devolve 0 se a flash ainda não tem registro válido
static inline int recorde_carregar(void) {
    const registro_recorde_t *r =
        (const registro_recorde_t *)(XIP_BASE + RECORDE_FLASH_OFFSET);
    if (r->magic == RECORDE_MAGIC && r->valor == (uint32_t)~r->valor_inv && r->valor < 100000u) {
        return (int)r->valor;
    }
    return 0;
}

// Roda com a outra tarefa/IRQs pausadas (flash_safe_execute cuida disso)
static void recorde_gravar_cb(void *param) {
    const uint8_t *pagina = (const uint8_t *)param;
    flash_range_erase(RECORDE_FLASH_OFFSET, FLASH_SECTOR_SIZE);
    flash_range_program(RECORDE_FLASH_OFFSET, pagina, FLASH_PAGE_SIZE);
}

// Chamar só fora de uma rodada (timer parado). Leva ~50-100 ms.
static inline bool recorde_salvar(int valor) {
    static uint8_t pagina[FLASH_PAGE_SIZE];
    memset(pagina, 0xFF, sizeof(pagina));
    registro_recorde_t r = { RECORDE_MAGIC, (uint32_t)valor, ~(uint32_t)valor };
    memcpy(pagina, &r, sizeof(r));
    return flash_safe_execute(recorde_gravar_cb, pagina, 1000) == PICO_OK;
}

#endif // RECORDE_FLASH_H