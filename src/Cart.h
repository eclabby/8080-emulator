#ifndef CART_H
#define CART_H

#include <string>

#include "common.h"

#define ROM_PATH "../roms/"


struct cart_context
{
    u32 ROM_size;
    u8 *ROM_data; // 8-bit word length
};



class Cart
{
public:
    Cart();


    struct cart_context ctx;


    bool cart_load(std::string cart);


    u8 cart_read(u16 addr);
    void cart_write(u16 addr, u8 data);

private:
    
};

#endif
