#ifndef MUSICAS_H
#define MUSICAS_H

#include <stddef.h>
#include <stdint.h>
#include "hardware_v7.h"

// ============================================================
// FREQUÊNCIAS DAS NOTAS MUSICAIS (4ª e 5ª Oitavas em Hz)
// ============================================================
#ifndef NOTA_DO4
#define NOTA_DO4            262
#endif
#ifndef NOTA_RE4
#define NOTA_RE4            294
#endif
#ifndef NOTA_MI4
#define NOTA_MI4            330
#endif
#ifndef NOTA_FA4
#define NOTA_FA4            349
#endif
#ifndef NOTA_FAS4
#define NOTA_FAS4           370 // Fá#4 / Solb4
#endif
#ifndef NOTA_SOL4
#define NOTA_SOL4           392
#endif
#ifndef NOTA_SOLS4
#define NOTA_SOLS4          415 // Sol#4 / Láb4
#endif
#ifndef NOTA_LA4
#define NOTA_LA4            440
#endif
#ifndef NOTA_LAS4
#define NOTA_LAS4           466 // Lá#4 / Sib4
#endif
#ifndef NOTA_SI4
#define NOTA_SI4            494
#endif
#ifndef NOTA_DO5
#define NOTA_DO5            523
#endif
#ifndef NOTA_DOS5
#define NOTA_DOS5           554 // Dó#5 / Réb5
#endif
#ifndef NOTA_RE5
#define NOTA_RE5            587
#endif
#ifndef NOTA_RES5
#define NOTA_RES5           622 // Ré#5 / Mib5
#endif
#ifndef NOTA_MI5
#define NOTA_MI5            659
#endif
#ifndef NOTA_FA5
#define NOTA_FA5            698
#endif
#ifndef NOTA_FAS5
#define NOTA_FAS5           740
#endif
#ifndef NOTA_SOL5
#define NOTA_SOL5           784
#endif

// ============================================================
// ESTRUTURAS DE NOTA E DE FASE MUSICAL
// ============================================================
typedef struct {
    uint8_t corda;
    uint16_t freq;
    uint16_t tick_disparo;
    uint8_t duracao_ticks;
} NotaMusical;

typedef struct {
    const char *nome;
    const NotaMusical *partitura;
    size_t total_notas;
    uint32_t velocidade_tick_us;
} FaseMusical;

// ============================================================
// FASE 1: BRILHA BRILHA ESTRELINHA (Forma Completa A - B - A)
// Semínima = 4 ticks | Mínima = 8 ticks
// ============================================================
static const NotaMusical partitura_fase1[] = {
    // Parte A1: "Brilha, brilha, estrelinha / Quero ver você brilhar"
    {0, NOTA_DO4,    0, 3}, {0, NOTA_DO4,    4, 3},
    {2, NOTA_SOL4,   8, 3}, {2, NOTA_SOL4,  12, 3},
    {3, NOTA_LA4,   16, 3}, {3, NOTA_LA4,   20, 3},
    {2, NOTA_SOL4,  24, 6}, {2, NOTA_FA4,   32, 3},
    {2, NOTA_FA4,   36, 3}, {1, NOTA_MI4,   40, 3},
    {1, NOTA_MI4,   44, 3}, {1, NOTA_RE4,   48, 3},
    {1, NOTA_RE4,   52, 3}, {0, NOTA_DO4,   56, 6},

    // Parte B: "Lá no alto, lá no céu / Num desenho de cordel"
    {2, NOTA_SOL4,  64, 3}, {2, NOTA_SOL4,  68, 3},
    {2, NOTA_FA4,   72, 3}, {2, NOTA_FA4,   76, 3},
    {1, NOTA_MI4,   80, 3}, {1, NOTA_MI4,   84, 3},
    {1, NOTA_RE4,   88, 6}, {2, NOTA_SOL4,  96, 3},
    {2, NOTA_SOL4, 100, 3}, {2, NOTA_FA4,  104, 3},
    {2, NOTA_FA4,  108, 3}, {1, NOTA_MI4,  112, 3},
    {1, NOTA_MI4,  116, 3}, {1, NOTA_RE4,  120, 6},

    // Parte A2: "Brilha, brilha, estrelinha / Quero ver você brilhar"
    {0, NOTA_DO4,  128, 3}, {0, NOTA_DO4,  132, 3},
    {2, NOTA_SOL4, 136, 3}, {2, NOTA_SOL4, 140, 3},
    {3, NOTA_LA4,  144, 3}, {3, NOTA_LA4,  148, 3},
    {2, NOTA_SOL4, 152, 6}, {2, NOTA_FA4,  160, 3},
    {2, NOTA_FA4,  164, 3}, {1, NOTA_MI4,  168, 3},
    {1, NOTA_MI4,  172, 3}, {1, NOTA_RE4,  176, 3},
    {1, NOTA_RE4,  180, 3}, {0, NOTA_DO4,  184, 7}
};

