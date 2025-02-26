#include "buzzer.h"

#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

int time = 144;

int bad_romance[] = {
    NOTE_C4, 4, NOTE_D4, 8, NOTE_E4, 4, NOTE_C4, 8, NOTE_F4, 2,
    NOTE_E4, 8, NOTE_F4, 4, NOTE_E4, 8, NOTE_D4, 2,
    NOTE_B3, 4, NOTE_C4, 8, NOTE_D4, 4, NOTE_E4, 2, NOTE_E4, 8, NOTE_E4, 4, NOTE_D4, 8, NOTE_C4, 2,
    
    NOTE_C4, 4, NOTE_D4, 8, NOTE_E4, 4, NOTE_C4, 8, NOTE_F4, 2,
    NOTE_E4, 8, NOTE_F4, 4, NOTE_E4, 8, NOTE_D4, 2,
    NOTE_B3, 4, NOTE_C4, 8, NOTE_D4, 4, NOTE_E4, 2, NOTE_E4, 8, NOTE_E4, 4, NOTE_D4, 8, NOTE_C4, 2,
    
    NOTE_A3, 4, NOTE_A3, 8, NOTE_E4, 4, NOTE_E4, 8, NOTE_F4, 2, NOTE_E4, 8,
    NOTE_A3, 4, NOTE_A3, 8, NOTE_E4, 4, NOTE_E4, 8, NOTE_F4, 2, NOTE_E4, 8,
    NOTE_A3, 4, NOTE_A3, 8, NOTE_E4, 4, NOTE_E4, 8, NOTE_F4, 2, NOTE_E4, 8,
    NOTE_C4, 4, NOTE_C4, 8, NOTE_A3, 4, NOTE_C4, 8, NOTE_C4, 4, NOTE_C4, 8, NOTE_A3, 2,
};

int just_dance[] = {
    NOTE_G3, 4, NOTE_G3, 4, NOTE_A3, 4, NOTE_B3, 4, NOTE_C4, 4, NOTE_D4, 4, NOTE_C4, 4, NOTE_A3, 4, NOTE_G3, 4, NOTE_A3, 4,
    NOTE_G3, 4, NOTE_G3, 4, NOTE_A3, 4, NOTE_B3, 4, NOTE_C4, 4, NOTE_D4, 4, NOTE_C4, 4, NOTE_A3, 4, NOTE_G3, 4, NOTE_A3, 4,
    NOTE_D4, 4, NOTE_D4, 4, NOTE_C4, 4, NOTE_C4, 4, NOTE_B3, 4, NOTE_G3, 4,
    NOTE_D4, 4, NOTE_D4, 4, NOTE_C4, 4, NOTE_C4, 4, NOTE_B3, 4, NOTE_G3, 4,
    NOTE_G3, 4, NOTE_G3, 4, NOTE_A3, 4, NOTE_B3, 4, NOTE_C4, 4, NOTE_D4, 4, NOTE_C4, 4, NOTE_A3, 4, NOTE_G3, 4, NOTE_A3, 4,
    NOTE_D4, 4, NOTE_C4, 4, NOTE_B3, 4, NOTE_G3, 4, NOTE_A3, 4, NOTE_G3, 4, NOTE_C4, 4,
    NOTE_G3, 4, NOTE_G3, 4, NOTE_G3, 4, NOTE_B3, 4, NOTE_B3, 4, NOTE_A3, 4, NOTE_B3, 4, NOTE_B3, 4, NOTE_B3, 4, NOTE_G3, 4, NOTE_G3, 4, NOTE_A3, 4, NOTE_G3, 4, NOTE_C4, 4,
    NOTE_D4, 4, NOTE_C4, 4, NOTE_B3, 4, NOTE_G3, 4, NOTE_A3, 4, NOTE_G3, 4, NOTE_C4, 4,
    NOTE_G3, 4, NOTE_G3, 4, NOTE_G3, 4, NOTE_B3, 4, NOTE_B3, 4, NOTE_B3, 4, NOTE_B3, 4, NOTE_B3, 4,

    // Refrão
    NOTE_A3, 4, NOTE_G3, 4,
    NOTE_G3, 4, NOTE_G3, 4, NOTE_G3, 4, NOTE_G3, 4, NOTE_G3, 4,
    NOTE_FS3, 4, NOTE_B3, 4, NOTE_B3, 4, NOTE_G3, 4,
    NOTE_A3, 4, NOTE_G3, 4,
    NOTE_G3, 4, NOTE_G3, 4, NOTE_B3, 4, NOTE_B3, 4, NOTE_G3, 4,
    NOTE_D4, 4, NOTE_D4, 4, NOTE_C4, 4, NOTE_C4, 4, NOTE_B3, 4,
    NOTE_B3, 4, NOTE_G3, 4,
    NOTE_G3, 4, NOTE_G3, 4, NOTE_G3, 4, NOTE_G3, 4, NOTE_G3, 4,
    NOTE_FS3, 4, NOTE_FS3, 4, NOTE_FS3, 4, NOTE_G3, 4, NOTE_FS3, 4, NOTE_G3, 4
};

