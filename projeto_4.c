#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "ssd1306.h" // Inclui a biblioteca do display OLED

// Definições dos pinos para o I2C e Joystick
#define I2C_PORT i2c1
#define I2C_SDA_PIN 14
#define I2C_SCL_PIN 15
#define JOYSTICK_X_PIN 26
#define ENDERECO_OLED 0x3C // Endereço I2C padrão para displays 0.96"

int main() {
    stdio_init_all();

    // ==========================================
    // INICIALIZAÇÃO DO ADC (Joystick)
    // ==========================================
    adc_init();
    adc_gpio_init(JOYSTICK_X_PIN); 
    adc_select_input(0);           

    // ==========================================
    // INICIALIZAÇÃO DO I2C (Display OLED)
    // ==========================================
    i2c_init(I2C_PORT, 400 * 1000); 
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    // ==========================================
    // INICIALIZAÇÃO DO DISPLAY OLED
    // ==========================================
    ssd1306_t disp; // Cria a estrutura do display
    // Inicializa o display (128x64 pixels)
    ssd1306_init(&disp, 128, 64, false, ENDERECO_OLED, I2C_PORT); 
    ssd1306_config(&disp);
    ssd1306_fill(&disp, false); // Limpa a tela com pixels apagados (false)
    ssd1306_send_data(&disp);   // Envia o comando de limpar para a tela

    while (true) {
        // Coleta o sinal analógico (0 a 4095)
        uint16_t adc_value = adc_read();

        // Converte para tensão (0V a 3.3V)
        float voltage = (adc_value * 3.3f) / 4095.0f;

        // Cria a string com o valor formatado
        char str_voltage[32];
        snprintf(str_voltage, sizeof(str_voltage), "%.2f V", voltage);

        // Imprime no terminal (útil para o relatório e debug)
        printf("Leitura ADC: %d | Tensao: %s\n", adc_value, str_voltage);

        // ==========================================
        // ATUALIZAÇÃO DO DISPLAY OLED
        // ==========================================
        ssd1306_fill(&disp, false); // Apaga os dados anteriores da tela

        // Desenha os textos (Posição X, Posição Y, Texto)
        // Nota: A função exata de escrita varia (pode ser ssd1306_draw_string ou ssd1306_draw_text)
        ssd1306_draw_string(&disp, 10, 10, 1, "Eixo X Joystick:");
        
        // Escreve a tensão um pouco mais abaixo
        ssd1306_draw_string(&disp, 35, 30, 2, str_voltage); 

        // Envia as modificações para atualizar a tela fisicamente
        ssd1306_send_data(&disp);

        // Tempo de amostragem exigido: 500 ms
        sleep_ms(500);
    }

    return 0;
}