// ============================================================
// FASE 2: BLITZKRIEG BOP (Ramones) - Versão Estendida
// Grade 4/4 (24 ticks/compasso): Colcheia = 3 | Semínima = 6 | Mínima = 12
// ============================================================
static const NotaMusical partitura_fase2[] = {
    // Compassos 1-2 (0..47): Grito "HEY! HO! LET'S GO!"
    {3, NOTA_MI5,     0,  5}, {2, NOTA_RE5,    12,  5},
    {3, NOTA_MI5,    24,  4}, {0, NOTA_LA4,    30, 10},

    // Compassos 3-4 (48..95): "They're form-ing in a straight line"
    {1, NOTA_DOS5,   48,  2}, {1, NOTA_DOS5,   51,  2},
    {1, NOTA_DOS5,   54,  5}, {1, NOTA_DOS5,   60,  5},
    {1, NOTA_DOS5,   66,  5}, {2, NOTA_RE5,    72, 10},
    {0, NOTA_SI4,    84, 10},

    // Compassos 5-6 (96..143): "They're go-ing through a tight wind"
    {1, NOTA_DOS5,   96,  2}, {1, NOTA_DOS5,   99,  2},
    {1, NOTA_DOS5,  102,  5}, {1, NOTA_DOS5,  108,  5},
    {1, NOTA_DOS5,  114,  5}, {2, NOTA_RE5,   120, 10},
    {0, NOTA_SI4,   132, 10},

    // Compassos 7-8 (144..191): "The kids are los-ing their minds"
    {1, NOTA_DOS5,  144,  5}, {1, NOTA_DOS5,  150,  5},
    {1, NOTA_DOS5,  156,  5}, {1, NOTA_DOS5,  162,  2},
    {1, NOTA_DOS5,  165,  2}, {2, NOTA_RE5,   168, 10},
    {0, NOTA_SI4,   180, 10},

    // Compasso 9 (192..215): "The Blitz-krieg Bop!"
    {0, NOTA_LA4,   192,  5}, {2, NOTA_RE5,   198,  5},
    {1, NOTA_DOS5,  204,  5}, {0, NOTA_LA4,   210,  5},

    // Compassos 10-11 (216..263): "They're pil-ing in the back seat"
    {1, NOTA_DOS5,  216,  2}, {1, NOTA_DOS5,  219,  2},
    {1, NOTA_DOS5,  222,  5}, {1, NOTA_DOS5,  228,  5},
    {1, NOTA_DOS5,  234,  5}, {2, NOTA_RE5,   240, 10},
    {0, NOTA_SI4,   252, 10},

    // Compassos 12-13 (264..311): "They're gen-er-at-ing steam heat"
    {1, NOTA_DOS5,  264,  2}, {1, NOTA_DOS5,  267,  2},
    {1, NOTA_DOS5,  270,  2}, {1, NOTA_DOS5,  273,  2},
    {1, NOTA_DOS5,  276,  5}, {1, NOTA_DOS5,  282,  5},
    {2, NOTA_RE5,   288, 10}, {0, NOTA_SI4,   300, 10},

    // Compassos 14-15 (312..359): Ponte "Hey, ho, let's go! Shoot 'em in the back now"
    {2, NOTA_RE5,   312,  5}, {2, NOTA_RE5,   318,  5},
    {2, NOTA_RE5,   324,  5}, {1, NOTA_DOS5,  330,  5},
    {0, NOTA_LA4,   336,  2}, {0, NOTA_LA4,   339,  2},
    {0, NOTA_LA4,   342,  2}, {0, NOTA_LA4,   345,  2},
    {2, NOTA_RE5,   348,  5}, {0, NOTA_SI4,   354,  5},

    // Compasso 16 (360..383): Final "The Blitz-krieg Bop!"
    {0, NOTA_LA4,   360,  5}, {2, NOTA_RE5,   366,  5},
    {3, NOTA_MI5,   372,  5}, {0, NOTA_LA4,   378, 12}
};

