#include "space_invaders.h"  

void print_state(STATE_8080 state_8080){
    printf("---\nregisters:\na=0x%02x\n", state_8080.a);
    printf("b=0x%02x c=0x%02x (bc=0x%04x)\n", state_8080.b, state_8080.c, (((state_8080.b<<8) | (state_8080.c))));
    printf("d=0x%02x e=0x%02x (de=0x%04x)\n", state_8080.d, state_8080.e, (((state_8080.d<<8) | (state_8080.e))));
    printf("h=0x%02x l=0x%02x (hl=0x%04x)\n", state_8080.h, state_8080.l, (((state_8080.h<<8) | (state_8080.l))));
    printf("sp=0x%02x\npc=0x%02x\n", state_8080.sp, state_8080.pc);
    printf("flags:\nz=%d s=%d p=%d cy=%d\n---\n", state_8080.cc.z, state_8080.cc.s, state_8080.cc.p, state_8080.cc.cy);
}

void init_state_8080(STATE_8080 *state_8080){
    state_8080->a = 0;
    state_8080->b = 0;
    state_8080->c = 0;
    state_8080->d = 0;
    state_8080->e = 0;
    state_8080->h = 0;
    state_8080->l = 0;
    state_8080->sp = 0;
    state_8080->pc = 0;
    state_8080->cc.z = 1;
    state_8080->cc.s = 0;
    state_8080->cc.cy = 0;
    state_8080->cc.p = 0;
}

int init_memory(STATE_8080 *state_8080, char *filename){
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

    state_8080->memory=malloc(fsize);    

    if (state_8080->memory == NULL)
    {    
        printf("error: Couldn't allocate memory\n");    
        return -1;    
    }    

    fread(state_8080->memory, fsize, 1, f);    
    fclose(f);

    return fsize;    
}

// TODO do
void set_register_pair(uint16_t *reg_pair, uint16_t value) {
    *reg_pair = value;
}