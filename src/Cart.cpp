#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "Cart.h"

using namespace std;



bool Cart::cart_load(string cart)
{
    string file = ROM_PATH + cart;

    ifstream in;
    in.open(file, ios::binary);

    if (in.fail())
    {
        cerr << "Failed to open " + cart << endl;
        return false;
    }
    cout << "Successfully opened " + cart << endl;

    // get file size in bytes
    in.seekg(0, ios::end);
    ctx.ROM_size = in.tellg();

    in.seekg(0); // rewind

    ctx.ROM_data = (u8 *)malloc(ctx.ROM_size);
    in.read((char *)ctx.ROM_data, ctx.ROM_size);
    in.close();

    cout << "ROM Loaded" << endl;

    return true;
}


u8 Cart::cart_read(u16 addr) 
{  
    return *(ctx.ROM_data + addr);
}

void Cart::cart_write(u16 addr, u8 data)
{
    // read only but option is there
    *(ctx.ROM_data + addr) = data;
}

