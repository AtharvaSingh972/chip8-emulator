#include "chip8.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




static const uint8_t fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void chip8_init(Chip8 *chip){
    *chip = (Chip8){0} ;
    chip->pc = 0x200;    //Interpretor occupies the lower 512 bytes

    for(int i = 0; i < 80; i++){
        chip->memory[i] = fontset[i];
    }

}

uint16_t chip8_fetch(Chip8 *chip){

    if(chip->pc >= sizeof(chip->memory) - 1){
        printf("Memory access violation during fetch at PC: %04X\n",chip->pc);
        exit(1);
    }
    uint8_t upper_byte = chip->memory[chip->pc];
    uint8_t lower_byte = chip->memory[chip->pc+1];

    uint16_t instruction = (upper_byte << 8) | lower_byte;

    chip->pc += 2;
    return instruction;
}

void decoder(Chip8 *chip, uint16_t instruction){

    uint8_t opcode = (instruction >> 12) & 0x0F;
    uint8_t regX = (instruction & 0x0F00) >> 8;
    uint8_t NN = (instruction & 0x00FF);
    uint8_t regY = (instruction & 0x00F0) >> 4;
    uint8_t lastnibble = (instruction & 0x000F);
    uint16_t NNN = instruction & 0x0FFF;

    switch(opcode){
        default:
            printf("Unknown opcode: %04X at PC: %04X\n", instruction, chip->pc - 2);
            exit(1);

        case 0x0:
            switch(NN){
                case 0xE0:
                    for(int i = 0; i < 64 * 32; i++){
                        chip->display[i] = 0;
                    }

                    break;

                case 0xEE:
                    if(chip->sp == 0){
                        printf("Stack underflow at PC: %04X\n", chip->pc - 2);
                        exit(1);
                    }

                    chip->sp--;
                    if(chip->stack[chip->sp] > sizeof(chip->memory) - 2){
                        printf("Invalid return address at PC: %04X\n", chip->pc - 2);
                        exit(1);
                    }
                    chip->pc = chip->stack[chip->sp];
                    break;

                default:
                    printf("Unknown 0x0 instruction: %04X at PC: %04X\n",
                           instruction, chip->pc - 2);
                    exit(1);
            }
            break;

        case 0x1:
            chip->pc = NNN;
            break;

        case 0x2:

            if(chip->sp >= 16){
                printf("Stack overflow at PC: %04X\n", chip->pc - 2);
                exit(1);
            }

            chip->stack[chip->sp] = chip->pc;
            chip->sp++;
            chip->pc = NNN;

            break;

        case 0x3:
            if(chip->V[regX] == NN){
                chip->pc += 2;
            }
            break;

        case 0x4:
            if(chip->V[regX] != NN){
                chip->pc += 2;
            }
            break;

        case 0x5:
            if(lastnibble != 0){
                printf("Invalid 5XYN instruction: %04X at PC: %04X\n",
                       instruction, chip->pc - 2);
                exit(1);
            }

            if(chip->V[regX] == chip->V[regY]){
                chip->pc += 2;
            }
            break;

        case 0x6:
            chip->V[regX] = NN;
            break;

        case 0x7:
            chip->V[regX] += NN;
            break;

        case 0x8:
            switch(lastnibble){

                case 0x0:
                    chip->V[regX] = chip->V[regY];
                    break;

                case 0x1:
                    chip->V[regX] |= chip->V[regY];
                    break;

                case 0x2:
                    chip->V[regX] &= chip->V[regY];
                    break;

                case 0x3:
                    chip->V[regX] ^= chip->V[regY];
                    break;

                case 0x4:{
                    uint16_t sum = chip->V[regX] + chip->V[regY];

                    chip->V[regX] = sum & 0xFF;
                    chip->V[0xF] = (sum > 0xFF);

                    break;}

                case 0x5:
                    chip->V[0xF] = (chip->V[regX] >= chip->V[regY]);
                    chip->V[regX] -= chip->V[regY];

                    break;

                case 0x6:
                    chip->V[0xF] = chip->V[regX] & 0x01;
                    chip->V[regX] >>= 1;

                    break;

                case 0x7:
                    chip->V[0xF] = (chip->V[regY] >= chip->V[regX]);
                    chip->V[regX] = chip->V[regY] - chip->V[regX];

                    break;

                case 0xE:
                    chip->V[0xF] = (chip->V[regX] & 0x80) >> 7;
                    chip->V[regX] <<= 1;

                    break;
                default:
                    printf("Unknown 8XYN instruction: %04X at PC: %04X\n",
                           instruction, chip->pc - 2);
                    exit(1);
            }
            break;

        case 0x9:
            if(lastnibble != 0){
                printf("Invalid 9XYN instruction: %04X at PC: %04X\n",
                       instruction, chip->pc - 2);
                exit(1);
            }

            if(chip->V[regX] != chip->V[regY]){
                chip->pc += 2;
            }
            break;

        case 0xA:
            chip->I = NNN;
            break;

        case 0xB:
            chip->pc = NNN + chip->V[0];
            break;

        case 0xC:
            chip->V[regX] = (rand() % 256) & NN;
            break;

        case 0xD:{
            uint8_t x = chip->V[regX] % 64;
            uint8_t y = chip->V[regY] % 32;

            chip->V[0xF] = 0;

            if(chip->I + lastnibble > sizeof(chip->memory)){
                printf("Memory access violation during sprite drawing at PC: %04X\n",
                       chip->pc - 2);
                exit(1);
            }

            for(int row = 0; row < lastnibble; row++){
                uint8_t sprite = chip->memory[chip->I + row];

                for(int col = 0; col < 8; col++){
                    if(sprite & (0x80 >> col)){
                        int px = (x + col) % 64;
                        int py = (y + row) % 32;
                        int index = py * 64 + px;

                        if(chip->display[index]){
                            chip->V[0xF] = 1;
                        }

                        chip->display[index] ^= 1;
                    }
                }
            }

            break;}

        case 0xE:
            switch(NN){
                case 0x9E:
                    if(chip->V[regX] >= 16){
                        printf("Invalid keypad index in EX9E at PC: %04X\n",
                               chip->pc - 2);
                        exit(1);
                    }

                    if(chip->keypad[chip->V[regX]]){
                        chip->pc += 2;
                    }
                    break;

                case 0xA1:
                    if(chip->V[regX] >= 16){
                        printf("Invalid keypad index in EXA1 at PC: %04X\n",
                               chip->pc - 2);
                        exit(1);
                    }

                    if(!chip->keypad[chip->V[regX]]){
                        chip->pc += 2;
                    }
                    break;
            }
            break;

        case 0xF:
            switch(NN){
                case 0x07:
                  chip->V[regX] = chip->delay_timer;
                    break;

                case 0x0A:
                    chip->waiting_for_key = 1;
                    chip->key_register = regX;

                    break;

                case 0x15:
                    chip->delay_timer = chip->V[regX];
                    break;

                case 0x18:
                    chip->sound_timer = chip->V[regX];
                    break;

                case 0x1E:
                    chip->I += chip->V[regX];
                    break;

                case 0x29:
                    if(chip->V[regX] > 0xF){
                        printf("Invalid font character at PC: %04X\n",
                               chip->pc - 2);
                        exit(1);
                    }

                    chip->I = chip->V[regX] * 5;
                    break;

                case 0x33:
                    if(chip->I > sizeof(chip->memory) - 3){
                        printf("Memory access violation during BCD storage at PC: %04X\n",
                               chip->pc - 2);
                        exit(1);
                    }

                    chip->memory[chip->I] = chip->V[regX] / 100;
                    chip->memory[chip->I + 1] = (chip->V[regX] / 10) % 10;
                    chip->memory[chip->I + 2] = chip->V[regX] % 10;

                    break;

                case 0x55:
                    if(chip->I + regX >= sizeof(chip->memory)){
                        printf("Memory access violation during FX55 at PC: %04X\n",
                               chip->pc - 2);
                        exit(1);
                    }

                    for(int i = 0; i <= regX; i++){
                        chip->memory[chip->I + i] = chip->V[i];
                    }
                    break;

                case 0x65:
                    if(chip->I + regX >= sizeof(chip->memory)){
                        printf("Memory access violation during FX65 at PC: %04X\n",
                               chip->pc - 2);
                        exit(1);
                    }

                    for(int i = 0; i <= regX; i++){
                        chip->V[i] = chip->memory[chip->I + i];
                    }
                    break;
            }
            break;
    }
}

int load_rom(Chip8 *chip, const char *filename){

    FILE *file = fopen(filename, "rb");

    if(file == NULL){
        printf("Could not open ROM: %s\n", filename);
        return 0;
    }

    size_t capacity = sizeof(chip->memory) - 0x200;

    // Clear previous ROM contents
    memset(chip->memory + 0x200, 0, capacity);

    size_t bytes_read = fread(chip->memory + 0x200, 1, capacity, file);

    if(ferror(file)){
        printf("Error reading ROM file\n");
        fclose(file);
        return 0;
    }

    // Check whether the ROM exceeds available memory
    if(bytes_read == capacity){
        int extra = fgetc(file);

        if(extra != EOF){
            printf("ROM is too large\n");
            fclose(file);
            return 0;
        }

        if(ferror(file)){
            printf("Error reading ROM file\n");
            fclose(file);
            return 0;
        }
    }

    if(bytes_read == 0){
        printf("ROM file is empty\n");
        fclose(file);
        return 0;
    }

    fclose(file);

    printf("ROM loaded successfully: %zu bytes\n", bytes_read);

    return 1;
}