int bloody_mary[] = {
    NOTE_F3, 8, NOTE_D3, 4, NOTE_D3, 4, NOTE_D3, 4,  // fa re re re
    NOTE_E3, 8, NOTE_F3, 8, NOTE_D3, 4, NOTE_D3, 4, NOTE_D3, 4, // mi fa re re re
    NOTE_D3, 4, NOTE_E3, 4, NOTE_F3, 4, NOTE_D3, 4, NOTE_D3, 4, NOTE_D3, 4, // re mi fa re re re
    NOTE_F3, 4, NOTE_A3, 4, NOTE_AS3, 4, NOTE_A3, 4, NOTE_G3, 4, NOTE_A3, 4, NOTE_G3, 4, // fa la la# la sol la sol

    NOTE_D3, 2, NOTE_D3, 4, NOTE_D3, 4, // re re re
    NOTE_E3, 4, NOTE_F3, 2, NOTE_D3, 4, NOTE_D3, 4, NOTE_D3, 2, // mi fa re re re
    NOTE_D3, 4, NOTE_E3, 4, NOTE_F3, 2,  // re mi fa
    NOTE_E3, 4, NOTE_D3, 4, NOTE_CS3, 2, NOTE_D3, 2, // MI RE DO# RE
    NOTE_E3, 4, NOTE_D3, 4, NOTE_CS3, 2, NOTE_D3, 2, // MI RE DO# RE
    NOTE_E3, 4, NOTE_D3, 4, NOTE_CS3, 2, NOTE_D3, 2, // MI RE DO# RE
    NOTE_E3, 4, NOTE_D3, 4, NOTE_CS3, 2, // MI RE DO#

    // Segunda parte
    NOTE_AS3, 2, NOTE_AS3, 2, NOTE_AS3, 4, NOTE_C4, 2, NOTE_A3, 4,  // la# la# la# DO la
    NOTE_AS3, 4, NOTE_A3, 4, NOTE_G3, 2, NOTE_F3, 4, NOTE_G3, 4, // la# la sol fa sol
    NOTE_E3, 4, NOTE_E3, 4, NOTE_G3, 2, NOTE_A3, 4, NOTE_G3, 4, NOTE_F3, 2, NOTE_D3, 2, // mi mi sol la sol fa re

    NOTE_AS3, 2, NOTE_AS3, 2, NOTE_AS3, 4, NOTE_C4, 2, NOTE_A3, 4,  // la# la# la# DO la
    NOTE_AS3, 4, NOTE_A3, 4, NOTE_G3, 2, NOTE_F3, 4, NOTE_G3, 4, // la# la sol fa sol
    NOTE_E3, 4, NOTE_E3, 4, NOTE_G3, 2, NOTE_A3, 4, NOTE_G3, 4, NOTE_F3, 2, NOTE_D3, 2 // mi mi sol la sol fa re
};

int poker_face[] = {
    NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8,
    NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8,
    NOTE_GS4, 4, NOTE_DS4, 8, NOTE_D4, 8, NOTE_DS4, 8, NOTE_E4, 8, NOTE_DS4, 8, NOTE_D4, 8, NOTE_DS4, 8,
    NOTE_E4, 8, NOTE_DS4, 8, NOTE_CS4, 8, NOTE_B3, 8, NOTE_AS3, 8,
    NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8,
    NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8,
    NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8,
    
    NOTE_B4, 4, NOTE_AS4, 8, NOTE_FS4, 8, NOTE_B4, 8, NOTE_AS4, 8, NOTE_FS4, 8,
    NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_DS4, 8, NOTE_E4, 8, NOTE_DS4, 8, NOTE_FS4, 8, NOTE_FS4, 8,
    
    NOTE_B4, 4, NOTE_AS4, 8, NOTE_FS4, 8, NOTE_B4, 8, NOTE_AS4, 8, NOTE_FS4, 8,
    NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_DS4, 8, NOTE_E4, 8, NOTE_DS4, 8, NOTE_FS4, 8, NOTE_FS4, 8,
    
    NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8,
    NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8,
    NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8,
    
    NOTE_GS4, 4, NOTE_DS4, 8, NOTE_D4, 8, NOTE_DS4, 8, NOTE_E4, 8, NOTE_DS4, 8, NOTE_D4, 8, NOTE_DS4, 8,
    NOTE_E4, 8, NOTE_DS4, 8, NOTE_CS4, 8, NOTE_B3, 8, NOTE_AS3, 8,
    NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8,
    NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8,
    NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_B4, 8
};

