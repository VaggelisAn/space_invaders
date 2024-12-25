#include "space_invaders.h"

int get_parity(unsigned int n) {
    int parity = 0;
    while (n) {
        parity ^= 1;    
        n &= (n - 1); 
    }
    return parity;
}

void emulator_8080(STATE_8080* state){    
    unsigned char *op = &state->memory[state->pc];    

//  Reseting cy flag:
    state->cc.cy = 0;

//  Flags when the accumulator (reg a) is 0:
    CONDITION_CODES condition_zero;
    condition_zero.z = 1;    
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
            uint8_t low_byte = state->memory[++state->pc];
            uint8_t high_byte = state->memory[++state->pc];
            state->b = high_byte;
            state->c = low_byte;
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
            uint8_t low_byte = state->memory[++state->pc];
            state->b = low_byte;
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
            uint8_t low_byte = state->memory[++state->pc];
            state->c = low_byte;
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
            uint8_t low_byte = state->memory[++state->pc];
            uint8_t high_byte = state->memory[++state->pc];
            state->d = high_byte;
            state->e = low_byte;
            break;
        }
        case 0x13: //INX D
        {
            uint16_t answer = ((state->b<<8) | (state->c)) + 1;
            state->c = (uint8_t) answer & 0xFF;
            state->b = (uint8_t) (answer>>8) & 0xFF;
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
            uint8_t low_byte = state->memory[++state->pc];
            uint8_t high_byte = state->memory[++state->pc];
            state->h = high_byte;
            state->l = low_byte;
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
            uint8_t low_byte = state->memory[++state->pc];  
            state->memory[addr] = low_byte;
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
            uint8_t low_byte = state->memory[ + 1];
            uint8_t high_byte = state->memory[++state->pc];
            state->sp = (high_byte << 8) | low_byte;
            break;
        }
        case 0x32: //STA addr
        {
            uint16_t addr = (state->memory[state->pc + 1]<<8) | (state->memory[state->pc + 2]);
            state->memory[addr] = state->a;
            state->pc += 2;
            break;
        }
        case 0x36: //MVI M, addr
        {
            uint16_t addr = (state->h<<8) | (state->l);  
            uint8_t low_byte = state->memory[++state->pc]; 
            state->memory[addr] = low_byte;
            break;
        }
        case 0x3a: //LDA addr
        {
            uint16_t addr = (state->memory[state->pc + 1]<<8) | (state->memory[state->pc + 2]);
            state->a = state->memory[addr];
            state->pc += 2;
            break;
        }
        case 0x3e: //MVI A, addr
        {
            uint8_t low_byte = state->memory[++state->pc];
            state->a = low_byte;
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
        case 0xc9: //RET
        {
            uint8_t low_byte = state->memory[state->sp];
            uint8_t high_byte = state->memory[state->sp]; 
            uint16_t addr = (low_byte<<8) | (high_byte);   
            state->pc = addr;
            state->sp += 2;
        }
    default:
        printf("debugging error: opcode %x not recognised by emulator\n", *op);
        break;
    }    
    state->pc++;  // advance pc
}    
