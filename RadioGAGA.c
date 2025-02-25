#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Definições das notas musicais
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define PAUSE 0

// Velocidade da música
int tempo = 144;

// Pino do buzzer (GPIO do Raspberry Pi Pico)
const uint buzzer_pin = 15;

// Melodia do Tetris
int melody[] = {
    NOTE_E5, 4,  NOTE_B4, 8,  NOTE_C5, 8,  NOTE_D5, 4,  NOTE_C5, 8,  NOTE_B4, 8,
    NOTE_A4, 4,  NOTE_A4, 8,  NOTE_C5, 8,  NOTE_E5, 4,  NOTE_D5, 8,  NOTE_C5, 8,
    NOTE_B4, -4, NOTE_C5, 8,  NOTE_D5, 4,  NOTE_E5, 4,
    NOTE_C5, 4,  NOTE_A4, 4,  NOTE_A4, 8,  NOTE_A4, 4,  NOTE_B4, 8,  NOTE_C5, 8,

    NOTE_D5, -4, NOTE_F5, 8,  NOTE_A5, 4,  NOTE_G5, 8,  NOTE_F5, 8,
    NOTE_E5, -4, NOTE_C5, 8,  NOTE_E5, 4,  NOTE_D5, 8,  NOTE_C5, 8,
    NOTE_B4, 4,  NOTE_B4, 8,  NOTE_C5, 8,  NOTE_D5, 4,  NOTE_E5, 4,
    NOTE_C5, 4,  NOTE_A4, 4,  NOTE_A4, 4,  PAUSE, 4,

    NOTE_E5, 4,  NOTE_B4, 8,  NOTE_C5, 8,  NOTE_D5, 4,  NOTE_C5, 8,  NOTE_B4, 8,
    NOTE_A4, 4,  NOTE_A4, 8,  NOTE_C5, 8,  NOTE_E5, 4,  NOTE_D5, 8,  NOTE_C5, 8,
    NOTE_B4, -4, NOTE_C5, 8,  NOTE_D5, 4,  NOTE_E5, 4,
    NOTE_C5, 4,  NOTE_A4, 4,  NOTE_A4, 8,  NOTE_A4, 4,  NOTE_B4, 8,  NOTE_C5, 8,

    NOTE_D5, -4, NOTE_F5, 8,  NOTE_A5, 4,  NOTE_G5, 8,  NOTE_F5, 8,
    NOTE_E5, -4, NOTE_C5, 8,  NOTE_E5, 4,  NOTE_D5, 8,  NOTE_C5, 8,
    NOTE_B4, 4,  NOTE_B4, 8,  NOTE_C5, 8,  NOTE_D5, 4,  NOTE_E5, 4,
    NOTE_C5, 4,  NOTE_A4, 4,  NOTE_A4, 4,  PAUSE, 4,

    NOTE_E5, 2,  NOTE_C5, 2,
    NOTE_D5, 2,  NOTE_B4, 2,
    NOTE_C5, 2,  NOTE_A4, 2,
    NOTE_GS4, 2, NOTE_B4, 4,  PAUSE, 8,
    NOTE_E5, 2,  NOTE_C5, 2,
    NOTE_D5, 2,  NOTE_B4, 2,
    NOTE_C5, 4,  NOTE_E5, 4,  NOTE_A5, 2,
    NOTE_GS5, 2
};

// Número de notas na melodia
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// Função para tocar um tom no buzzer
void play_tone(int frequency, int duration) {
    if (frequency == PAUSE) {
        // Pausa (sem som)
        sleep_ms(duration);
    } else {
        // Configura o PWM para gerar o tom
        uint slice_num = pwm_gpio_to_slice_num(buzzer_pin);
        uint channel = pwm_gpio_to_channel(buzzer_pin);

        pwm_set_clkdiv(slice_num, 125); // Divisor de clock para frequência
        pwm_set_wrap(slice_num, 1000000 / frequency);
        pwm_set_chan_level(slice_num, channel, 1000000 / (2 * frequency));
        pwm_set_enabled(slice_num, true);

        // Mantém o tom pelo tempo especificado
        sleep_ms(duration);

        // Desliga o PWM
        pwm_set_enabled(slice_num, false);
    }
}

int main() {
    // Inicializa o SDK
    stdio_init_all();

    // Configura o pino do buzzer como saída PWM
    gpio_set_function(buzzer_pin, GPIO_FUNC_PWM);

    // Calcula a duração de uma semínima em milissegundos
    int wholenote = (60000 * 4) / tempo;

    // Reproduz a melodia
    for (int thisNote = 0; thisNote < notes * 2; thisNote += 2) {
        int noteDuration = wholenote / abs(melody[thisNote + 1]);
        if (melody[thisNote + 1] < 0) {
            // Nota pontuada
            noteDuration *= 1.5;
        }

        // Toca a nota
        play_tone(melody[thisNote], noteDuration);

        // Pequena pausa entre as notas
        sleep_ms(noteDuration * 0.1);
    }

    return 0;
}