#include <stdio.h>
#include <stdlib.h>
#include <pico/multicore.h>

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "aux/buzzer.h"
#include "hardware/i2c.h"
#include "aux/io_handler.h"
#include "aux/ssd1306.h"

ssd1306_t ssd;
bool cor = true;

uint16_t adc_value_x;
uint16_t adc_value_y; 

int8_t line = 6; // Posição inicial da seta

static volatile int song = 0;

// Mapeia as posições das músicas no menu
const int8_t menu_positions[] = {6, 17, 28, 39, 50};  
#define MENU_SIZE (sizeof(menu_positions) / sizeof(menu_positions[0]))

void buzzer_task() {
    while (true) {
        switch (song) {
            case 1:
                play_song(bad_romance);
                break;
            case 2:
                play_song(just_dance);
                break;
            case 3:
                play_song(bloody_mary);
                break;
            case 4:
                play_song(poker_face);
                break;
            case 5:
                play_song(paparazzi);
                break;
            default:
                break;
        }
        sleep_ms(500);
    }
}

int main() {
    // Inicializa o SDK
    stdio_init_all();
    joystick_init();
    buttons_init();
    I2C_init();
    SSD1306_clear(&ssd);

    // Interrupção do botão B
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    
    // Configura o pino do buzzer como saída PWM
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);

    // Inicia a thread do buzzer no core 1
    multicore_launch_core1(buzzer_task); 

    int current_option = 0; // Índice da opção selecionada no menu
    int last_option = -1; // Guarda a última posição para evitar redesenhos desnecessários

    while (true) {
        // Lê valores do joystick
        joystick_read(&adc_value_x, &adc_value_y);
        printf("Joystick Y: %d\n", adc_value_y); // Debug

        // Move para cima
        if (adc_value_y > MAX_JOYSTICK) {
            if (current_option == 0) {
                current_option = MENU_SIZE - 1; 
            } else {
                current_option--;
            }
        }
        // Move para baixo
        else if (adc_value_y < MIN_JOYSTICK) {
            if (current_option == MENU_SIZE - 1) {
                current_option = 0; 
            } else {
                current_option++;
            }
        }

        // Atualiza a posição da seta
        line = menu_positions[current_option];

        // Evita redesenhar desnecessariamente
        if (current_option != last_option) {
            last_option = current_option;

            // Limpa a tela
            ssd1306_fill(&ssd, !cor);

            // Desenha as opções do menu
            ssd1306_draw_string(&ssd, "Bad Romance", 20, 6);  
            ssd1306_draw_string(&ssd, "Just Dance", 20, 17);  
            ssd1306_draw_string(&ssd, "Bloody Mary", 20, 28);  
            ssd1306_draw_string(&ssd, "Poker Face", 20, 39); 
            ssd1306_draw_string(&ssd, "Paparazzi", 20, 50);   

            // Desenha a borda da tela
            ssd1306_rect(&ssd, 0, 0, 128, 64, cor, !cor);

            // Desenha a seta na posição correta
            ssd1306_draw_char(&ssd, '>', 7, line);

            // Envia os dados para o display
            ssd1306_send_data(&ssd);
        }

        sleep_ms(200);
    }

    return 0;
}

