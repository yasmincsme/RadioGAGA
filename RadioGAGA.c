#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "aux/buzzer.h"

int main() {
    // Inicializa o SDK
    stdio_init_all();

    // Configura o pino do buzzer como saída PWM
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);

    // Reproduz a melodia
    play_song(bad_romance);

    return 0;
}