##  Projeto e Implementação de Sistema de Elevador

### Objetivo

Elaborar um protótipo de elevador utilizando o LaunchPad MSP430FR2355  da Texas Instruments.


### Introdução
O projeto desenvolvido é um protótipo de Elevador. A escolha desta aplicação é pautada na intenção de se utilizar diversos os conceitos de microcontroladores, portando são abordados os seguintes temas:

1. GPIO (General Purpose Input/Output)
2. Timers(Temporizador/Contador)
3. Interrupções
4. ADC (Conversor Analógico Digital)
5. Protocolos:
  - UART
  - I2C
  - SPI


O sistema do Elevador atende três andares diferentes (0, 1 e 2), portanto o usuário que se encontra dentro da cabine é capaz de acessar outros dois pavimentos de uma edificação. A cabine é movimentada verticalmente, em ambas as direções, através de um Motor DC, com caixa de redução acoplada, que é alimentado entre 3 e 6V. A velocidade de rotação do motor é controlada por um potenciômetro, conectado em série, responsável
por gerar uma queda de tensão variável nos terminais da máquina. O sentido de rotação do motor é controlado pelo Módulo Driver Motor com Dupla Ponte H – L298N, cujos bits de controle são pinos (2) de saída do microcontrolador MSP430. O L298N é alimentado por uma bateria de 9V, que é responsável por fornecer potência ao Motor DC.


O controle da cabine é realizado por botões que indicam o destino do elevador em cada ciclo de operação. O controle do elevador só é possível caso sua posição seja conhecida, neste projeto isso é possível por meio de três módulos sensores magnéticos fixos (um para cada andar) que ao ser aproximado de um imã corta alimentação do motor parando a cabine no andar de destino. A caixa de redução acoplada evita o giro do Motor DC quando não está alimentado, e tem torque suficiente para manter a cabine parada.
Além disso o projeto apresenta a aplicação de um LCD, de duas linhas e 16 colunas de caracteres, que se comunica com o MSP através da interface I2C e indica se a cabine do elevador está subindo ou descendo, além do andar de destino e ainda monitora a queda de tensão nos terminais do potenciômetro que controla a velocidade do Motor DC (dois canais analógicos obtidos através do ADC, as amostras destes canais são subtraídas em software), ainda existe um display de sete segmentos indicando a posição atual da cabine do Elevador (0, 1 ou 2).

### Desenvolvimento
O ciclo de operação do Elevador se inicia em estado de repouso, no andar 0. Após comandar um destino para o Elevador, através de botões (1 ou 2), o motor DC será ligado. Ao chegar no andar de destino, após sensor magnético correspondente ser acionado, o motor DC é desligado finalizando o ciclo de operação. Nesta etapa a variável de “estado atual” do elevador é atualizada. Após finalizado o ciclo de operação o elevador pode ser
comandado para qualquer outro andar através dos botões (push button) e o ciclo se repete. Importante reforçar que, para respeitar a aplicação real e também preservar os dispositivos, a inversão de giro do motor durante um ciclo de operação foi vetada por software através de flags. Os estados dos três sensores são verificados a cada meio segundo (0,5 s). Isso é
possível inicializando um Timer, em modo UP, utilizando recursos de comparação e interrupção. Após interrupção, a cada 0,5s, os estados dos sensores são captados. O estado atual do Elevador é atualizado, indicando no display de sete segmentos e no LCD, por exemplo: quando o elevador está no andar 0 e o comando de destino é para o andar 2, o LCD indica que o elevador está subindo e o andar de destino (2), ao passar pelo sensor
do andar 1 o display de sete segmentos é atualizado informando que a posição atual ou o último andar pelo qual o elevador passou foi o andar 1, após chegar no destino (2) o display também é atualizado indicando a posição 2 e o LCD indica que o estado atual é 2 e o comando de destino como “nda”, porque não há comando, em outras palavras o display de sete segmentos atualiza a posição do elevador em tempo real simulando uma
situação real deste sistema. Após o comando realizando nos botões para partida do elevador o LCD indica se o elevador está subindo, descendo e monitora o nível de queda de tensão aplicado sobre no potenciômetro em série com o motor, são necessários dois canais de amostragem. Ambos os canais são conectadas como entradas do MSP e suas leituras são amostradas e convertidas pelo ADC, após isso são subtraídas as leituras indicando a queda de tensão. 
