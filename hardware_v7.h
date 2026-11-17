// ===============================================================
// PROJETO 2 - EA801 - Laboratório Projeto de Sistemas Embarcados
// Luíza Maria Cabel Alayo, RA: 243537
// Rafael Mattos Sacramento, RA: 247349
// ===============================================================

#ifndef HARDWARE_V7_H
#define HARDWARE_V7_H

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "hardware/structs/timer.h"

// ============================================================
// MAPEAMENTO DE HARDWARE - BITDOGLAB V7
// ============================================================
// Botões Onboard (Ativos em nível BAIXO / GND)
#define BTN_A_PIN           5
#define BTN_B_PIN           6
#define BTN_C_PIN           10

// Display OLED SH1107 128x128 Onboard (I2C1)
#define OLED_I2C_PORT       i2c1
#define OLED_SDA_PIN        2
#define OLED_SCL_PIN        3
#define OLED_ADDR           0x3C
#define OLED_BAUDRATE       400000

// Sensor Touch Capacitivo 4 Vias (Conector IDC de expansão)
#define TOUCH_CH1_PIN       16  // Corda 1
#define TOUCH_CH2_PIN       17  // Corda 2
#define TOUCH_CH3_PIN       18  // Corda 3
#define TOUCH_CH4_PIN       19  // Corda 4

// Buzzer e LEDs auxiliares
#define BUZZER_PIN          21
#define LED_RGB_R           13
#define LED_RGB_G           11
#define LED_RGB_B           12

// ============================================================
// NOTAS MUSICAIS (Frequências em Hz - 4ª e 5ª Oitavas)
// ============================================================
#define NOTA_DO4            262
#define NOTA_RE4            294
#define NOTA_MI4            330
#define NOTA_FA4            349
#define NOTA_FAS4           370 // Fá#4 (Dancing With Myself / American Idiot)
#define NOTA_SOL4           392
#define NOTA_SOLS4          415 // Sol#4 / Láb4 (Chicleteiro / American Idiot)
#define NOTA_LA4            440
#define NOTA_LAS4           466 // Lá#4 / Sib4 (American Idiot)
#define NOTA_SI4            494
#define NOTA_DO5            523
#define NOTA_DOS5           554 // Dó#5 / Réb5 (Ramones / American Idiot)
#define NOTA_RE5            587
#define NOTA_RES5           622 // Ré#5 / Mib5 (American Idiot)
#define NOTA_MI5            659

// ============================================================
// REGISTRADORES DO TIMER DO RP2040 (BAIXO NÍVEL)
// ============================================================
#define GAME_ALARM_NUM      0
#define GAME_ALARM_IRQ      TIMER0_IRQ_0

static volatile uint32_t periodo_tick_us = 125000;
static volatile bool tick_flag = false;

static void isr_timer_baixo_nivel(void) {
    timer_hw->intr = (1u << GAME_ALARM_NUM);
    timer_hw->alarm[GAME_ALARM_NUM] = timer_hw->timelr + periodo_tick_us;
    tick_flag = true;
}

static inline void timer_baixo_nivel_init(uint32_t periodo_us) {
    periodo_tick_us = periodo_us;
    irq_set_exclusive_handler(GAME_ALARM_IRQ, isr_timer_baixo_nivel);
    irq_set_enabled(GAME_ALARM_IRQ, true);
    timer_hw->inte |= (1u << GAME_ALARM_NUM);
    timer_hw->alarm[GAME_ALARM_NUM] = timer_hw->timelr + periodo_tick_us;
}

static inline void timer_baixo_nivel_stop(void) {
    timer_hw->inte &= ~(1u << GAME_ALARM_NUM);
    irq_set_enabled(GAME_ALARM_IRQ, false);
}

// ============================================================
// MOTOR DE ÁUDIO PWM (BUZZER PASSIVO - GPIO21)
// ============================================================
static inline void buzzer_init_pwm(void) {
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_set_enabled(slice_num, false);
}

static inline void buzzer_tocar_nota(uint32_t freq_hz) {
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    if (freq_hz == 0) {
        pwm_set_enabled(slice_num, false);
        return;
    }
    // Clock padrão do RP2040 = 125 MHz. Divisor 125.0f -> 1 MHz (1 us por contagem)
    uint32_t clock_sys = clock_get_hz(clk_sys);
    float clkdiv = (float)clock_sys / 1000000.0f;
    uint32_t wrap = (1000000u / freq_hz) - 1u;

    pwm_set_clkdiv(slice_num, clkdiv);
    pwm_set_wrap(slice_num, wrap);
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(BUZZER_PIN), wrap / 2); // 50% duty cycle
    pwm_set_enabled(slice_num, true);
}

static inline void buzzer_parar(void) {
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_set_enabled(slice_num, false);
}

// ============================================================
// INICIALIZAÇÃO GERAL DOS PERIFÉRICOS
// ============================================================
static inline void bitdoglab_init_hardware(void) {
    stdio_init_all();

    // 1. Configuração dos Botões com Pull-Up interno
    uint botoes[] = {BTN_A_PIN, BTN_B_PIN, BTN_C_PIN};
    for (int i = 0; i < 3; i++) {
        gpio_init(botoes[i]);
        gpio_set_dir(botoes[i], GPIO_IN);
        gpio_pull_up(botoes[i]);
    }

    // 2. Configuração dos pinos do Touch com Pull-Down interno
    uint touch_pins[] = {TOUCH_CH1_PIN, TOUCH_CH2_PIN, TOUCH_CH3_PIN, TOUCH_CH4_PIN};
    for (int i = 0; i < 4; i++) {
        gpio_init(touch_pins[i]);
        gpio_set_dir(touch_pins[i], GPIO_IN);
        gpio_pull_down(touch_pins[i]);
    }

    // 3. Configuração do Barramento I2C1 para o SH1107
    i2c_init(OLED_I2C_PORT, OLED_BAUDRATE);
    gpio_set_function(OLED_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(OLED_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(OLED_SDA_PIN);
    gpio_pull_up(OLED_SCL_PIN);

    // 4. LEDs e Buzzer PWM
    gpio_init(LED_RGB_R); gpio_set_dir(LED_RGB_R, GPIO_OUT); gpio_put(LED_RGB_R, 0);
    gpio_init(LED_RGB_G); gpio_set_dir(LED_RGB_G, GPIO_OUT); gpio_put(LED_RGB_G, 0);
    gpio_init(LED_RGB_B); gpio_set_dir(LED_RGB_B, GPIO_OUT); gpio_put(LED_RGB_B, 0);
    buzzer_init_pwm();
}

// Leitura individual das 4 vias touch (Ativo em HIGH)
static inline bool touch_read(uint pino) {
    return gpio_get(pino) == 1;
}

static inline bool touch_qualquer_pressionado(void) {
    return gpio_get(TOUCH_CH1_PIN) || gpio_get(TOUCH_CH2_PIN) || 
           gpio_get(TOUCH_CH3_PIN) || gpio_get(TOUCH_CH4_PIN);
}

// Leitura dos botões físicos da BitDogLab (Ativo em LOW)
static inline bool botao_qualquer_pressionado(void) {
    return (!gpio_get(BTN_A_PIN)) || (!gpio_get(BTN_B_PIN)) || (!gpio_get(BTN_C_PIN));
}

// Função combinada: retorna true se QUALQUER botão físico OU sensor touch for acionado
static inline bool entrada_inicio_pressionada(void) {
    return botao_qualquer_pressionado() || touch_qualquer_pressionado();
}

#endif // HARDWARE_V7_H