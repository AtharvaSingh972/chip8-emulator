#ifndef DISPLAY_H
#define DISPLAY_H

#include "chip8.h"

int display_init(void);
void display_update(Chip8 *chip);
void display_close(void);
void display_handle_events(Chip8 *chip, int *running);

#endif