// ============================================================
// FASE 3: CHICLETEIRO EU, CHICLETEIRA ELA - Versão Estendida
// Refrão + Solo de Guitarra Baiana + Refrão Final (E7 / Am)
// Grade 4/4 (24 ticks/compasso): Colcheia = 3 | Semínima = 6 | Mínima = 12
// ============================================================
static const NotaMusical partitura_fase3[] = {
    // Compasso 1 (0..23 - E7): "Chi-cle-tei-ro | eu..."
    {1, NOTA_SI4,     0,  2}, {1, NOTA_SI4,     3,  2},
    {2, NOTA_DO5,     6,  2}, {3, NOTA_RE5,     9,  2},
    {1, NOTA_SI4,    12,  9},

    // Compasso 2 (24..47 - Am): "chi-cle-tei-ra | e - la"
    {0, NOTA_LA4,    24,  2}, {0, NOTA_LA4,    27,  2},
    {1, NOTA_SI4,    30,  2}, {2, NOTA_DO5,    33,  2},
    {1, NOTA_SI4,    36,  5}, {0, NOTA_LA4,    42,  5},

    // Compasso 3 (48..71 - E7): "Chi-cle-tei-ro | eu..."
    {1, NOTA_SI4,    48,  2}, {1, NOTA_SI4,    51,  2},
    {2, NOTA_DO5,    54,  2}, {3, NOTA_RE5,    57,  2},
    {1, NOTA_SI4,    60,  9},

    // Compasso 4 (72..95 - Am): "chi-cle-tei-ra | e - la"
    {0, NOTA_LA4,    72,  2}, {0, NOTA_LA4,    75,  2},
    {1, NOTA_SI4,    78,  2}, {2, NOTA_DO5,    81,  2},
    {1, NOTA_SI4,    84,  5}, {0, NOTA_LA4,    90,  5},

    // Compasso 5 (96..119): "Li-be-ra, | li-be-ra!"
    {2, NOTA_DO5,    96,  2}, {1, NOTA_SI4,    99,  2},
    {0, NOTA_LA4,   102,  5}, {1, NOTA_SI4,   108,  2},
    {0, NOTA_SOLS4, 111,  2}, {0, NOTA_LA4,   114,  5},

    // Compassos 6-7 (120..167): Solo de Guitarra Baiana (Parte 1)
    {0, NOTA_RE4,   120,  2}, {1, NOTA_MI4,   123,  2},
    {2, NOTA_FA4,   126,  2}, {1, NOTA_MI4,   129,  2},
    {3, NOTA_DO5,   132,  5}, {2, NOTA_LA4,   138,  5},
    {0, NOTA_RE4,   144,  2}, {1, NOTA_MI4,   147,  2},
    {2, NOTA_FA4,   150,  2}, {1, NOTA_MI4,   153,  2},
    {3, NOTA_SI4,   156,  5}, {1, NOTA_SOLS4, 162,  5},

    // Compassos 8-9 (168..215): Solo de Guitarra Baiana (Parte 2)
    {0, NOTA_RE4,   168,  2}, {1, NOTA_MI4,   171,  2},
    {2, NOTA_FA4,   174,  2}, {1, NOTA_MI4,   177,  2},
    {3, NOTA_DO5,   180,  5}, {2, NOTA_LA4,   186,  5},
    {3, NOTA_RE5,   192,  2}, {2, NOTA_DO5,   195,  2},
    {1, NOTA_SI4,   198,  2}, {0, NOTA_SOLS4, 201,  2},
    {1, NOTA_LA4,   204, 10},

    // Compasso 10 (216..239): Volta ao Refrão "Chi-cle-tei-ro | eu..."
    {1, NOTA_SI4,   216,  2}, {1, NOTA_SI4,   219,  2},
    {2, NOTA_DO5,   222,  2}, {3, NOTA_RE5,   225,  2},
    {1, NOTA_SI4,   228,  9},

    // Compasso 11 (240..263): "chi-cle-tei-ra | e - la!"
    {0, NOTA_LA4,   240,  2}, {0, NOTA_LA4,   243,  2},
    {1, NOTA_SI4,   246,  2}, {2, NOTA_DO5,   249,  2},
    {1, NOTA_SI4,   252,  5}, {0, NOTA_LA4,   258, 12}
};

