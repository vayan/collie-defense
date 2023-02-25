#include "save.h"

using namespace cd;

Save::Save()
{
    read();
}

Save::~Save()
{
}

void Save::set_latest_level(int level_index)
{
    read();
    cart_save_data.latest_level = level_index;
    write();
}

void Save::set_level_score(int level_index, bn::fixed score)
{
    read();
    cart_save_data.score_per_level[level_index] = score;
    write();
}

void Save::read()
{
    bn::sram::read(cart_save_data);
    log("Save read!");
    if (cart_save_data.status != SAVE_STATUS_VALID_CODE) // this mean the save was never created
    {
        reset();
    }
    log("latest level in story saved", cart_save_data.latest_level);
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
    for (int i = 0; i < MAX_LEVEL_COUNT; i++)
    {
        cart_save_data.score_per_level[i] = -1;
    }

    write();
}

save_data Save::get_data()
{
    return cart_save_data;
}
