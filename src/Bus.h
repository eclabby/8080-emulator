#ifndef BUS_H
#define BUS_H

#include <array>

#include "common.h"
#include "CPU.h"
#include "Cart.h"



class Bus
{
public:
    Bus();


    CPU cpu;
    Cart cart;


    std::array<u8, 2*1024> ram;


    u8 bus_read(u16 addr);
    void bus_write(u16 addr, u8 data);

private:



};

#endif
