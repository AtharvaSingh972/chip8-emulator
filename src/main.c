#include "chip8.h"
#include "display.h"
#include <SDL2/SDL.h>  
#include <stdint.h>    
#include <stdio.h>     

int main(int argc , char *argv[]){

    if(argc != 2){
            printf("Usage: %s <ROM file>\n", argv[0]);
            return 1;
        }

    Chip8 chip;
    chip8_init(&chip);

    if(!display_init()){
        return 1;
    }

    if(!load_rom(&chip, argv[1])){
        display_close();
        return 1;
    }


    int running = 1;

    uint32_t previous_time = SDL_GetTicks();

    float cpu_accumulator = 0;
    float timer_accumulator = 0;

    while(running){

        uint32_t current_time = SDL_GetTicks();
        uint32_t elapsed = current_time - previous_time;

        previous_time = current_time;


        display_handle_events(&chip, &running);

        // CPU execution
        cpu_accumulator += elapsed * 700.0f / 1000.0f;



        while(cpu_accumulator >= 1.0f){

            if(!chip.waiting_for_key){

                uint16_t instruction = chip8_fetch(&chip);

                decoder(&chip, instruction);
                
            }

            cpu_accumulator -= 1.0f;
        }

        // 60 Hz timers
        timer_accumulator += elapsed * 60.0f / 1000.0f;

        while(timer_accumulator >= 1.0f){

            if(chip.delay_timer > 0)
                chip.delay_timer--;

            if(chip.sound_timer > 0)
                chip.sound_timer--;

            timer_accumulator -= 1.0f;
        }

        display_update(&chip);

        SDL_Delay(1);
    }

    display_close();

    return 0;
}
