// ===============================================================
// PROJETO 2 - EA801 - Laboratório Projeto de Sistemas Embarcados
// Luíza Maria Cabel Alayo, RA: 243537
// Rafael Mattos Sacramento, RA: 247349
// ===============================================================

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware_v7.h"
#include "sh1107.h"
#include "recorde_flash.h"
#include "musicas.h"

// Estados da Máquina de Estados Finitos (MEF)
#define ESTADO_TELA_INICIAL     0
#define ESTADO_SELECAO_FASE     1
#define ESTADO_TELA_LOAD        2
#define ESTADO_TELA_JOGO        3
#define ESTADO_TELA_PONTUACAO   4

volatile int estado_mef = ESTADO_TELA_INICIAL;
int pontuacao = 0;
int recorde = 0;
bool novo_recorde = false;

int fase_selecionada = 0;


// Posições X das 4 cordas verticais na tela de 128x128
const int corda_x[4] = {20, 48, 76, 104};
const uint touch_pins[4] = {TOUCH_CH1_PIN, TOUCH_CH2_PIN, TOUCH_CH3_PIN, TOUCH_CH4_PIN};

#define Y_ALVO             112 // Posição vertical dos alvos
#define JANELA_MIN         96  // Início da janela de acerto
#define JANELA_MAX         124 // Fim da janela de acerto
#define PASSO_QUEDA        8   // Distância que a nota cai a cada tick

// ============================================================
// TELAS DO JOGO
// ============================================================
void desenhar_tela_inicial(void) {
    sh1107_clear();
    sh1107_draw_string(28, 14, "GUITAR DOG", 1);
    sh1107_draw_hline(20, 108, 26, 1);

    sh1107_draw_string(28, 48, "APERTE BOTAO", 1);
    sh1107_draw_string(22, 62, "A, B, C OU TOUCH", 1);
    sh1107_draw_string(26, 84, "PARA INICIAR", 1);
    sh1107_show();
}

void desenhar_tela_selecao(void) {
    sh1107_clear();

    int pagina = fase_selecionada / 3;
    int fase_ini = (pagina * 3) + 1;
    int fase_fim = fase_ini + 2;
    if (fase_fim > TOTAL_FASES) fase_fim = TOTAL_FASES;

    char titulo[24];
    sprintf(titulo, "ESCOLHA FASE %d-%d", fase_ini, fase_fim);
    sh1107_draw_string(10, 10, titulo, 1);
    sh1107_draw_hline(8, 120, 24, 1);

    for (int i = 0; i < 3; i++) {
        int idx = (pagina * 3) + i;
        if (idx >= TOTAL_FASES) break;
        int y = 40 + (i * 24);

        sh1107_draw_string(8, y, fases[idx].nome, 1);

        if (idx == fase_selecionada) {
            sh1107_draw_hline(8, 120, y + 10, 1);
        }
    }

    sh1107_draw_string(10, 112, "JOYSTICK V-H : BTN", 1);
    sh1107_show();
}

void desenhar_tela_load(int segundos) {
    sh1107_clear();
    sh1107_draw_string(28, 20, "GUITAR DOG", 1);
    sh1107_draw_hline(20, 108, 32, 1);

    char titulo_carregando[24];
    sprintf(titulo_carregando, "FASE %d", fase_selecionada + 1);
    sh1107_draw_string(38, 46, titulo_carregando, 1);

    char digito[2] = {'0' + segundos, '\0'};
    sh1107_draw_string(60, 80, digito, 1);
    sh1107_show();
}

void desenhar_base_guitarra(void) {
    sh1107_clear();

    for (int i = 0; i < 5; i++) {
        sh1107_draw_vline(6 + (i * 28), 0, 127, 1);
    }

    for (int i = 0; i < 4; i++) {
        for (int y = 0; y < 108; y += 4) {
            sh1107_draw_pixel(corda_x[i], y, 1);
        }
        sh1107_draw_char(corda_x[i] - 2, Y_ALVO, 'O', 1);
    }

    char txt_pts[16];
    sprintf(txt_pts, "%d", pontuacao);
    sh1107_draw_string(2, 2, txt_pts, 1);
}

void desenhar_tela_pontuacao(void) {
    sh1107_clear();
    sh1107_draw_string(30, 16, "FIM DE JOGO", 1);
    sh1107_draw_hline(20, 108, 28, 1);

    char buffer[24];
    sprintf(buffer, "PONTOS: %d", pontuacao);
    sh1107_draw_string(24, 50, buffer, 1);

    sprintf(buffer, "RECORDE: %d", recorde);
    sh1107_draw_string(20, 68, buffer, 1);

    if (novo_recorde) {
        sh1107_draw_string(14, 90, "NOVO RECORDE!", 1);
    }
    sh1107_show();
}

