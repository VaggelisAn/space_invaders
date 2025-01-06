#include "space_invaders.h"  

void print_state(STATE_8080 state, int counter){
    printf("counter=%d- - -\nregisters:\na=0x%02x\n", counter, state.a);
    printf("b=0x%02x c=0x%02x (bc=0x%04x)\n", state.b, state.c, (((state.b<<8) | (state.c))));
    printf("d=0x%02x e=0x%02x (de=0x%04x)\n", state.d, state.e, (((state.d<<8) | (state.e))));
    printf("h=0x%02x l=0x%02x (hl=0x%04x)\n", state.h, state.l, (((state.h<<8) | (state.l))));
    printf("sp=0x%02x\npc=0x%02x\n", state.sp, state.pc);
    printf("---flags:\nz=%d s=%d p=%d cy=%d\n- - - - -\n\n", state.cc.z, state.cc.s, state.cc.p, state.cc.cy);
}


STATE_8080* init_state_8080(void)
{
	STATE_8080* state = calloc(1, sizeof(state));
	state->memory = calloc(0x10000, sizeof(uint8_t));  
    if (state->memory == NULL)
    {    
        printf("error: Couldn't allocate memory\n");    
        exit(1);    
    }    
    state->a = 0;
    state->b = 0;
    state->c = 0;
    state->d = 0;
    state->e = 0;
    state->h = 0;
    state->l = 0;
    state->sp = 0;
    state->pc = 0;
    state->cc.z = 0;
    state->cc.s = 0;
    state->cc.cy = 0;
    state->cc.p = 0;

	return state;
}

int load_memory(STATE_8080 *state, char *filename){
    FILE *f= fopen(filename, "rb");    
    if (f==NULL)    
    {    
        printf("error: Couldn't open %s\n", filename);    
        return -1;    
    }    

    //Get the file size and read it into a memory buffer    
    fseek(f, 0L, SEEK_END);    
    int fsize = ftell(f);    
    fseek(f, 0L, SEEK_SET); 

    uint8_t *buffer;
    printf("fsize=%d\n", fsize);
    for (int i=0; i < fsize/FILE_BUFF_SIZE; i++){
        buffer = &state->memory[i*FILE_BUFF_SIZE];
        fread(buffer, FILE_BUFF_SIZE, 1, f);    
    }
    fclose(f);

    return fsize;    
}

void export_mem(STATE_8080 state){
    const int COLUMNS = 16;
    int row, col;

    FILE *f_out;
    f_out = fopen("mem_out", "wb");

    for (row=0; row<MEM_SIZE/COLUMNS; row++){
        for (col=0; col<COLUMNS; col++)
            fwrite(&(state.memory[row*COLUMNS + col]), sizeof(uint8_t), 1, f_out);
    }

    // fprintf(f_out, "     ");
    // for (int col=0; col<COLUMNS; col++)
    //     fprintf(f_out, "%02x ", col);
    // fprintf(f_out, "\n");
    // for (row=0; row<MEM_SIZE/COLUMNS; row++){
    //     fprintf(f_out, "%03x: ", row);
    //     for (col=0; col<COLUMNS; col++)
    //         fprintf(f_out, "%02x ", state.memory[row*COLUMNS + col]);
    //     fprintf(f_out, "\n");
    // }

    fclose(f_out);
}

// TODO do
void set_register_pair(uint16_t *reg_pair, uint16_t value) {
    *reg_pair = value;
}