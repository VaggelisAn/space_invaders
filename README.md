# space_invaders

Emulator for the videogame Space Invaders, written in C.
Space Invaders originally uses an Intel 8080 CPU. This project does not fully emulate the 8080 architecture, since only 50 instructions were used for the development of the game.

This project follows the steps described in emulator101.com (as of now the site is down, and can be accessed through the wayback machine).

Later goals include optimization experiments.

Common Abbreviations used:

R       Any of the 8-Bit registers A,B,C,D,E,H,L.

data    Any 8-bit or 16-bit value.

PC      Program Counter.

SP      Stack Pointer.

RM      Register A,B,C,D,E,H,L or memory M pointed by HL.

BD      Either register pair BC or DE  (B=BC, D=DE).

BP      Any byte pair symbol (B=BC, D=DE, H=HL, PSW=AF).

RP      Any register pair (B=BC, D=DE, H=HL, SP=SP).

Addr    A 16-Byte address.

(Source: https://gist.github.com/joefg/634fa4a1046516d785c9)

The 8080's flags are named Z, S, P, CY, and AC.

Z (zero) set to 1 when the result is equal to zero

S (sign) set to 1 when bit 7 (the most significant bit or MSB) of the math instruction is set

P (parity) is set when the answer has even parity, clear when odd parity

CY (carry) set to 1 when the instruction resulted in a carry out or borrow into the high order bit

AC (auxillary carry) is used mostly for BCD (binary coded decimal) math. Read the data book for more details, Space Invaders doesn't use it.

(Source: http://www.emulator101.com/arithmetic-group.html)