// ============================================================
// MOTOR GENÉRICO DE EXECUÇÃO DE FASES
// ============================================================
// Mude para 0 quando quiser voltar a jogar normalmente com os sensores touch
#define MODO_AUTOPLAY 1

// ============================================================
// MOTOR GENÉRICO DE EXECUÇÃO DE FASES
// ============================================================
void executar_jogo(const NotaMusical *partitura, size_t total_notas, uint32_t velocidade_tick_us) {
    int pos_y[total_notas];
    bool julgada[total_notas];
    bool acertada[total_notas];

    for (size_t n = 0; n < total_notas; n++) {
        pos_y[n] = -1;
        julgada[n] = false;
        acertada[n] = false;
    }

    bool toque_anterior[4] = {false, false, false, false};
    int ticks_buzzer_restantes = 0;
    int ticks_decorridos = 0;

    tick_flag = false;
    timer_baixo_nivel_init(velocidade_tick_us);

    while (true) {
#if !MODO_AUTOPLAY
        for (int c = 0; c < 4; c++) {
            bool toque_atual = touch_read(touch_pins[c]);
            if (toque_atual && !toque_anterior[c]) {
                for (size_t n = 0; n < total_notas; n++) {
                    if (partitura[n].corda == c && !julgada[n]) {
                        if (pos_y[n] >= JANELA_MIN && pos_y[n] <= JANELA_MAX) {
                            pontuacao += 10;
                            julgada[n] = true;
                            acertada[n] = true;

                            buzzer_tocar_nota(partitura[n].freq);
                            ticks_buzzer_restantes = partitura[n].duracao_ticks;

                            gpio_put(LED_RGB_G, 1);
                            gpio_put(LED_RGB_R, 0);
                            break;
                        }
                    }
                }
            }
            toque_anterior[c] = toque_atual;
        }
#else
        (void)toque_anterior; // Evita aviso de variável não utilizada no modo autoplay
#endif

        if (tick_flag) {
            tick_flag = false;

            if (ticks_buzzer_restantes > 0) {
                ticks_buzzer_restantes--;
                if (ticks_buzzer_restantes == 0) {
                    buzzer_parar();
                    gpio_put(LED_RGB_G, 0);
                }
            } else {
                gpio_put(LED_RGB_G, 0);
            }
            gpio_put(LED_RGB_R, 0);

            bool todas_finalizadas = true;

            for (size_t n = 0; n < total_notas; n++) {
                if (ticks_decorridos >= partitura[n].tick_disparo) {
                    if (pos_y[n] < 128) {
                        if (pos_y[n] == -1) pos_y[n] = 0;
                        else pos_y[n] += PASSO_QUEDA;

#if MODO_AUTOPLAY
                        // Toca a nota sozinha assim que ela atinge a linha do alvo (Y_ALVO = 112)
                        if (pos_y[n] == Y_ALVO && !julgada[n]) {
                            julgada[n] = true;
                            acertada[n] = true;

                            buzzer_tocar_nota(partitura[n].freq);
                            ticks_buzzer_restantes = partitura[n].duracao_ticks;
                            gpio_put(LED_RGB_G, 1);
                        }
#endif

                        if (pos_y[n] > JANELA_MAX && !julgada[n]) {
                            julgada[n] = true;
                            gpio_put(LED_RGB_R, 1);
                        }
                    }
                }

                if (pos_y[n] < 128) {
                    todas_finalizadas = false;
                }
            }

            desenhar_base_guitarra();

            for (size_t n = 0; n < total_notas; n++) {
                if (pos_y[n] >= 0 && pos_y[n] <= 124 && !acertada[n]) {
                    uint8_t c = partitura[n].corda;
                    sh1107_draw_char(corda_x[c] - 2, pos_y[n], 'X', 1);
                }
            }

            sh1107_show();
            ticks_decorridos++;

            if (todas_finalizadas && ticks_buzzer_restantes == 0) {
                break;
            }
        }
    }

    buzzer_parar();
    gpio_put(LED_RGB_R, 0);
    gpio_put(LED_RGB_G, 0);
    timer_baixo_nivel_stop();
}

