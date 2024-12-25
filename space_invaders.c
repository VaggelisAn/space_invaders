

#include <stdio.h>
#include <stdlib.h>

#include "space_invaders.h"
#include "emulator_8080.c"
#include "disassembler_8080.c"
#include "state_handling.c"

int main (int argc, char**argv)    
   {   
    STATE_8080 state_8080;
    int mem_size;

    if (argc<2){
        printf("error: Missing filename argument.\n");
        return 1;
    }

    init_state_8080(&state_8080);
    mem_size = init_memory(&state_8080, argv[1]);
    if (mem_size < 0){
        printf("error: Memory initialization failed.\n");
        return 1;
    }

    while (state_8080.pc < mem_size){
        disassembler_8080(&state_8080);
        emulator_8080(&state_8080);
    }
    print_state(state_8080);
    return 0;    
}   

