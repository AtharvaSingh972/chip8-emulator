#include "display.h"
#include <SDL2/SDL.h>
#include <stdio.h>    

#define SCALE 10

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

int display_init(void){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
            printf("SDL initialization failed: %s\n", SDL_GetError());
            return 0;
    }

    window = SDL_CreateWindow("CHIP-8 EMULATOR", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 64 * SCALE, 32 * SCALE, SDL_WINDOW_SHOWN);

    if(window == NULL){
        printf("window creation failed: %s\n",SDL_GetError());
        SDL_Quit();
        return 0;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if(renderer == NULL){
        printf("renderer creation failed: %s\n",SDL_GetError());
        SDL_DestroyWindow(window);
        window = NULL;
        SDL_Quit();
        return 0;
    }

    return 1;    
    
}

void display_update(Chip8 *chip){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for(int y = 0; y < 32; y++){
        for(int x = 0; x < 64; x++){
            if(chip->display[y * 64 + x]){
                SDL_Rect pixel = {
                    x * SCALE,
                    y * SCALE,
                    SCALE,SCALE
                };

                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }
    SDL_RenderPresent(renderer);
}


void display_handle_events(Chip8 *chip, int *running){

    SDL_Event event;

    while(SDL_PollEvent(&event)){

        if(event.type == SDL_QUIT){
            *running = 0;
        }

        if(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP){

            int pressed = (event.type == SDL_KEYDOWN);
            SDL_Scancode key = event.key.keysym.scancode;

            int chip_key = -1;

            switch(key){

                case SDL_SCANCODE_1: chip_key = 0x1; break;
                case SDL_SCANCODE_2: chip_key = 0x2; break;
                case SDL_SCANCODE_3: chip_key = 0x3; break;
                case SDL_SCANCODE_4: chip_key = 0xC; break;

                case SDL_SCANCODE_Q: chip_key = 0x4; break;
                case SDL_SCANCODE_W: chip_key = 0x5; break;
                case SDL_SCANCODE_E: chip_key = 0x6; break;
                case SDL_SCANCODE_R: chip_key = 0xD; break;

                case SDL_SCANCODE_A: chip_key = 0x7; break;
                case SDL_SCANCODE_S: chip_key = 0x8; break;
                case SDL_SCANCODE_D: chip_key = 0x9; break;
                case SDL_SCANCODE_F: chip_key = 0xE; break;

                case SDL_SCANCODE_Z: chip_key = 0xA; break;
                case SDL_SCANCODE_X: chip_key = 0x0; break;
                case SDL_SCANCODE_C: chip_key = 0xB; break;
                case SDL_SCANCODE_V: chip_key = 0xF; break;
                default: break;
            }

            if(chip_key != -1){

                chip->keypad[chip_key] = pressed;

                if(pressed && chip->waiting_for_key){
                    chip->V[chip->key_register] = chip_key;
                    chip->waiting_for_key = 0;
                }
            }
        }
    }
}

void display_close(void){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}