// ============================================================
// CONFIGURAÇÃO E LAÇO DA MEF
// ============================================================
void configuracao(void) {
    bitdoglab_init_hardware();
    sh1107_init();
    recorde = recorde_carregar();
}

int main(void) {
    configuracao();

    adc_init();
    adc_gpio_init(26); // VRy (Vertical) do Joystick KY023 - Canal 0
    adc_gpio_init(27); // VRx (Horizontal) do Joystick KY023 - Canal 1

    while (true) {
        switch (estado_mef) {
            case ESTADO_TELA_INICIAL:
                desenhar_tela_inicial();

                while (!entrada_inicio_pressionada()) {
                    tight_loop_contents();
                }

                sleep_ms(50);
                while (entrada_inicio_pressionada()) {
                    tight_loop_contents();
                }

                estado_mef = ESTADO_SELECAO_FASE;
                break;

case ESTADO_SELECAO_FASE:
            {
                bool selecionou = false;

                while (!selecionou) {
                    desenhar_tela_selecao();

                    // Leitura do eixo Vertical (GPIO26 = Canal 0)
                    adc_select_input(0);
                    uint16_t raw_y = adc_read();

                    // Leitura do eixo Horizontal (GPIO27 = Canal 1)
                    adc_select_input(1);
                    uint16_t raw_x = adc_read();

                    int total_paginas = (TOTAL_FASES + 2) / 3;
                    int pagina = fase_selecionada / 3; // 0 (1..3), 1 (4..6), 2 (7..9)...
                    int linha  = fase_selecionada % 3; // 0, 1 ou 2

                    // Movimento vertical: navega entre as 3 fases da página atual
                    if (raw_y < 1000) {
                        linha = (linha - 1 + 3) % 3;
                        fase_selecionada = (pagina * 3) + linha;
                        if (fase_selecionada >= TOTAL_FASES) {
                            fase_selecionada = TOTAL_FASES - 1;
                        }
                        sleep_ms(250);
                    } else if (raw_y > 3000) {
                        linha = (linha + 1) % 3;
                        fase_selecionada = (pagina * 3) + linha;
                        if (fase_selecionada >= TOTAL_FASES) {
                            fase_selecionada = pagina * 3;
                        }
                        sleep_ms(250);
                    }
                    // Movimento horizontal: Esquerda (> 3000) volta página | Direita (< 1000) avança página
                    else if (raw_x > 3000) {
                        pagina = (pagina - 1 + total_paginas) % total_paginas;
                        fase_selecionada = (pagina * 3) + linha;
                        if (fase_selecionada >= TOTAL_FASES) {
                            fase_selecionada = TOTAL_FASES - 1;
                        }
                        sleep_ms(250);
                    } else if (raw_x < 1000) {
                        pagina = (pagina + 1) % total_paginas;
                        fase_selecionada = (pagina * 3) + linha;
                        if (fase_selecionada >= TOTAL_FASES) {
                            fase_selecionada = TOTAL_FASES - 1;
                        }
                        sleep_ms(250);
                    }

                    // Seleção por QUALQUER botão físico (A, B ou C) ou sensor TOUCH capacitivo
                    if (entrada_inicio_pressionada()) {
                        sleep_ms(50);
                        while (entrada_inicio_pressionada()) {
                            tight_loop_contents();
                        }
                        selecionou = true;
                    }
                    sleep_ms(50);
                }

                pontuacao = 0;
                novo_recorde = false;
                estado_mef = ESTADO_TELA_LOAD;
                break;
            }

            case ESTADO_TELA_LOAD:
                for (int s = 3; s > 0; s--) {
                    desenhar_tela_load(s);
                    buzzer_tocar_nota(440);
                    sleep_ms(100);
                    buzzer_parar();
                    sleep_ms(900);
                }
                estado_mef = ESTADO_TELA_JOGO;
                break;

            case ESTADO_TELA_JOGO:
                // Executa diretamente a fase selecionada a partir da tabela em musicas.h
                executar_jogo(
                    fases[fase_selecionada].partitura,
                    fases[fase_selecionada].total_notas,
                    fases[fase_selecionada].velocidade_tick_us
                );

                if (pontuacao > recorde) {
                    recorde = pontuacao;
                    novo_recorde = true;
                    recorde_salvar(recorde);
                }
                estado_mef = ESTADO_TELA_PONTUACAO;
                break;

            case ESTADO_TELA_PONTUACAO:
                desenhar_tela_pontuacao();
                sleep_ms(4000);
                estado_mef = ESTADO_TELA_INICIAL;
                break;
        }
    }

    return 0;
}