#include <stdio.h>
#include <stdlib.h>
#include <pico/multicore.h>

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "aux/buzzer.h"
#include "hardware/i2c.h"
#include "pico/bootrom.h"
#include "aux/io_handler.h"
#include "aux/ssd1306.h"

ssd1306_t ssd;
bool cor = true;

uint16_t adc_value_x;
uint16_t adc_value_y; 

// Posição inicial da seta
int8_t line = 6; 

int8_t song = 0;

// Mapeia as posições das músicas no menu
const int8_t menu_positions[] = {6, 17, 28, 39, 50};  
#define MENU_SIZE (sizeof(menu_positions) / sizeof(menu_positions[0]))

static volatile uint32_t last_press_B = 0;
static volatile uint32_t last_press_PB = 0;

void buzzer_task() {
    while (true) {
        if(song == 1) {
            song = 0;
            play_song(bad_romance);  
        }
        else if(song == 2) {
            song = 0;
            play_song(just_dance);    
        }
        else if(song == 3) {
            song = 0;
            play_song(bloody_mary);   
        }
        else if(song == 4) {
            song = 0;
            play_song(poker_face);  
        }
        else if(song == 5) {
            song = 0;
            play_song(paparazzi);  
        }
        sleep_ms(500); 
    }
}

void gpio_irq_handler(uint gpio, uint32_t events) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    // Verifica se o botão B foi pressionado
    if (gpio == BUTTON_B && (events & GPIO_IRQ_EDGE_FALL)) {
        if (current_time - last_press_B > DEBOUNCE_TIME_US) {
            last_press_B = current_time;
            reset_usb_boot(0, 0); // Entra no modo de boot USB
        }
    } 
    // Verifica se o botão do joystick foi pressionado
    else if (gpio == JOYSTICK_PB && (events & GPIO_IRQ_EDGE_FALL)) {
        if (current_time - last_press_PB > DEBOUNCE_TIME_US) {
            last_press_PB = current_time;
            if(line == 6) {
                song = 1;
            }
            else if(line == 17) {
                song = 2;
            }
            else if(line == 28) {
                song = 3;
            }
            else if(line == 39) {
                song = 4;
            }
            else if(line == 50) {
                song = 5;
            }
        }
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
    gpio_set_irq_enabled_with_callback(JOYSTICK_PB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    // Configura o pino do buzzer como saída PWM
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);

    // Inicia a thread do buzzer no core 1
    multicore_launch_core1(buzzer_task); 

    int current_option = 0; // Índice da opção selecionada no menu
    int last_option = -1; // Guarda a última posição para evitar redesenhos desnecessários

    while (true) {
        joystick_read(&adc_value_x, &adc_value_y);
        printf("%d", song);

        // Move para cima
        if (adc_value_y > MAX_JOYSTICK) {
            if (current_option == 0) {
                current_option = MENU_SIZE - 1; 
            } else {
                current_option--;
            }
            sleep_ms(200); // Pequeno delay para evitar movimentação excessiva
        }
        // Move para baixo
        else if (adc_value_y < MIN_JOYSTICK) {
            if (current_option == MENU_SIZE - 1) {
                current_option = 0; 
            } else {
                current_option++;
            }
            sleep_ms(200);
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

        sleep_ms(100);
    }

    return 0;
}