// ============================================================
// FASE 4: DANCING WITH MYSELF (Billy Idol) - Versão Estendida
// Verso + Refrão + Riff Descendente + Ponte (Mi Maior)
// Grade 4/4 (24 ticks/compasso): Colcheia = 3 | Semínima = 6 | Mínima = 12
// ============================================================
static const NotaMusical partitura_fase4[] = {
    // Compassos 1-2 (0..47): Verso "On the floors of To-ky-o / Down in Lon-don town to go"
    {0, NOTA_MI4,     0,  2}, {1, NOTA_SOLS4,   3,  2},
    {2, NOTA_SI4,     6,  5}, {2, NOTA_SI4,    12,  2},
    {1, NOTA_LA4,    15,  2}, {1, NOTA_SOLS4,  18,  5},
    {0, NOTA_MI4,    24,  2}, {1, NOTA_SOLS4,  27,  2},
    {2, NOTA_SI4,    30,  5}, {3, NOTA_DOS5,   36,  5},
    {2, NOTA_SI4,    42,  5},

    // Compassos 3-4 (48..95): "With the rec-ord se-lec-tion / And the mir-ror's re-flec-tion"
    {0, NOTA_MI4,    48,  2}, {1, NOTA_SOLS4,  51,  2},
    {2, NOTA_SI4,    54,  2}, {2, NOTA_SI4,    57,  2},
    {1, NOTA_LA4,    60,  5}, {1, NOTA_SOLS4,  66,  5},
    {1, NOTA_FAS4,   72,  2}, {1, NOTA_SOLS4,  75,  2},
    {2, NOTA_LA4,    78,  5}, {1, NOTA_SOLS4,  84,  5},
    {0, NOTA_FAS4,   90,  5},

    // Compassos 5-6 (96..143): Refrão "Oh, oh, danc-ing with my-self" + Riff
    {2, NOTA_SI4,    96,  5}, {1, NOTA_SOLS4, 102,  5},
    {2, NOTA_LA4,   108,  2}, {2, NOTA_LA4,   111,  2},
    {1, NOTA_SOLS4, 114,  2}, {0, NOTA_FAS4,  117,  2},
    {0, NOTA_MI4,   120, 10}, {3, NOTA_SI4,   132,  2},
    {2, NOTA_LA4,   135,  2}, {1, NOTA_SOLS4, 138,  2},
    {0, NOTA_FAS4,  141,  2},

    // Compassos 7-8 (144..191): "Oh, oh, danc-ing with my-self" + Riff
    {2, NOTA_SI4,   144,  5}, {1, NOTA_SOLS4, 150,  5},
    {2, NOTA_LA4,   156,  2}, {2, NOTA_LA4,   159,  2},
    {1, NOTA_SOLS4, 162,  2}, {0, NOTA_FAS4,  165,  2},
    {0, NOTA_MI4,   168, 10}, {3, NOTA_SI4,   180,  2},
    {2, NOTA_LA4,   183,  2}, {1, NOTA_SOLS4, 186,  2},
    {0, NOTA_FAS4,  189,  2},

    // Compassos 9-10 (192..239): "When there's noth-ing to lose / And there's noth-ing to prove"
    {2, NOTA_SI4,   192,  2}, {2, NOTA_SI4,   195,  2},
    {3, NOTA_DOS5,  198,  5}, {2, NOTA_SI4,   204,  5},
    {1, NOTA_LA4,   210,  5}, {2, NOTA_SI4,   216,  2},
    {2, NOTA_SI4,   219,  2}, {3, NOTA_DOS5,  222,  5},
    {2, NOTA_SI4,   228,  5}, {1, NOTA_LA4,   234,  5},

    // Compassos 11-12 (240..287): Clímax "Oh, oh, oh - oh! Danc-ing with my-self!"
    {3, NOTA_RE5,   240,  5}, {3, NOTA_DOS5,  246,  5},
    {2, NOTA_SI4,   252,  5}, {1, NOTA_LA4,   258,  5},
    {1, NOTA_SOLS4, 264,  2}, {2, NOTA_LA4,   267,  2},
    {1, NOTA_SOLS4, 270,  2}, {0, NOTA_FAS4,  273,  2},
    {0, NOTA_MI4,   276, 12}
};

// ============================================================
// FASE 5: AMERICAN IDIOT (Green Day) - Versão Estendida
// Riff + Verso 1 + Riff + Verso 2 + Refrão (Láb Maior)
// Grade 4/4 (24 ticks/compasso): Colcheia = 3 | Semínima = 6 | Mínima = 12
// ============================================================
static const NotaMusical partitura_fase5[] = {
    // Compassos 1-2 (0..47): Riff Principal
    {1, NOTA_SOLS4,   0,  2}, {1, NOTA_SOLS4,   3,  2},
    {1, NOTA_SOLS4,   6,  2}, {2, NOTA_DOS5,    9,  2},
    {2, NOTA_DOS5,   12,  5}, {0, NOTA_FAS4,   18,  2},
    {0, NOTA_FAS4,   21,  2}, {2, NOTA_DOS5,   24,  2},
    {1, NOTA_SOLS4,  27,  2}, {0, NOTA_FAS4,   30,  5},
    {3, NOTA_RES5,   36,  5}, {1, NOTA_SOLS4,  42,  5},

    // Compassos 3-4 (48..95): "Don't wan-na be an A-mer-i-can id-i-ot!"
    {2, NOTA_DO5,    48,  2}, {2, NOTA_DO5,    51,  2},
    {2, NOTA_DO5,    54,  2}, {3, NOTA_DOS5,   57,  2},
    {2, NOTA_DO5,    60,  2}, {1, NOTA_LAS4,   63,  2},
    {0, NOTA_SOLS4,  66,  2}, {1, NOTA_LAS4,   69,  2},
    {2, NOTA_DO5,    72,  5}, {1, NOTA_LAS4,   78,  2},
    {0, NOTA_SOLS4,  81,  2}, {0, NOTA_SOLS4,  84, 10},

    // Compassos 5-6 (96..143): Riff Interlúdio
    {1, NOTA_SOLS4,  96,  2}, {1, NOTA_SOLS4,  99,  2},
    {1, NOTA_SOLS4, 102,  2}, {2, NOTA_DOS5,  105,  2},
    {2, NOTA_DOS5,  108,  5}, {0, NOTA_FAS4,  114,  2},
    {0, NOTA_FAS4,  117,  2}, {2, NOTA_DOS5,  120,  2},
    {1, NOTA_SOLS4, 123,  2}, {0, NOTA_FAS4,  126,  5},
    {3, NOTA_RES5,  132,  5}, {1, NOTA_SOLS4, 138,  5},

    // Compassos 7-8 (144..191): "Don't want a na-tion un-der the new ma-ni-a!"
    {2, NOTA_DO5,   144,  2}, {2, NOTA_DO5,   147,  2},
    {2, NOTA_DO5,   150,  2}, {3, NOTA_DOS5,  153,  2},
    {2, NOTA_DO5,   156,  2}, {1, NOTA_LAS4,  159,  2},
    {0, NOTA_SOLS4, 162,  2}, {1, NOTA_LAS4,  165,  2},
    {2, NOTA_DO5,   168,  5}, {1, NOTA_LAS4,  174,  2},
    {0, NOTA_SOLS4, 177,  2}, {0, NOTA_SOLS4, 180, 10},

    // Compassos 9-10 (192..239): Refrão "Wel-come to a new kind of ten-sion" (Db -> Ab)
    {2, NOTA_DOS5,  192,  5}, {2, NOTA_DOS5,  198,  2},
    {2, NOTA_DOS5,  201,  2}, {2, NOTA_DO5,   204,  5},
    {1, NOTA_LAS4,  210,  5}, {2, NOTA_DO5,   216,  5},
    {1, NOTA_LAS4,  222,  5}, {0, NOTA_SOLS4, 228, 10},

    // Compassos 11-12 (240..287): "All a-cross the a-li-en na-tion" (Eb -> Ab)
    {3, NOTA_RES5,  240,  5}, {3, NOTA_RES5,  246,  2},
    {3, NOTA_RES5,  249,  2}, {2, NOTA_DOS5,  252,  5},
    {2, NOTA_DO5,   258,  5}, {1, NOTA_LAS4,  264,  5},
    {2, NOTA_DO5,   270,  5}, {0, NOTA_SOLS4, 276, 10},

    // Compassos 13-14 (288..335): Riff de Fecho Explosivo
    {1, NOTA_SOLS4, 288,  2}, {1, NOTA_SOLS4, 291,  2},
    {1, NOTA_SOLS4, 294,  2}, {2, NOTA_DOS5,  297,  2},
    {2, NOTA_DOS5,  300,  5}, {0, NOTA_FAS4,  306,  2},
    {0, NOTA_FAS4,  309,  2}, {2, NOTA_DOS5,  312,  2},
    {1, NOTA_SOLS4, 315,  2}, {0, NOTA_FAS4,  318,  5},
    {3, NOTA_RES5,  324,  5}, {1, NOTA_SOLS4, 330, 12}
};

