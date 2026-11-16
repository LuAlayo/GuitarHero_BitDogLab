// ===============================================================
// PROJETO 2 - EA801 - Laboratório Projeto de Sistemas Embarcados
// Luíza Maria Cabel Alayo, RA: 243537
// Rafael Mattos Sacramento, RA: 247349
// ===============================================================

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware_v7.h"
#include "sh1107.h"
#include "recorde_flash.h"

// Estados da Máquina de Estados Finitos (MEF)
#define ESTADO_TELA_INICIAL   0
#define ESTADO_TELA_LOAD      1
#define ESTADO_TELA_JOGO      2
#define ESTADO_TELA_PONTUACAO 3

volatile int estado_mef = ESTADO_TELA_INICIAL;
int pontuacao = 0;
int recorde = 0;
bool novo_recorde = false;

// Posições X das 4 cordas verticais na tela de 128x128
const int corda_x[4] = {20, 48, 76, 104};
const uint touch_pins[4] = {TOUCH_CH1_PIN, TOUCH_CH2_PIN, TOUCH_CH3_PIN, TOUCH_CH4_PIN};

#define Y_ALVO             112 // Posição vertical dos alvos
#define JANELA_MIN         96  // Início da janela de acerto (tolerância de +-2 ticks)
#define JANELA_MAX         124 // Fim da janela de acerto
#define PASSO_QUEDA        8   // Distância que a nota cai a cada tick (125 ms)

// ============================================================
// ESTRUTURA E PARTITURA: "BRILHA BRILHA ESTRELINHA"
// ============================================================
typedef struct {
    uint8_t corda;         // 0 a 3 (Canal Touch 1 a 4)
    uint16_t freq;         // Frequência da nota em Hz
    uint16_t tick_disparo; // Tick em que a nota nasce no topo (y = 0)
    uint8_t duracao_ticks; // Quantos ticks o buzzer fica soando ao acertar
} NotaMusical;

// Cada tick = 125 ms.
// Semínima = 4 ticks (500 ms) de intervalo, 3 ticks (375 ms) de som ligado.
// Mínima   = 8 ticks (1000 ms) de intervalo, 6 ticks (750 ms) de som ligado.
const NotaMusical partitura_estrelinha[] = {
    // "Bri - lha, bri - lha, es - tre - li - nha" (Dó Dó Sol Sol Lá Lá Sol)
    {0, NOTA_DO4,   0, 3}, // Bri-
    {0, NOTA_DO4,   4, 3}, // -lha,
    {2, NOTA_SOL4,  8, 3}, // bri-
    {2, NOTA_SOL4, 12, 3}, // -lha,
    {3, NOTA_LA4,  16, 3}, // es-
    {3, NOTA_LA4,  20, 3}, // -tre-
    {2, NOTA_SOL4, 24, 6}, // -li-nha (nota longa)

    // "Que - ro ver vo - cê bri - lhar" (Fá Fá Mi Mi Ré Ré Dó)
    {2, NOTA_FA4,  32, 3}, // Que-
    {2, NOTA_FA4,  36, 3}, // -ro
    {1, NOTA_MI4,  40, 3}, // ver
    {1, NOTA_MI4,  44, 3}, // vo-
    {1, NOTA_RE4,  48, 3}, // -cê
    {1, NOTA_RE4,  52, 3}, // bri-
    {0, NOTA_DO4,  56, 6}  // -lhar (nota longa)
};

#define TOTAL_NOTAS (sizeof(partitura_estrelinha) / sizeof(partitura_estrelinha[0]))

// ============================================================
// TELAS DO JOGO
// ============================================================
void desenhar_tela_inicial(void) {
    sh1107_clear();
    sh1107_draw_string(28, 14, "GUITAR DOG", 1);
    sh1107_draw_hline(20, 108, 26, 1);

    sh1107_draw_string(16, 40, "MUSICA: ESTRELINHA", 1);
    sh1107_draw_string(28, 62, "APERTE BOTAO", 1);
    sh1107_draw_string(22, 76, "A, B, C OU TOUCH", 1);
    sh1107_draw_string(26, 96, "PARA INICIAR", 1);
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

    if (novo_recorde) {
        sh1107_draw_string(14, 90, "NOVO RECORDE!", 1);
    }
    sh1107_show();
}

// ============================================================
// MOTOR DA PARTITURA ("BRILHA BRILHA ESTRELINHA")
// ============================================================
// Obs.: Se você quiser que a música toque sozinha (modo demonstração/base)
// mesmo sem acertar o touch, mude TOCAR_SOMENTE_NO_ACERTO para 0.
#define TOCAR_SOMENTE_NO_ACERTO 1