int paparazzi[] = {
    NOTE_F4, 4, NOTE_D4, 4, 
    NOTE_F4, 8, NOTE_D4, 8, NOTE_D4, 4, 
    NOTE_G4, 4, NOTE_A4, 4, NOTE_G4, 4, NOTE_D4, 4, 
    NOTE_F4, 4, NOTE_D4, 8, NOTE_D4, 8, 
    NOTE_F4, 4, NOTE_D4, 8, NOTE_D4, 8, 
    NOTE_C4, 4, NOTE_D4, 4, 

    NOTE_F4, 4, NOTE_D4, 8, NOTE_D4, 8, 
    NOTE_G4, 4, NOTE_A4, 4, NOTE_G4, 4, NOTE_D4, 4, 
    NOTE_F4, 4, NOTE_D4, 8, NOTE_D4, 8, 
    NOTE_F4, 4, NOTE_D4, 4, NOTE_C4, 4, 
    NOTE_D4, 4, NOTE_D4, 4, 

    NOTE_D4, 8, NOTE_D4, 8, NOTE_D4, 8, NOTE_D4, 8, 
    NOTE_C4, 4, NOTE_D4, 4, NOTE_F4, 4, NOTE_D4, 4, 
    NOTE_D4, 8, NOTE_D4, 8, NOTE_D4, 8, NOTE_D4, 8, 
    NOTE_C4, 4, NOTE_D4, 4, NOTE_A4, 4, NOTE_F4, 4, 

    NOTE_A4, 4, NOTE_G4, 4, NOTE_A4, 4, NOTE_G4, 4, 
    NOTE_A4, 8, NOTE_G4, 8, NOTE_A4, 8, 
    NOTE_G4, 8, NOTE_B4, 4, NOTE_A4, 8, NOTE_G4, 8, 
    NOTE_A4, 8, NOTE_G4, 8, NOTE_B4, 4, NOTE_A4, 8, NOTE_G4, 8, 

    NOTE_G4, 4, NOTE_G4, 4, NOTE_B4, 4, 
    NOTE_G4, 4, NOTE_B4, 4, 
    NOTE_G4, 8, NOTE_A4, 8, NOTE_A4, 8, NOTE_B4, 8, NOTE_C5, 4, 
    NOTE_F4, 4, NOTE_A4, 4, 
    NOTE_D4, 4, NOTE_A4, 4, NOTE_G4, 4, 

    NOTE_G4, 4, NOTE_G4, 4, NOTE_B4, 4, 
    NOTE_G4, 4, NOTE_G4, 4, NOTE_B4, 4, 
    NOTE_G4, 8, NOTE_G4, 8, 
    NOTE_A4, 8, NOTE_A4, 8, NOTE_B4, 8, NOTE_C5, 8, 
    NOTE_F4, 4, NOTE_A4, 4, 
    NOTE_D4, 4, NOTE_A4, 4, NOTE_G4, 4, 

    NOTE_G4, 8, NOTE_G4, 8, NOTE_G4, 8, 
    NOTE_C5, 4, NOTE_B4, 4, NOTE_A4, 4, 
    NOTE_F4, 4, NOTE_C5, 4, 
    NOTE_C5, 8, NOTE_D5, 8, NOTE_B4, 8, NOTE_G4, 8
};


void play_tone(int frequency, int duration) {
    if (frequency == PAUSE) {
        // Pausa (sem som)
        sleep_ms(duration);
    } else {
        // Configura o PWM para gerar o tom
        uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
        uint channel = pwm_gpio_to_channel(BUZZER_PIN);

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

int get_notes(int melody[]) {
    int size = 0;
    while(melody[size] != -1) {
        ++size;
    }
    return size/2;
}

void play_song(int melody[]) {
    int notes = get_notes(melody);
    int wholenote = (60000 * 4) / time;

    for (int thisNote = 0; thisNote < notes * 2; thisNote += 2) {
        int noteDuration = wholenote / abs(melody[thisNote + 1]);
        if(melody[thisNote + 1] < 0) {
            noteDuration *= 1.5;
        }
        play_tone(melody[thisNote], noteDuration);
        sleep_ms(noteDuration * 0.1);
    }
}
