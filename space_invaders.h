#ifndef SPACE_INVADERS_H
#define SPACE_INVADERS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct CONDITION_CODES {    
    uint8_t    z:1;    
    uint8_t    s:1;    
    uint8_t    p:1;    
    uint8_t    cy:1;    
    //uint8_t    ac:1;    
    //uint8_t    pad:3;    
} CONDITION_CODES;    

typedef struct STATE_8080 {    
    uint8_t     a;    
    uint8_t     b;    
    uint8_t     c;    
    uint8_t     d;    
    uint8_t     e;    
    uint8_t     h;    
    uint8_t     l;    
    uint16_t    sp;    
    uint16_t    pc;
    uint8_t     *memory;    
    struct      CONDITION_CODES cc;    
    uint8_t     int_enable;    
} STATE_8080; 

void emulator_8080(STATE_8080 *state);
void disassembler_8080(STATE_8080 *state);
STATE_8080* init_state_8080(void);
int load_memory(STATE_8080 *state, char *filename);
void print_state(STATE_8080 state, int counter);
void export_mem(STATE_8080 state);

#define MEM_SIZE 0x10000 //16K
#define FILE_BUFF_SIZE 1024

#endif