void executar_musica(void) {
    int pos_y[TOTAL_NOTAS];
    bool julgada[TOTAL_NOTAS];
    bool acertada[TOTAL_NOTAS];

    for (size_t n = 0; n < TOTAL_NOTAS; n++) {
        pos_y[n] = -1;
        julgada[n] = false;
        acertada[n] = false;
    }

    bool toque_anterior[4] = {false, false, false, false};
    int ticks_buzzer_restantes = 0;
    int ticks_decorridos = 0;

    tick_flag = false;
    timer_baixo_nivel_init(125000); // 125 ms por tick (Semínima = 500 ms)

    while (true) {
        // 1. Leitura em tempo real dos 4 canais Touch (borda de subida)
        for (int c = 0; c < 4; c++) {
            bool toque_atual = touch_read(touch_pins[c]);
            if (toque_atual && !toque_anterior[c]) {
                // Procura a nota não julgada mais próxima do alvo nesta corda
                for (size_t n = 0; n < TOTAL_NOTAS; n++) {
                    if (partitura_estrelinha[n].corda == c && !julgada[n]) {
                        if (pos_y[n] >= JANELA_MIN && pos_y[n] <= JANELA_MAX) {
                            pontuacao += 10;
                            julgada[n] = true;
                            acertada[n] = true;

                            // Toca a nota correspondente da melodia no Buzzer!
                            buzzer_tocar_nota(partitura_estrelinha[n].freq);
                            ticks_buzzer_restantes = partitura_estrelinha[n].duracao_ticks;

                            gpio_put(LED_RGB_G, 1);
                            gpio_put(LED_RGB_R, 0);
                            break;
                        }
                    }
                }
            }
            toque_anterior[c] = toque_atual;
        }

        // 2. Avanço sincronizado via Timer de Hardware RP2040 (a cada 125 ms)
        if (tick_flag) {
            tick_flag = false;

            // Controle de tempo do som da nota no Buzzer
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

            for (size_t n = 0; n < TOTAL_NOTAS; n++) {
                if (ticks_decorridos >= partitura_estrelinha[n].tick_disparo) {
                    if (pos_y[n] < 128) {
                        if (pos_y[n] == -1) pos_y[n] = 0;
                        else pos_y[n] += PASSO_QUEDA;

                        #if !TOCAR_SOMENTE_NO_ACERTO
                        // Modo guia: toca a nota exatamente quando ela passa pelo Y_ALVO
                        if (pos_y[n] == Y_ALVO) {
                            buzzer_tocar_nota(partitura_estrelinha[n].freq);
                            ticks_buzzer_restantes = partitura_estrelinha[n].duracao_ticks;
                        }
                        #endif

                        // Nota passou da janela sem ser tocada -> Erro
                        if (pos_y[n] > JANELA_MAX && !julgada[n]) {
                            julgada[n] = true;
                            gpio_put(LED_RGB_R, 1); // Pisca LED vermelho ao perder nota
                        }
                    }
                }

                if (pos_y[n] < 128) {
                    todas_finalizadas = false;
                }
            }

            // Renderização gráfica no OLED SH1107 128x128
            desenhar_base_guitarra();

            for (size_t n = 0; n < TOTAL_NOTAS; n++) {
                // Desenha apenas as notas que estão na tela e ainda não foram acertadas
                if (pos_y[n] >= 0 && pos_y[n] <= 124 && !acertada[n]) {
                    uint8_t c = partitura_estrelinha[n].corda;
                    sh1107_draw_char(corda_x[c] - 2, pos_y[n], 'X', 1);
                }
            }

            sh1107_show();
            ticks_decorridos++;

            // Aguarda a última nota sair da tela e o último som terminar
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
    recorde = recorde_carregar();   // recupera o recorde salvo na flash
}

int main(void) {
    configuracao();

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

                pontuacao = 0;
                novo_recorde = false;
                estado_mef = ESTADO_TELA_LOAD;
                break;

            case ESTADO_TELA_LOAD:
                for (int s = 3; s > 0; s--) {
                    desenhar_tela_load(s);
                    buzzer_tocar_nota(440); // Beep curto de contagem regressiva
                    sleep_ms(100);
                    buzzer_parar();
                    sleep_ms(900);
                }
                estado_mef = ESTADO_TELA_JOGO;
                break;

            case ESTADO_TELA_JOGO:
                executar_musica();

                if (pontuacao > recorde) {
                    recorde = pontuacao;
                    novo_recorde = true;
                    recorde_salvar(recorde);   // grava na flash (timer já parado)
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