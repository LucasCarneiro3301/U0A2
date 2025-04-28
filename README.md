# U0A2
Ohmímetro digital baseado no RP2040 capaz de medir resistores de 510Ω a 100kΩ, identificar o valor E24 mais próximo e exibir no display OLED o valor da resistência junto com as cores correspondentes das faixas do resistor. Este projeto consiste numa modificação do ohmímetro desenvolvido em:  [Ohmímetro](https://github.com/wiltonlacerda/EmbarcaTechResU1RevOhm.git).

__Aluno:__
Lucas Carneiro de Araújo Lima

## ATIVIDADE 

__Para este trabalho, os seguintes componentes e ferramentas se fazem necessários:__
1) Microcontrolador Raspberry Pi Pico W.
2) Ambiente de trabalho VSCode.
3) LEDs RGB.
4) _Display_ SSD1306.
5) 1 Botão Pull-Up.
7) Ferramenta educacional BitDogLab.
8) Matriz de LEDs 5x5.
9) 1 Protoboard.
10) 1 resistor de 10000 Ohms.
11) 1 ou mais resistores para medição.
12) 3 _jumpers_.
13) Cabo USB.

__O resultado do projeto pode ser assistido através deste link: [Vídeo de Apresentação - Ohmímetro Digital com Conversão para Série E24 e Identificação de Códigos de Cores.](https://youtu.be/OLpv9MqyvTM?si=C90EpFGFlh7SRtJ5).__

## Instruções de Uso

### 1. Clone o repositório
Abra o terminal e execute o comando abaixo para clonar o repositório em sua máquina:
```bash
git clone https://github.com/LucasCarneiro3301/U0A2.git
```

### 2. Configure o ambiente de desenvolvimento
Certifique-se de que o [SDK do Raspberry Pi Pico](https://github.com/raspberrypi/pico-sdk) esteja instalado e configurado corretamente no seu sistema.

### 3. Instale o CMake
Certifique-se de que o [CMake](https://cmake.org/download/) esteja instalado e configurado corretamente no seu sistema.

### 4. Conexão com a Rapberry Pico
1. Conecte o Raspberry Pi Pico ao seu computador via USB.
2. Inicie o modo de gravação pressionando o botão **BOOTSEL** e **RESTART**.
3. O Pico será montado como um dispositivo de armazenamento USB.
4. Execute através do comando **RUN** a fim de copiar o arquivo `U0A2.uf2` para o Pico.
5. O Pico reiniciará automaticamente e executará o programa.

### 5. Observações (IMPORTANTE !!!)
2. Manuseie a placa com cuidado.

## Recursos e Funcionalidades

### 1. Botão

| BOTÃO                            | DESCRIÇÃO                                     | 
|:----------------------------------:|:---------------------------------------------:|
| B                                  | Modo de Gravação              | 

### 2. Display OLED
Exibe informações sobre o erro relativo, as cores das faixas do resistor, o valor médio do ADC medido e o valor da resistência convertido para a série E24.

### 3. Matriz de LEDs (5x5)
Representa visualmente as cores das faixas de um resistor. 

### 4. Interrupções
As interrupções são usadas para detectar a borda de descida do Botão B de forma assíncrona, com tratamento de debounce para evitar leituras falsas por oscilações elétricas, eliminando a necessidade de polling contínuo.

### 5. Comunicação Serial
Envia mensagens do sistema via USB para o terminal do computador.

### 6. Conversão Digital-Analógica
O pino 28 conecta o resistor ao canal 2 do ADC do RP2040, permitindo a conversão do sinal de tensão analógico em um valor digital. 


