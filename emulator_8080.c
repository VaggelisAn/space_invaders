#include "space_invaders.h"

int get_parity(int num) {
    int parity = 0;
    while (num) {
        parity ^= (num & 1); 
        num >>= 1;          
    }

    // Return 1 if even parity, 0 if odd parity
    return (parity == 0);
}

void machine_OUT(STATE_8080 *state, uint8_t port){
    switch (port)
    {
        case 2:
        {
            break;
        }
        case 4:
        {
            break;
        }
        default:
            break;
        }
}

uint8_t machine_IN(STATE_8080 *state, uint8_t port){
    switch (port)
    {
        case 3:    
           {    
            break;
           }    
        default:
            break;
        }
    return 0;
}

void emulator_8080(STATE_8080 *state){    
    uint8_t *op = &state->memory[state->pc];    

//  Flags when the accumulator (reg a) is 0:
    CONDITION_CODES condition_zero;
    condition_zero.z = 0;    
    condition_zero.s = 0;    
    condition_zero.cy = 0;    
    condition_zero.p = 0;    

    switch (*op){
        // NOP
		case 0x00:
		case 0x08:
		case 0x10:
		case 0x18:
        case 0x20:
		case 0x28:
        case 0x30:
		case 0x38:
		case 0xcb:
		case 0xd9:
		case 0xdd:
		case 0xed:
		case 0xfd:
			break;
        case 0x01: //LXI B, addr
        {
            uint8_t low_byte = state->memory[state->pc + 1];
            uint8_t high_byte = state->memory[state->pc + 2];
            state->b = high_byte;
            state->c = low_byte;
            state->pc += 2;
            break;
        }
        case 0x05: //DCR B
        {
            uint16_t answer = (uint16_t) state->b - 1;    
            state->cc.z = ((answer & 0xff) == 0);    
            state->cc.s = ((answer & 0x80) != 0);    
            state->cc.cy = (answer > 0xff);    
            state->cc.p = get_parity(answer&0xff);    
            state->b = answer & 0xff;
            break;    
        }
        case 0x06: //MVI B, addr
        {
            uint8_t low_byte = state->memory[state->pc + 1];
            state->b = low_byte;
            state->pc += 1;
            break;
        }
        case 0x09: //DAD D
        {
            state->h += state->b;
            state->l += state->c;
            uint16_t answer = (state->h<<8) | (state->l);  

            state->cc.cy = (answer > 0xff);    
            break;
        }
        case 0x0d: //DCR D
        {
            uint16_t answer = (uint16_t) state->d - 1;    
            state->cc.z = ((answer & 0xff) == 0);    
            state->cc.s = ((answer & 0x80) != 0);    
            state->cc.cy = (answer > 0xff);    
            state->cc.p = get_parity(answer&0xff);    
            state->d = answer & 0xff;
            break;    
        }
        case 0x0e: //MVI C, addr
        {
            uint8_t low_byte = state->memory[state->pc + 1];
            state->c = low_byte;
            state->pc += 1;
            break;
        }
        case 0x0f: //RRC 
        {
            uint8_t prev_a = state->a;
            // rotates lsb to msb and right shifts a by one.    
            state->a = ((prev_a & 1) << 7) | (prev_a >> 1);    
            state->cc.cy = (1 == (prev_a & 1)); 
            break;   
        }
        case 0x11: //LXI D, addr
        {
            uint8_t low_byte = state->memory[state->pc + 1];
            uint8_t high_byte = state->memory[state->pc + 2];
            state->d = high_byte;
            state->e = low_byte;
            state->pc += 2;
            break;
        }
        case 0x13: //INX D
        {
            uint16_t answer = ((state->d<<8) | (state->e)) + 1;
            state->e = (uint8_t) answer & 0xFF;
            state->d = (uint8_t) (answer>>8) & 0xFF;
            break;
        }
        case 0x19: //DAD D
        {
            state->h += state->d;
            state->l += state->e;
            uint16_t answer = (state->h<<8) | (state->l);  

            state->cc.cy = (answer > 0xff);    
            break;
        }
        case 0x1a: //LDAX D
        {
            uint16_t addr = (state->d<<8) | (state->e);  
            state->a = state->memory[addr];
            break;
        }
        case 0x21: //LXI H, addr
        {
            uint8_t low_byte = state->memory[state->pc + 1];
            uint8_t high_byte = state->memory[state->pc + 2];
            state->h = high_byte;
            state->l = low_byte;
            state->pc += 2;
            break;
        }
        case 0x23: //INX H
        {
            uint16_t answer = ((state->h<<8) | (state->l)) + 1;
            state->l = (uint8_t) answer & 0xFF;
            state->h = (uint8_t) (answer>>8) & 0xFF;
            break;
        }
        case 0x26: //MVI H, addr
        {
            uint16_t addr = (state->h<<8) | (state->l);  
            uint8_t low_byte = state->memory[state->pc + 1];  
            state->memory[addr] = low_byte;
            state->pc += 1;
            break;
        }
        case 0x29: //DAD H  // TODO easy optimization
        {
            state->h += state->h;
            state->l += state->l;
            uint16_t answer = (state->h<<8) | (state->l);  

            state->cc.cy = (answer > 0xff);    
            break;
        }
        case 0x31: //LXI SP, addr
        {
            uint8_t low_byte = state->memory[state->pc + 1];
            uint8_t high_byte = state->memory[state->pc + 2];
            state->sp = (high_byte << 8) | low_byte;
            state->pc += 2;
            break;
        }
        case 0x32: //STA addr
        {
            uint16_t addr = (state->memory[(state->pc + 2)]<<8) | (state->pc + 1);
            state->memory[addr] = state->a;
            state->pc += 2;
            break;
        }
        case 0x36: //MVI M, addr
        {
            uint16_t addr = (state->h<<8) | (state->l);  
            uint8_t low_byte = state->memory[state->pc + 1]; 
            state->memory[addr] = low_byte;
            state->pc += 1;
            break;
        }
        case 0x3a: //LDA addr TODO fix
        {
            uint16_t addr = (state->memory[(state->pc + 2)]<<8) | (state->pc + 1);
            state->a = state->memory[addr];
            state->pc += 2;
            break;
        }
        case 0x3e: //MVI A, addr
        {
            uint8_t low_byte = state->memory[state->pc + 1];
            state->a = low_byte;
            state->pc += 1;
            break;
        }
        case 0x56: //MOV D,M
        {
            uint16_t addr = (state->h<<8) | (state->l);
            state->d = state->memory[addr];
            break; 
        }
        case 0x5e: //MOV E,M
        {
            uint16_t addr = (state->h<<8) | (state->l);
            state->e = state->memory[addr];
            break; 
        }
        case 0x66: //MOV H,M
        {
            uint16_t addr = (state->h<<8) | (state->l);
            state->h = state->memory[addr];
            break; 
        }
        case 0x6f: //MOV L,A
        {
            state->l = state->a;
            break;
        }
        case 0x77: //MOV M,A
        {
            uint16_t addr = (state->h<<8) | (state->l);
            state->memory[addr] = state->a;
            break; 
        }
        case 0x7a: //MOV A,D
        {
            state->a = state->d;
            break;
        }
        case 0x7b: //MOV A,E
        {
            state->a = state->e;
            break;
        }
        case 0x7c: //MOV A,E
        {
            state->a = state->h;
            break;
        }
        case 0x7e: //MOV A,M
        {
            uint16_t addr = (state->h<<8) | (state->l);
            state->a = state->memory[addr];
            break;
        }
        case 0x80: //ADD B
        {    
            uint16_t answer = (uint16_t) state->a + (uint16_t) state->b;    
            state->cc.z = ((answer & 0xff) == 0);    
            state->cc.s = ((answer & 0x80) != 0);    
            state->cc.cy = (answer > 0xff);    
            state->cc.p = get_parity(answer&0xff);    
            state->a = answer & 0xff;
            break;    
        }                 
        case 0x81: //ADD C    
        {    
            uint16_t answer = (uint16_t) state->a + (uint16_t) state->c;    
            state->cc.z = ((answer & 0xff) == 0);    
            state->cc.s = ((answer & 0x80) != 0);    
            state->cc.cy = (answer > 0xff);    
            state->cc.p = get_parity(answer&0xff);    
            state->a = answer & 0xff;
            break;    
        }            
        case 0x82: //ADD D
        {    
            uint16_t answer = (uint16_t) state->a + (uint16_t) state->d;    
            state->cc.z = ((answer & 0xff) == 0);    
            state->cc.s = ((answer & 0x80) != 0);    
            state->cc.cy = (answer > 0xff);    
            state->cc.p = get_parity(answer&0xff);    
            state->a = answer & 0xff;
            break;    
        }         
        case 0x83: //ADD E
        {    
            uint16_t answer = (uint16_t) state->a + (uint16_t) state->e;    
            state->cc.z = ((answer & 0xff) == 0);    
            state->cc.s = ((answer & 0x80) != 0);    
            state->cc.cy = (answer > 0xff);    
            state->cc.p = get_parity(answer&0xff);    
            state->a = answer & 0xff;
            break;    
        }         
        case 0x84: //ADD H
        {    
            uint16_t answer = (uint16_t) state->a + (uint16_t) state->h;    
            state->cc.z = ((answer & 0xff) == 0);    
            state->cc.s = ((answer & 0x80) != 0);    
            state->cc.cy = (answer > 0xff);    
            state->cc.p = get_parity(answer&0xff);    
            state->a = answer & 0xff;
            break;    
        }                 
        case 0x85: //ADD L
        {    
            uint16_t answer = (uint16_t) state->a + (uint16_t) state->l;    
            state->cc.z = ((answer & 0xff) == 0);    
            state->cc.s = ((answer & 0x80) != 0);    
            state->cc.cy = (answer > 0xff);    
            state->cc.p = get_parity(answer&0xff);    
            state->a = answer & 0xff;
            break;    
        }                 
        case 0x86: //ADD M    
        {    
            uint16_t addr = (state->h<<8) | (state->l);    
            uint16_t answer = (uint16_t) state->a + state->memory[addr];    
            state->cc.z = ((answer & 0xff) == 0);    
            state->cc.s = ((answer & 0x80) != 0);    
            state->cc.cy = (answer > 0xff);    
            state->cc.p = get_parity(answer&0xff);    
            state->a = answer & 0xff;
            break;    
        }              
        case 0x87: //ADD A
        {    
            uint16_t answer = (uint16_t) state->a + (uint16_t) state->a;    
            state->cc.z = ((answer & 0xff) == 0);    
            state->cc.s = ((answer & 0x80) != 0);    
            state->cc.cy = (answer > 0xff);    
            state->cc.p = get_parity(answer&0xff);    
            state->a = answer & 0xff;
            break;    
        }            

        case 0x90: //SUB B
        {    
            uint16_t answer = (uint16_t) state->a - (uint16_t) state->b;    
            state->cc.z = ((answer & 0xff) == 0);    
            state->cc.s = ((answer & 0x80) != 0);    
            state->cc.cy = (answer > 0xff);    
            state->cc.p = get_parity(answer&0xff);    
            state->a = answer & 0xff;
            break;    
        }                      
        case 0x91: //SUB C
        {    
            uint16_t answer = (uint16_t) state->a - (uint16_t) state->c;    
            state->cc.z = ((answer & 0xff) == 0);    
            state->cc.s = ((answer & 0x80) != 0);    
            state->cc.cy = (answer > 0xff);    
            state->cc.p = get_parity(answer&0xff);    
            state->a = answer & 0xff;
            break;    
        }                      
        case 0x92: //SUB D
        {    
            uint16_t answer = (uint16_t) state->a - (uint16_t) state->d;    
            state->cc.z = ((answer & 0xff) == 0);    
            state->cc.s = ((answer & 0x80) != 0);    
            state->cc.cy = (answer > 0xff);    
            state->cc.p = get_parity(answer&0xff);    
            state->a = answer & 0xff;
            break;    
        }                   
        case 0x93: //SUB E
        {    
            uint16_t answer = (uint16_t) state->a - (uint16_t) state->e;    
            state->cc.z = ((answer & 0xff) == 0);    
            state->cc.s = ((answer & 0x80) != 0);    
            state->cc.cy = (answer > 0xff);    
            state->cc.p = get_parity(answer&0xff);    
            state->a = answer & 0xff;
            break;    
        }   
        case 0x94: //SUB H
        {    
            uint16_t answer = (uint16_t) state->a - (uint16_t) state->h;    
            state->cc.z = ((answer & 0xff) == 0);    
            state->cc.s = ((answer & 0x80) != 0);    
            state->cc.cy = (answer > 0xff);    
            state->cc.p = get_parity(answer&0xff);    
            state->a = answer & 0xff;
            break;    
        }           
        case 0x95: //SUB L
        {    
            uint16_t answer = (uint16_t) state->a - (uint16_t) state->l;    
            state->cc.z = ((answer & 0xff) == 0);    
            state->cc.s = ((answer & 0x80) != 0);    
            state->cc.cy = (answer > 0xff);    
            state->cc.p = get_parity(answer&0xff);    
            state->a = answer & 0xff;
            break;    
        }       
        case 0x96: //SUB M    
        {    
            uint16_t addr = (state->h<<8) | (state->l);    
            uint16_t answer = (uint16_t) state->a - state->memory[addr];    
            state->cc.z = ((answer & 0xff) == 0);    
            state->cc.s = ((answer & 0x80) != 0);    
            state->cc.cy = (answer > 0xff);    
            state->cc.p = get_parity(answer&0xff);    
            state->a = answer & 0xff;    
            break;
        }          
        case 0x97: //SUB A
        {    
            state->cc = condition_zero;
            state->a = 0;
            break;    
        } 
        case 0xa7: //ANA A
        {
            break;
        }
        case 0xaf: //XRA A
        {
            state->a = 0;
            state->cc = condition_zero;
            break;
        }
        case 0xc1: //POP B
        {
            state->b = state->memory[state->sp+1];  
            state->c = state->memory[state->sp];
            state->sp += 2;  
            break;  
        }
        case 0xc2: //JNZ addr
        {
            if (!state->cc.z)    
                state->pc = ((op[2] << 8) | op[1]) - 1;    
            else    
                state->pc += 2;    
            break;    
        }
        case 0xc3: //JMP addr    
        {
            state->pc = ((op[2] << 8) | (op[1])) - 1;    
            break;    
        }   
        case 0xc5: //PUSH B
        {
            state->memory[state->sp-1] = state->b;  
            state->memory[state->sp-2] = state->c ;
            state->sp -= 2;  
            break;  
        }
        case 0xc6: //ADI byte
        {
            uint16_t answer = (uint16_t) state->a + state->pc + 1;    
            state->cc.z = ((answer & 0xff) == 0);    
            state->cc.s = ((answer & 0x80) != 0);    
            state->cc.cy = (answer > 0xff);    
            state->cc.p = get_parity(answer&0xff);    
            state->a = answer & 0xff;
            state->pc += 1;
            break;
        }
        case 0xc9: //RET
        {
			state->pc = (state->memory[state->sp+1] << 8) | state->memory[state->sp];            
            state->sp += 2;    
            break;
        }
        case 0xcd: //CALL addr    
        {    

            state->memory[(state->sp)-1] = (state->pc + 2) >> 8;    
            state->memory[(state->sp)-2] = state->pc + 2;    
            state->sp -= 2;    
            state->pc = ((op[2] << 8) | (op[1])) - 1;    
            break; 
        }    
        case 0xd1: //POP D
        {
            state->d = state->memory[state->sp+1];  
            state->e = state->memory[state->sp];
            state->sp += 2;  
            break;  
        }
        case 0xd3: //OUT port
        {
        // This case (as well as IN) is handled in the main loop.
            break;  
        }
        case 0xd5: //PUSH D
        {
            state->memory[state->sp-1] = state->d;  
            state->memory[state->sp-2] = state->e ;
            state->sp -= 2;  
            break;  
        }
        case 0xdb: //IN port 
        {
        // This case (as well as OUT) is handled in the main loop.
            break;  
        }
        case 0xe1: //POP H
        {
            state->h = state->memory[state->sp+1];  
            state->l = state->memory[state->sp];
            state->sp += 2;  
            break;  
        }
        case 0xe5: //PUSH H
        {
            state->memory[state->sp-1] = state->h;  
            state->memory[state->sp-2] = state->l;
            state->sp -= 2;  
            break;  
        }
        case 0xe6: //ANI byte
        {
            uint16_t answer = (uint16_t) state->a & (state->pc + 1);    
            state->cc.z = ((answer & 0xff) == 0);    
            state->cc.s = ((answer & 0x80) != 0);    
            state->cc.cy = (answer > 0xff);    
            state->cc.p = get_parity(answer&0xff);    
            state->a = answer & 0xff;
            state->pc += 1;
            break;
        }
        case 0xeb: //XCHG
        {
            uint8_t temp_h, temp_l;
            temp_h = state->h;
            temp_l = state->l;
            state->h = state->d;
            state->d = temp_h;
            state->l = state->e;
            state->e = temp_l;
            break;
        }
        case 0xf1: //POP PSW
        {
            state->cc.s = (state->memory[state->sp] >> 7) & 1;
            state->cc.z = (state->memory[state->sp] >> 6) & 1;
            // state->cc.ac = (state->memory[state->sp] >> 4) & 1;
            state->cc.p = (state->memory[state->sp] >> 2) & 1;
            state->cc.cy = state->memory[state->sp] & 1;
            state->a = state->memory[state->sp+1];
            state->sp += 2;
        }
        case 0xf5: //PUSH PSW
        {
            uint8_t psw = 0;
            psw |= state->cc.s << 7;
            psw |= state->cc.z << 6;
            // psw |= state->cc.ac << 4;
            psw |= state->cc.p << 2;
            psw |= state->cc.cy << 6;
            state->memory[state->sp-2] = psw;
            state->memory[state->sp-1] = state->a;
            state->sp -= 2; 
        }
        case 0xfb: //EI
        {
            state->int_enable = 1;
            break;
        }
        case 0xfe: //CPI byte
        {
            uint16_t answer = (uint16_t) state->a - (state->pc + 1);
            state->cc.z = ((answer & 0xff) == 0);
            state->cc.s = ((answer & 0x80) != 0);   
            state->cc.cy = (answer > 0xff);
            state->cc.p = get_parity(answer&0xff);
            state->pc += 1;
            break;
        }
    default:
        printf("debugging error: opcode %x not recognised by emulator\n", *op);
        getchar();
        break;
    }    
    state->pc++;  // advance pc
}    