// ============================================================
// FASE 6: MINOR THREAT (Minor Threat) - Versão Estendida
// Verso 1 + Refrão 1 + Verso 2 + Refrão 2 (G5 - A5 - F5 - C5)
// Grade 4/4 (24 ticks/compasso): Colcheia = 3 | Semínima = 6 | Mínima = 12
// ============================================================
static const NotaMusical partitura_fase6[] = {
    // Compassos 1-2 (0..47): Intro / Verso 1 "I don't mind, it's all the same"
    {1, NOTA_SOL4,    0,  5}, {1, NOTA_SOL4,    6,  2},
    {1, NOTA_SOL4,    9,  2}, {2, NOTA_LA4,    12,  5},
    {2, NOTA_LA4,    18,  2}, {2, NOTA_LA4,    21,  2},
    {0, NOTA_FA4,    24,  5}, {0, NOTA_FA4,    30,  2},
    {0, NOTA_FA4,    33,  2}, {1, NOTA_SOL4,   36,  5},
    {1, NOTA_SOL4,   42,  2}, {1, NOTA_SOL4,   45,  2},

    // Compassos 3-4 (48..95): "Looks like I'm play-ing the game!"
    {1, NOTA_SOL4,   48,  5}, {1, NOTA_SOL4,   54,  2},
    {1, NOTA_SOL4,   57,  2}, {2, NOTA_LA4,    60,  5},
    {2, NOTA_LA4,    66,  2}, {2, NOTA_LA4,    69,  2},
    {0, NOTA_FA4,    72,  5}, {0, NOTA_FA4,    78,  2},
    {0, NOTA_FA4,    81,  2}, {1, NOTA_SOL4,   84, 10},

    // Compassos 5-6 (96..143): Refrão 1 "We're just... a MI-NOR THREAT!"
    {3, NOTA_DO5,    96,  5}, {2, NOTA_LA4,   102, 10},
    {2, NOTA_LA4,   114,  5}, {0, NOTA_FA4,   120,  5},
    {1, NOTA_SOL4,  126,  5}, {3, NOTA_DO5,   132, 10},

    // Compassos 7-8 (144..191): "We're just... a MI-NOR THREAT!"
    {3, NOTA_DO5,   144,  5}, {2, NOTA_LA4,   150, 10},
    {2, NOTA_LA4,   162,  5}, {0, NOTA_FA4,   168,  5},
    {1, NOTA_SOL4,  174,  5}, {1, NOTA_SOL4,  180, 10},

    // Compassos 9-10 (192..239): Verso 2 "Early man walked a-way / Modern man takes control"
    {1, NOTA_SOL4,  192,  5}, {1, NOTA_SOL4,  198,  2},
    {1, NOTA_SOL4,  201,  2}, {2, NOTA_LA4,   204,  5},
    {2, NOTA_LA4,   210,  2}, {2, NOTA_LA4,   213,  2},
    {0, NOTA_FA4,   216,  5}, {0, NOTA_FA4,   222,  2},
    {0, NOTA_FA4,   225,  2}, {1, NOTA_SOL4,  228, 10},

    // Compassos 11-12 (240..287): Ponte "And it's not a warn-ing, it's a prom-ise!"
    {2, NOTA_LA4,   240,  5}, {2, NOTA_LA4,   246,  5},
    {3, NOTA_DO5,   252,  5}, {2, NOTA_LA4,   258,  5},
    {0, NOTA_FA4,   264,  5}, {0, NOTA_FA4,   270,  5},
    {1, NOTA_SOL4,  276, 10},

    // Compassos 13-14 (288..335): Refrão Final "We're just... a MI-NOR THREAT!"
    {3, NOTA_DO5,   288,  5}, {2, NOTA_LA4,   294, 10},
    {2, NOTA_LA4,   306,  5}, {0, NOTA_FA4,   312,  5},
    {1, NOTA_SOL4,  318,  5}, {3, NOTA_DO5,   324, 12}
};

