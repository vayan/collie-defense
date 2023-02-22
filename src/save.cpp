#include "save.h"

using namespace cd;

Save::Save()
{
    read();
}

Save::~Save()
{
}

void Save::read()
{
    bn::sram::read(cart_save_data);
    log("Save read!");
    log("current level saved", cart_save_data.latest_level);
    if (cart_save_data.status != SAVE_STATUS_VALID_CODE) // this mean the save was never created
    {
        reset();
    }
}

void Save::write()
{
    bn::sram::write(cart_save_data);
    log("Save wrote!");
}

void Save::reset()
{
    log("Save reset!");
    cart_save_data.status = SAVE_STATUS_VALID_CODE;
    cart_save_data.latest_level = 0;

    write();
}

save_data Save::get_data()
{
    return cart_save_data;
}
