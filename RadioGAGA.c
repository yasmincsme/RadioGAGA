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

static volatile int song = 0;

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
    
    while (true) {
        ssd1306_fill(&ssd, !cor);
		 
        ssd1306_draw_string(&ssd, "Bad Romance", 20, 6);  
        ssd1306_draw_string(&ssd, "Just Dance", 20, 17);  
        ssd1306_draw_string(&ssd, "Bloody Mary", 20, 28);  
        ssd1306_draw_string(&ssd, "Poker Face", 20, 39); 
        ssd1306_draw_string(&ssd, "Paparazzi", 20, 50);   

        ssd1306_draw_char(&ssd, '>', 8, 50);

        ssd1306_rect(&ssd, 0, 0, 128, 64, cor, !cor);

        ssd1306_send_data(&ssd); 

        sleep_ms(500);
    }

    return 0;
}
