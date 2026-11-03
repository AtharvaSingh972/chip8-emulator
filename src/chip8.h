#include <stdint.h>
#ifndef CHIP8_H
#define CHIP8_H


typedef struct{
    uint8_t memory[4096];
    uint16_t pc;
    uint16_t I;
    uint8_t V[16];
    uint8_t sp;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint16_t stack[16];
    uint8_t display[64 * 32];
    uint8_t keypad[16];
    uint8_t waiting_for_key;
    uint8_t key_register;   
    
}Chip8;

void chip8_init(Chip8 *chip);
uint16_t chip8_fetch(Chip8 *chip);
void decoder(Chip8 *chip, uint16_t instruction);
int load_rom(Chip8 *chip, const char *filename);


#endif
