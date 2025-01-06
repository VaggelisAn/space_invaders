/*
*   Emulator for the videogame Space Invaders, written in C.
*   Vaggelis Ananiadis 20/12/24
*
*/

#include <stdio.h>
#include <stdlib.h>

#include "space_invaders.h"
#include "emulator_8080.c"
#include "disassembler_8080.c"
#include "state_handling.c"

int main (int argc, char**argv)    
   {   
    STATE_8080 *state;
    int mem_size;

    if (argc<2){
        printf("error: Missing filename argument.\n");
        return 1;
    }

    state = init_state_8080();
    mem_size = load_memory(state, argv[1]);
    if (mem_size < 0){
        printf("error: Memory initialization failed.\n");
        return 1;
    }

    int counter = 0;
    while (state->pc < mem_size){
        disassembler_8080(state);

        uint8_t *op = &state->memory[state->pc];    

        //machine specific handling for IN   
        if (*op == 0xdb){
            uint8_t port = op[1];    
            state->a = machine_IN(state, port);    
            state->pc++;    
        }    
        //machine specific handling for OUT   
        else if (*op == 0xd3){
            uint8_t port = op[1];    
            machine_OUT(state, port);    
            state->pc++;    
        }    
        else    
            emulator_8080(state);

    }
    print_state(*state, counter);
    export_mem(*state);

    return 0;    
}   

/*
        // if (counter < 1751){
        //     counter++;
        //     continue;
        // }
        // else{
        //     print_state(*state, counter);
        //     export_mem(*state);
        //     getchar();
        //     counter++;
        // }
        // print_state(*state, counter);
        // export_mem(*state);
        // getchar();
        // counter++;
*/

