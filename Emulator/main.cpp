
#include "../Shared/stdafx.h"
#include "../Shared/primitives.h"
#include "processor_state.h"

#define ROMSIZE 0x10000

using byte = primitives::byte;
using word = primitives::word;
using bytes = primitives::bytes;
using flags = emulator::state::flags;


emulator::state processor;

int main() {
    std::ifstream inhex;
    bytes operation = 1;
    bytes ROM(ROMSIZE);

    //Open and read
    inhex.open( "out.hex", std::ios::in | std::ios::binary );
    for( size_t i = 0x0; i < ROMSIZE; i++ ) {
		try {
			ROM.data[i] = inhex.get() & 0xFF;
		} catch(std::exception e) {
			PRINT("Error loading ROM on address " << HEX(i));
		}
    }
    inhex.close();

    processor.PC = 0x0600;

    //Iterate and execute
#define READBYTE() ROM[processor.PC++]
#define SWAP(a1,a2) (a1+(a2<<8))
    while( processor.ok ) {
        byte opcode = READBYTE();
		PRINT(HEX(opcode));
        switch( opcode ) {
            case 0x00://BRK
                processor.ok = false;
                break;
            case 0x4C://JMP label/$xxxx
                processor.PC = SWAP( READBYTE(), READBYTE() );
				PRINT("Done a jump to " << HEX(processor.PC));
                break;
            case 0x65://ADC $xx
                processor.addToA( ROM[READBYTE()] );
                break;
            case 0x8D://STA $xxxx
                ROM[SWAP( READBYTE(), READBYTE() )] = processor.A;
                break;
            case 0xA9://LDA #$xx
                processor.setA( ROM[processor.PC++] );
                break;
            case 0xAA://TAX
                processor.setX( processor.A );
                break;
            default:
                std::cout << "Error: not implemented: " << std::hex << ( int )opcode << std::endl;
                break;
        }
    }
#undef READBYTE
#undef SWAP

    std::cout << processor.dump().str();

    return 0;
}