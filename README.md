# Monitoramento de Joystick Analógico com Display OLED (Raspberry Pi Pico)

Este projeto foi desenvolvido como parte de uma atividade acadêmica da disciplina de Microcontroladores e Microprocessadores (ELET0021) da UNIVASF, ministrada pelo Prof. Dr. Ricardo Menezes Prates[cite: 10]. O objetivo principal é empregar a interface de comunicação I2C e o conversor A/D do microcontrolador RP2040 para coletar o sinal analógico do eixo X de um joystick e plotar o respectivo valor de tensão em um display OLED[cite: 10].

O projeto foi validado utilizando o simulador **Wokwi** integrado com as especificações da placa pedagógica **BitDogLAB**[cite: 10].

## Como Funciona

1. **Leitura Analógica (ADC):** O pino GP26 do microcontrolador está configurado para realizar a leitura do sinal analógico gerado pelo eixo horizontal (X) do joystick[cite: 2, 5]. 
2. **Processamento dos Dados:** O firmware captura o valor numérico do ADC, que varia de 0 a 4095, e converte esse valor proporcionalmente para uma escala de tensão real entre 0V e 3.3V[cite: 5].
3. **Atualização no Display (I2C):** Utilizando o protocolo I2C através dos pinos GP14 e GP15, o microcontrolador envia o valor da tensão formatado para o display OLED no endereço 0x3C[cite: 5]. Essa atualização ocorre continuamente respeitando o tempo de amostragem exigido de 500 milissegundos[cite: 5, 10].

## Componentes e Conexões

* **Placa Principal:** Raspberry Pi Pico W (BitDogLAB)[cite: 10]
* **Joystick Analógico:** Pino GP26 (Eixo HORZ / X)[cite: 2, 5]
* **Display OLED (0,96" 128x64):** Pinos GP14 (SDA) e GP15 (SCL)[cite: 2, 5, 10]

## Instruções de Uso

Para compilar e executar a simulação deste projeto em sua máquina local, siga os passos abaixo:

1. **Pré-requisitos:** Certifique-se de ter o Visual Studio Code (VS Code) instalado, juntamente com a extensão oficial do **Raspberry Pi Pico** e a extensão do simulador **Wokwi**[cite: 10].
2. **Preparação:** Clone este repositório contendo os arquivos fonte em linguagem C e as bibliotecas do display (`ssd1306.c` e `font.h`), em seguida abra a pasta no VS Code[cite: 1, 3, 7, 10].
3. **Configuração do CMake:** Pressione `F1`, digite `CMake: Configure` e selecione o compilador adequado para o Pico SDK[cite: 10]. O `CMakeLists.txt` já está preparado para vincular as bibliotecas `hardware_adc` e `hardware_i2c`[cite: 1].
4. **Compilação:** Na barra inferior do VS Code, clique em "Compile" para compilar o código em C e gerar o arquivo binário executável do projeto[cite: 10].
5. **Simulação:** Abra o arquivo `diagram.json` e inicie a simulação[cite: 2]. Mova a haste do joystick horizontalmente para observar o display atualizando a leitura de tensão em tempo real.

## Links do Projeto

* Pasta Drive com Vídeo Ensaio (Demonstração) e Arquivos: https://drive.google.com/drive/folders/1h-QlxvdgR7tw_el1S8eaybN_us9Cav22?usp=sharing