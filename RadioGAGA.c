#include <stdio.h>
#include <stdlib.h>
#include <pico/multicore.h>

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "aux/buzzer.h"
#include "hardware/i2c.h"
#include "aux/io_handler.h"
#include "aux/ssd1306.h"

int main() {
    // Inicializa o SDK
    stdio_init_all();
    buttons_init();

    // Interrupção do botão B
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    
    // Configura o pino do buzzer como saída PWM
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);

    // Reproduz a melodia
    while (true) {
        play_song(just_dance);
        sleep_ms(5000);
    }
    
    return 0;
}