#include "Bus.h"



Bus::Bus()
{
}


// Memory Map
// 0x0000 - 0x1FFF 8K ROM
// 0x2000 - 0x23FF 1K RAM
// 0x2400 - 0x3FFF 7K Video RAM
// 0x4000 -        RAM mirror


u8 Bus::bus_read(u16 addr)
{
    if (addr >= 0x0000 && addr <= 0x1FFF)
    {
        // TODO read from cart
        return 0;
    }
    else if (addr >= 0x2000 && addr <= 0x23FF)
    {
        return ram[addr - 0x2000];
    }
    else if (addr >= 0x2400 && addr <= 0x3FFF)
    {
        // vram, not yet implemented
        return 0x00;
    }
    return 0x00;
}

void Bus::bus_write(u16 addr, u8 data)
{
    if (addr >= 0x0000 && addr <= 0x1FFF)
    {
        // TODO write to cart
        return;
    }
    else if (addr >= 0x2000 && addr <= 0x23FF)
    {
        ram[addr - 0x2000] = data;
        return;
    }
    else if (addr >= 0x2400 && addr <=0x3FFF)
    {
        return;
    }
}