// ============================================================
// FASE 7: SK8ER BOI (Avril Lavigne) - Versão Estendida
// Verso ("He was a boy, she was a girl") + Refrão Duplo (Ré Maior)
// Grade 4/4 (24 ticks/compasso): Colcheia = 3 | Semínima = 6 | Mínima = 12
// ============================================================
static const NotaMusical partitura_fase7[] = {
    // Compassos 1-2 (0..47): "He was a boy, she was a girl"
    {3, NOTA_RE5,     0,  5}, {2, NOTA_LA4,     6,  2},
    {1, NOTA_FAS4,    9,  2}, {0, NOTA_RE4,    12, 10},
    {0, NOTA_MI4,    24,  5}, {1, NOTA_FAS4,   30,  2},
    {1, NOTA_SOL4,   33,  2}, {2, NOTA_LA4,    36, 10},

    // Compassos 3-4 (48..95): "Can I make it an-y more ob-vi-ous?"
    {2, NOTA_SI4,    48,  2}, {2, NOTA_SI4,    51,  2},
    {2, NOTA_SI4,    54,  2}, {3, NOTA_DOS5,   57,  2},
    {3, NOTA_RE5,    60,  5}, {2, NOTA_LA4,    66,  5},
    {1, NOTA_FAS4,   72,  5}, {0, NOTA_MI4,    78,  5},
    {0, NOTA_RE4,    84, 10},

    // Compassos 5-6 (96..143): Refrão "He was a sk8-er boi, she said see ya la-ter boi"
    {3, NOTA_RE5,    96,  2}, {3, NOTA_RE5,    99,  2},
    {3, NOTA_RE5,   102,  2}, {3, NOTA_DOS5,  105,  2},
    {2, NOTA_LA4,   108,  5}, {1, NOTA_FAS4,  114,  5},
    {2, NOTA_SI4,   120,  2}, {2, NOTA_SI4,   123,  2},
    {2, NOTA_SI4,   126,  2}, {2, NOTA_LA4,   129,  2},
    {1, NOTA_FAS4,  132,  5}, {0, NOTA_RE4,   138,  5},

    // Compassos 7-8 (144..191): "He was-n't good e-nough for her!"
    {1, NOTA_SOL4,  144,  2}, {1, NOTA_SOL4,  147,  2},
    {1, NOTA_SOL4,  150,  5}, {1, NOTA_FAS4,  156,  5},
    {0, NOTA_MI4,   162,  5}, {1, NOTA_FAS4,  168,  5},
    {0, NOTA_RE4,   174, 14},

    // Compassos 9-10 (192..239): "Now he's a su-per-star, slam-min' on his gui-tar"
    {3, NOTA_RE5,   192,  2}, {3, NOTA_RE5,   195,  2},
    {3, NOTA_RE5,   198,  2}, {3, NOTA_DOS5,  201,  2},
    {2, NOTA_LA4,   204,  5}, {1, NOTA_FAS4,  210,  5},
    {2, NOTA_SI4,   216,  2}, {2, NOTA_SI4,   219,  2},
    {2, NOTA_SI4,   222,  2}, {3, NOTA_DOS5,  225,  2},
    {3, NOTA_RE5,   228,  5}, {2, NOTA_LA4,   234,  5},

    // Compassos 11-12 (240..287): "Does your pret-ty face see what he's worth?"
    {1, NOTA_SOL4,  240,  2}, {1, NOTA_SOL4,  243,  2},
    {1, NOTA_SOL4,  246,  5}, {1, NOTA_FAS4,  252,  5},
    {0, NOTA_MI4,   258,  5}, {1, NOTA_FAS4,  264,  5},
    {2, NOTA_LA4,   270,  5}, {3, NOTA_RE5,   276, 12}
};

