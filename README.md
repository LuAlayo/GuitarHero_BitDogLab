# Guitar Dog 
## O Guitar Hero do BitDogLab

![Guitar Hero setup](imagens/image.jpg)

Adaptação do jogo Guitar Hero para a placa BitDogLab V7 (RP2040), em MicroPython. Usa o display OLED para servir de tela do jogo e utiliza os botões A, B e C para fins de jogabilidade. A quantidade de cordas do jogo foi reduzida para três justamente para o uso dos três botões disponíveis na placa. Projeto da disciplina EA801 - Laboratório de Projeto de Sistemas Embarcados (FEEC/Unicamp).

## Etapa Inicial de trabalho

Como etapa inicial de nosso trabalho, a fim termos uma melhor visão mental dos requisitos do projeto Guitar Dog e, assim, facilitar e orientar o início de nossa etapa de programação, desenvolvemos uma conceptualização inicial do projeto através da elaboração de um primeiro diagrama de blocos que caracterizasse o funcionamento padrão do famoso jogo Guitar Hero de forma que pudesse ser incorporado à nossa placa. 

![Guitar Dog diagrama](imagens/Diagrama_blocos.png)

Considerando as recomendações docentes, optamos por desconsiderar a adaptação de funcionalidades musicais e sonoras para essa versão inicial do projeto de forma que essas partes associadas foram deixadas de lado no momento. O sistema de pontuação também foi simplificado, desconsiderando-se o efeito de multiplicadores e, no lugar, apenas registrando acertos e os pontuando com um ganho de 10 pontos.

Através do diagrama desenvolvemos a conceptualização do Guitar Dog como, fundamentalmente, uma máquina de estados finitos constituída por quatro diferentes estados com suas respectivas funcionalidades:

### - Tela de Início
Deve ter capacidade de identificar quando é pressionado qualquer dos três botões para garantir transição de estado. Display de tela informando nome do jogo de instruindo o jogador.

### - Tela de Load
Estado temporário, a transição para o próximo estado tem como condicional apenas a passagem do tempo. Display de contagem regressiva feita na tela, disponibiliza tempo para o jogador se preparar para o jogo e garante transição mais suave do que apenas abruptamente iniciar o jogo de imediato.

### - Tela de Jogo
Deve ter capacidade de "lançar" as telas graficamente na tela para display gráfico do jogo, atualizando a tela com o passar do tempo e fazendo as notas "descerem". Deve ser capaz de identificar quando um dos botões é pressionado e checar se ele foi apertado corretamente de acordo com a posição ou não da nota na "corda" correspondente. A um acerto são contabilizados 10 pontos. Abandonamos temporariamente o conceito de diferentes estágios ou níveis no jogo, desenvolvemos apenas uma única fase que é sempre a mesma.

### - Tela de Pontuação
Garantimos a rejogabilidade fazendo com que, após a tela de pontuação, retorne-se novamente à tela inicial. Esse estado, como o da tela de load, também vai ter transição associada a uma passagem temporal, após um tempo hábil para leitura da pontuação, retornamos à tela inicial.
### Demonstração de Resultado atual
[Assista à demonstração do projeto](https://youtube.com/shorts/ToGgFWhk9B0?feature=share)
