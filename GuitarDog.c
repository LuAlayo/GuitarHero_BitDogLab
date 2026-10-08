// ===============================================================
// PROJETO 1 - EA801 - Laboratório Projeto de Sistemas Embarcados
// Luíza Maria Cabel Alayo, RA: 243537
// Rafael Mattos Sacramento, RA: 247349
// ===============================================================

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware_v7.h"
#include "sh1107.h"

// Estados da Máquina de Estados Finitos (MEF)
#define ESTADO_TELA_INICIAL   0
#define ESTADO_TELA_LOAD      1
#define ESTADO_TELA_JOGO      2
#define ESTADO_TELA_PONTUACAO 3

volatile int estado_mef = ESTADO_TELA_INICIAL;
int pontuacao = 0;
int recorde = 0;

// Posições X das 4 cordas verticais na tela de 128x128
const int corda_x[4] = {20, 48, 76, 104};
const uint touch_pins[4] = {TOUCH_CH1_PIN, TOUCH_CH2_PIN, TOUCH_CH3_PIN, TOUCH_CH4_PIN};

#define Y_ALVO             112 // Posição vertical dos alvos
#define JANELA_MIN         104 // Início da janela de acerto
#define JANELA_MAX         120 // Fim da janela de acerto
#define PASSO_QUEDA        8   // Distância que a nota cai a cada tick

// ============================================================
// TELAS DO JOGO
// ============================================================
void desenhar_tela_inicial(void) {
    sh1107_clear();
    sh1107_draw_string(28, 14, "GUITAR DOG", 1);
    sh1107_draw_hline(20, 108, 26, 1);
    
    // Instrução atualizada para contemplar botões e touch
    sh1107_draw_string(28, 48, "APERTE BOTAO", 1);
    sh1107_draw_string(22, 62, "A, B, C OU TOUCH", 1);
    sh1107_draw_string(26, 84, "PARA INICIAR", 1);
    sh1107_show();
}

void desenhar_tela_load(int segundos) {
    sh1107_clear();
    sh1107_draw_string(28, 20, "GUITAR DOG", 1);
    sh1107_draw_hline(20, 108, 32, 1);
    sh1107_draw_string(14, 56, "PREPARANDO JOGO", 1);
    
    char digito[2] = {'0' + segundos, '\0'};
    sh1107_draw_string(60, 80, digito, 1);
    sh1107_show();
}

void desenhar_base_guitarra(void) {
    sh1107_clear();

    // Divisórias verticais das 4 pistas da guitarra
    sh1107_draw_vline(6, 0, 127, 1);
    sh1107_draw_vline(34, 0, 127, 1);
    sh1107_draw_vline(62, 0, 127, 1);
    sh1107_draw_vline(90, 0, 127, 1);
    sh1107_draw_vline(118, 0, 127, 1);

    // Linhas centrais tracejadas das 4 cordas
    for (int i = 0; i < 4; i++) {
        for (int y = 0; y < 108; y += 4) {
            sh1107_draw_pixel(corda_x[i], y, 1);
        }
    }

    // Alvos na base
    for (int i = 0; i < 4; i++) {
        sh1107_draw_char(corda_x[i] - 2, Y_ALVO, 'O', 1);
    }

    // Pontuação corrente no topo
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

    if (pontuacao >= recorde && pontuacao > 0) {
        sh1107_draw_string(14, 90, "NOVO RECORDE!", 1);
    }
    sh1107_show();
}

// ============================================================
// MOTOR DE UMA RODADA DO JOGO
// ============================================================
void executar_rodada(int t1, int t2, int t3, int t4) {
    int pos_y[4] = {-1, -1, -1, -1};
    bool julgada[4] = {false, false, false, false};
    bool toque_anterior[4] = {false, false, false, false};
    int disparos[4] = {t1, t2, t3, t4};

    int ticks_decorridos = 0;
    tick_flag = false;
    timer_baixo_nivel_init(250000); // 250 ms por tick

    while (true) {
        // Leitura contínua dos canais touch
        for (int i = 0; i < 4; i++) {
            bool toque_atual = touch_read(touch_pins[i]);
            if (toque_atual && !toque_anterior[i]) {
                if (!julgada[i] && pos_y[i] >= JANELA_MIN && pos_y[i] <= JANELA_MAX) {
                    pontuacao += 10;
                    julgada[i] = true;
                    gpio_put(LED_RGB_G, 1);
                }
            }
            toque_anterior[i] = toque_atual;
        }

        // Avanço sincronizado via Timer de Hardware RP2040
        if (tick_flag) {
            tick_flag = false;
            gpio_put(LED_RGB_G, 0);

            for (int i = 0; i < 4; i++) {
                if (ticks_decorridos >= disparos[i] && pos_y[i] < 128) {
                    if (pos_y[i] == -1) pos_y[i] = 0;
                    else pos_y[i] += PASSO_QUEDA;

                    if (pos_y[i] > JANELA_MAX && !julgada[i]) {
                        julgada[i] = true;
                    }
                }
            }

            desenhar_base_guitarra();

            for (int i = 0; i < 4; i++) {
                if (pos_y[i] >= 0 && pos_y[i] <= 124) {
                    sh1107_draw_char(corda_x[i] - 2, pos_y[i], 'X', 1);
                }
            }

            sh1107_show();
            ticks_decorridos++;

            if (pos_y[0] >= 128 && pos_y[1] >= 128 && pos_y[2] >= 128 && pos_y[3] >= 128) {
                break;
            }
        }
    }

    timer_baixo_nivel_stop();
}

// ============================================================
// CONFIGURAÇÃO E LAÇO DA MEF
// ============================================================
void configuracao(void) {
    bitdoglab_init_hardware();
    sh1107_init();
}

int main(void) {
    configuracao();

    while (true) {
        switch (estado_mef) {
            case ESTADO_TELA_INICIAL:
                desenhar_tela_inicial();

                // Aguarda o acionamento de qualquer botão da BitDogLab (A, B, C) OU canal Touch
                while (!entrada_inicio_pressionada()) {
                    tight_loop_contents();
                }

                // Debounce e espera de liberação do botão/touch para evitar avanço duplo
                sleep_ms(50);
                while (entrada_inicio_pressionada()) {
                    tight_loop_contents();
                }

                pontuacao = 0;
                estado_mef = ESTADO_TELA_LOAD;
                break;

            case ESTADO_TELA_LOAD:
                for (int s = 3; s > 0; s--) {
                    desenhar_tela_load(s);
                    sleep_ms(1000);
                }
                estado_mef = ESTADO_TELA_JOGO;
                break;

            case ESTADO_TELA_JOGO:
                // Rodada de teste de 4 cordas
                executar_rodada(0, 4, 8, 12);
                executar_rodada(8, 0, 4, 0);

                if (pontuacao > recorde) {
                    recorde = pontuacao;
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