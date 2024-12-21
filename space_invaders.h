#ifndef SPACE_INVADERS_H
#define SPACE_INVADERS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "disassembler_8080.c"

int disassembler_8080(unsigned char *code_buffer, int pc);

#endif
