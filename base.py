# ===============================================================
# PROJETO 1 - EA801 - Laboratório Projeto de Sistemas Embarcados
#
# Luíza Maria Cabel Alayo, RA: 243537
# 
# Rafael Mattos Sacramento, RA: 247349
#
# ===============================================================

from machine import Pin, SoftI2C
import utime
from ssd1306 import SSD1306_I2C

# ============================================================
# 1. CONFIGURAÇÃO DE HARDWARE (BitDogLab V7)
# ============================================================
# Configuração do Display Oled 128 x 64 SSD1306
i2c = SoftI2C(scl=Pin(3), sda=Pin(2), freq=50000)
oled = SSD1306_I2C(128, 64, i2c, addr=0x3C)

# Configuração dos Botões A, B e C, botão pressionado => nível lógico baixo
btn_a = Pin(5, Pin.IN, Pin.PULL_UP)
btn_b = Pin(6, Pin.IN, Pin.PULL_UP)
btn_c = Pin(10, Pin.IN, Pin.PULL_UP)

# ============================================================
# 2. DEFINIÇÃO DOS ESTADOS DA MEF QUE REPRESENTA O SISTEMA
# ============================================================
ESTADO_TELA_INICIAL = 0
ESTADO_LOADING = 1
ESTADO_JOGO = 2
ESTADO_EM_DESENVOLVIMENTO = 3

estado_atual = ESTADO_TELA_INICIAL

# ============================================================
# 3. FUNÇÕES GRÁFICAS AUXILIARES PARA FUNCIONAMENTO DA TELA
# ============================================================
def desenhar_tela_inicial():
    oled.fill(0)
    oled.text("GUITAR HERO", 20, 8)
    oled.hline(16, 20, 96, 1)
    oled.text("Pressione:", 28, 30)
    oled.text("[A] [B] ou [C]", 12, 42)
    oled.text("para iniciar", 16, 52)
    oled.show()
    
def desenhar_tela_load():
    oled.fill(0)
    oled.text("GUITAR HERO", 20, 8)
    oled.hline(16, 20, 96, 1)
    oled.text("Preparando jogo", 7, 30)
    oled.text("Aguarde", 35, 42)
    oled.text("...", 50, 52)
    oled.show()
    
def desenhar_base_guitarra():
    # Função para desenhar a Tela de Jogo de acordo com o escopo estabelecido
    oled.fill(0)
    
    # Limites da guitarra
    oled.text("_____________", 7, 2)
    oled.text("_____________", 7, 52)
    
    # Cordas e alvos '0' com indicador do botão correspondente
    oled.text("------------0 A", 7, 10)
    oled.text("------------0 B", 7, 30)
    oled.text("------------0 C", 7, 50)
    
    # Separadores de fret (barras verticais)
    for y_fret in (10, 15, 20, 25, 30, 35, 40, 45, 50):
        oled.text("   |    |    ", 7, y_fret)

# Temporário, tela de desenvolvimento
def desenhar_tela_desenvolvimento():
    oled.fill(0)
    oled.rect(0, 0, 128, 64, 1)
    oled.rect(2, 2, 124, 60, 1)
    oled.text("STATUS:", 36, 16)
    oled.text("IN DEVELOPMENT", 8, 32)
    oled.text("BitDogLab V7", 16, 46)
    oled.show()


# ============================================================
# 4. FUNÇÕES AUXILIARES DE JOGABILIDADE
# ============================================================
def verificar_botoes(): # Função para checar se qualquer botão foi pressionado
    return (btn_a.value() == 0) or (btn_b.value() == 0) or (btn_c.value() == 0)



# Função auxiliar que faz o jogo funcionar comandando o surgimento de notas na tela com atraso definido
# para iniciar rodada, e executa a animação das notas se movendo ao longo da corda com o passar do tempo.
# ta, tb, tc: tempo em ticks (de 350 ms) em que cada nota é disparada
def rodada_jogo(ta, tb, tc):
    INTERVALO_TICK_MS = 350
    
    # Posição atual de cada nota na pista (-1 = ainda não lançada, 0 a 12 = na tela, 13 = finalizada)
    pos_a = -1
    pos_b = -1
    pos_c = -1
    
    temp_notas = 0 # Variável auxiliar para identificar hora exata de lançar cada nota, conta ticks 
    proximo_tick = utime.ticks_ms() 
    
    while True:
        agora = utime.ticks_ms()
        
        # Só atualiza nota na tela quando passam 350 ms
        if utime.ticks_diff(agora, proximo_tick) >= 0:
            proximo_tick = utime.ticks_add(proximo_tick, INTERVALO_TICK_MS)
            
            # Verifica se é o momento correto de lançar cada nota
            if temp_notas >= ta and pos_a < 13:
                pos_a += 1
            if temp_notas >= tb and pos_b < 13:
                pos_b += 1
            if temp_notas >= tc and pos_c < 13:
                pos_c += 1
                
            # Redesenha desenho padrão da guitarra
            desenhar_base_guitarra()
            
            # Representa a nota com um "X" sobreposto na guitarra na posição correspondente
            if 0 <= pos_a <= 12:
                x_a = 7 + (8 * pos_a)
                oled.fill_rect(x_a, 10, 8, 8, 0) 
                oled.text("X", x_a, 10)
                
            if 0 <= pos_b <= 12:
                x_b = 7 + (8 * pos_b)
                oled.fill_rect(x_b, 30, 8, 8, 0)
                oled.text("X", x_b, 30)
                
            if 0 <= pos_c <= 12:
                x_c = 7 + (8 * pos_c)
                oled.fill_rect(x_c, 50, 8, 8, 0)
                oled.text("X", x_c, 50)
            
            # Atualiza representação no display
            oled.show()
            
            temp_notas += 1
            
            # Condição de encerramento da rodada, todas as notas foram tocadas
            if pos_a >= 13 and pos_b >= 13 and pos_c >= 13:
                break

# ============================================================
# 5. LOOP PRINCIPAL DE CONTROLE
# ============================================================
desenhar_tela_inicial()

while True:
    if estado_atual == ESTADO_TELA_INICIAL:
        if verificar_botoes():
            utime.sleep_ms(50) # Espera ativa para evitar debounce
            if verificar_botoes():
                estado_atual = ESTADO_LOADING # Mudança de estado
                desenhar_tela_load()
                while verificar_botoes():
                    utime.sleep_ms(20)

    elif estado_atual == ESTADO_LOADING:
        for segundos in range(5, 0, -1): # Estabelece contagem regressiva para tela de load
            oled.fill_rect(50, 52, 24, 8, 0)
            oled.text(str(segundos), 60, 52)
            oled.show()
            utime.sleep_ms(1000)
            
        estado_atual = ESTADO_JOGO # Mudança de estado
        
    elif estado_atual == ESTADO_JOGO:
        # Execução de nosso estágio teste do jogo com três rodadas
        rodada_jogo(33, 18, 3)
        rodada_jogo(15, 0, 0)
        rodada_jogo(0, 8, 25)
        
        # Ao término das notas, segue para a tela de desenvolvimento
        estado_atual = ESTADO_EM_DESENVOLVIMENTO # Mudança de estado 
        desenhar_tela_desenvolvimento()

    elif estado_atual == ESTADO_EM_DESENVOLVIMENTO:
        utime.sleep_ms(100)