// ============================================================
// FASE 8: CALIFORNIA GIRLS (Pop / Synth Hook + Refrão Completo)
// Grade 4/4 (24 ticks/compasso): Colcheia = 3 | Semínima = 6 | Mínima = 12
// ============================================================
static const NotaMusical partitura_fase8[] = {
    // Compassos 1-2 (0..47): Intro Groove (F - G - Am)
    {0, NOTA_FA4,     0,  5}, {0, NOTA_FA4,     6,  2},
    {1, NOTA_SOL4,    9,  2}, {2, NOTA_LA4,    12,  5},
    {3, NOTA_DO5,    18,  5}, {2, NOTA_LA4,    24,  5},
    {1, NOTA_SOL4,   30,  2}, {0, NOTA_FA4,    33,  2},
    {1, NOTA_SOL4,   36, 10},

    // Compassos 3-4 (48..95): Refrão "Ca-li-for-nia girls, we're un-for-get-ta-ble"
    {2, NOTA_LA4,    48,  8}, {1, NOTA_SOL4,   57,  2},
    {0, NOTA_FA4,    60,  2}, {0, NOTA_MI4,    63,  2},
    {0, NOTA_RE4,    66,  5}, {1, NOTA_SOL4,   72,  2},
    {2, NOTA_LA4,    75,  2}, {3, NOTA_DO5,    78,  5},
    {2, NOTA_LA4,    84,  2}, {1, NOTA_SOL4,   87,  2},
    {0, NOTA_FA4,    90,  5},

    // Compassos 5-6 (96..143): "Dai-sy Dukes, bi-ki-nis on top"
    {2, NOTA_LA4,    96,  5}, {1, NOTA_SOL4,  102,  2},
    {0, NOTA_FA4,   105,  2}, {0, NOTA_MI4,   108,  2},
    {0, NOTA_RE4,   111,  2}, {0, NOTA_RE4,   114,  8},
    {1, NOTA_SOL4,  123,  2}, {2, NOTA_LA4,   126,  2},
    {3, NOTA_DO5,   129,  5}, {2, NOTA_LA4,   135,  8},

    // Compassos 7-8 (144..191): "Sun-kissed skin so hot, we'll melt your pop-si-cle!"
    {2, NOTA_LA4,   144,  8}, {1, NOTA_SOL4,  153,  2},
    {0, NOTA_FA4,   156,  2}, {0, NOTA_MI4,   159,  2},
    {0, NOTA_RE4,   162,  5}, {1, NOTA_SOL4,  168,  2},
    {2, NOTA_LA4,   171,  2}, {3, NOTA_DO5,   174,  5},
    {2, NOTA_LA4,   180,  2}, {1, NOTA_SOL4,  183,  2},
    {0, NOTA_FA4,   186,  5},

    // Compassos 9-10 (192..239): Hook "Oh-oh-oh-oh-oh, oh-oh-oh-oh-oh!"
    {3, NOTA_DO5,   192,  5}, {3, NOTA_RE5,   198,  5},
    {3, NOTA_DO5,   204,  5}, {2, NOTA_LA4,   210,  2},
    {1, NOTA_SOL4,  213,  2}, {3, NOTA_DO5,   216,  5},
    {3, NOTA_RE5,   222,  5}, {3, NOTA_MI5,   228,  5},
    {3, NOTA_DO5,   234,  5},

    // Compassos 11-12 (240..287): Fecho "Ca-li-for-nia girls!"
    {2, NOTA_LA4,   240,  8}, {1, NOTA_SOL4,  249,  2},
    {0, NOTA_FA4,   252,  2}, {0, NOTA_MI4,   255,  2},
    {0, NOTA_RE4,   258,  5}, {1, NOTA_SOL4,  264,  5},
    {2, NOTA_LA4,   270,  5}, {0, NOTA_FA4,   276, 12}
};

// ============================================================
// FASE 9: BACK IN BLACK (AC/DC) - Riff Clássico + Verso + Refrão
// Tom Original: Mi menor / Lá Maior / Ré Maior (E5 - D5 - A5)
// Grade 4/4 (24 ticks/compasso): Colcheia = 3 | Semínima = 6 | Mínima = 12
// ============================================================
static const NotaMusical partitura_fase9[] = {
    // Compasso 1 (0..23): Riff Principal (E5 ... D5-D5-D5 ... A5-A5-A5)
    {0, NOTA_MI4,     0,  5},
    {2, NOTA_RE5,     6,  2}, {2, NOTA_RE5,     9,  2}, {2, NOTA_RE5,    12,  2},
    {1, NOTA_LA4,    15,  2}, {1, NOTA_LA4,    18,  2}, {1, NOTA_LA4,    21,  2},

    // Compasso 2 (24..47): Lick 1 pentatônico (G5 - E5 - D5 - B4 - A4 - G4 - E4)
    {3, NOTA_SOL5,   24,  2}, {3, NOTA_MI5,    27,  2},
    {2, NOTA_RE5,    30,  2}, {1, NOTA_SI4,    33,  2},
    {1, NOTA_LA4,    36,  2}, {0, NOTA_SOL4,   39,  2},
    {0, NOTA_MI4,    42,  5},

    // Compasso 3 (48..71): Riff Principal (E5 ... D5-D5-D5 ... A5-A5-A5)
    {0, NOTA_MI4,    48,  5},
    {2, NOTA_RE5,    54,  2}, {2, NOTA_RE5,    57,  2}, {2, NOTA_RE5,    60,  2},
    {1, NOTA_LA4,    63,  2}, {1, NOTA_LA4,    66,  2}, {1, NOTA_LA4,    69,  2},

    // Compasso 4 (72..95): Lick 2 cromático (A4 - G#4 - A4 - A#4 - B4)
    {1, NOTA_LA4,    72,  2}, {0, NOTA_SOLS4,  75,  2},
    {1, NOTA_LA4,    78,  2}, {1, NOTA_LAS4,   81,  2},
    {2, NOTA_SI4,    84,  5}, {2, NOTA_SI4,    90,  5},

    // Compassos 5-6 (96..143): Verso "Back in black, I hit the sack!"
    {2, NOTA_MI5,    96,  2}, {2, NOTA_MI5,    99,  2},
    {2, NOTA_MI5,   102,  5}, {2, NOTA_RE5,   108,  2},
    {2, NOTA_RE5,   111,  2}, {2, NOTA_RE5,   114,  5},
    {1, NOTA_DOS5,  120,  2}, {1, NOTA_DOS5,  123,  2},
    {1, NOTA_DOS5,  126,  5}, {1, NOTA_SI4,   132, 10},

    // Compassos 7-8 (144..191): "I've been too long, I'm glad to be back!"
    {2, NOTA_MI5,   144,  2}, {2, NOTA_MI5,   147,  2},
    {2, NOTA_MI5,   150,  5}, {2, NOTA_RE5,   156,  2},
    {2, NOTA_RE5,   159,  2}, {2, NOTA_RE5,   162,  5},
    {1, NOTA_LA4,   168,  5}, {2, NOTA_SI4,   174,  5},
    {0, NOTA_MI4,   180, 10},

    // Compassos 9-10 (192..239): Refrão "'Cause I'm BACK! Yes I'm BACK!"
    {1, NOTA_LA4,   192,  2}, {2, NOTA_SI4,   195,  2},
    {3, NOTA_DOS5,  198,  8}, {1, NOTA_SI4,   207,  2},
    {2, NOTA_DOS5,  210,  2}, {3, NOTA_RE5,   213,  8},
    {1, NOTA_LA4,   222,  2}, {2, NOTA_SI4,   225,  2},
    {3, NOTA_DOS5,  228, 10},

    // Compassos 11-12 (240..287): "Well I'm BACK, yes I'm BACK IN BLACK!"
    {2, NOTA_SI4,   240,  5}, {1, NOTA_SOL4,  246,  5},
    {2, NOTA_RE5,   252,  5}, {1, NOTA_LA4,   258,  5},
    {0, NOTA_MI4,   264,  5}, {2, NOTA_RE5,   270,  2},
    {2, NOTA_RE5,   273,  2}, {1, NOTA_LA4,   276, 12}
};

// ============================================================
// TABELA UNIFICADA DE FASES DO JOGO
// ============================================================
#define TOTAL_FASES 9

static const FaseMusical fases[TOTAL_FASES] = {
    {"1-ESTRELINHA",       partitura_fase1, sizeof(partitura_fase1) / sizeof(partitura_fase1[0]), 125000},
    {"2-BLITZKRIEG BOP",   partitura_fase2, sizeof(partitura_fase2) / sizeof(partitura_fase2[0]),  65000},
    {"3-CHICLETEIRO EU",   partitura_fase3, sizeof(partitura_fase3) / sizeof(partitura_fase3[0]),  70000},
    {"4-DANCING W MYSELF", partitura_fase4, sizeof(partitura_fase4) / sizeof(partitura_fase4[0]),  62000},
    {"5-AMERICAN IDIOT",   partitura_fase5, sizeof(partitura_fase5) / sizeof(partitura_fase5[0]),  58000},
    {"6-MINOR THREAT",     partitura_fase6, sizeof(partitura_fase6) / sizeof(partitura_fase6[0]),  56000},
    {"7-SK8ER BOI",        partitura_fase7, sizeof(partitura_fase7) / sizeof(partitura_fase7[0]),  62000},
    {"8-CALIFORNIA GIRLS", partitura_fase8, sizeof(partitura_fase8) / sizeof(partitura_fase8[0]),  72000},
    {"9-BACK IN BLACK",    partitura_fase9, sizeof(partitura_fase9) / sizeof(partitura_fase9[0]),  76000}
};

#endif // MUSICAS_H