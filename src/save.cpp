#include "save.h"

using namespace cd;

Save::Save()
{
    read();
}

Save::~Save()
{
}

void Save::save_story_progress(int level_index, bn::fixed money, bn::fixed life)
{
    read();
    cart_save_data.latest_story_level = level_index;
    cart_save_data.story_money = money;
    cart_save_data.story_life = life;

    write();
}

void Save::save_level_score(int level_index, bn::fixed score)
{
    read();
    cart_save_data.score_per_level[level_index] = score;
    write();
}

void Save::read()
{
    bn::sram::read(cart_save_data);
    log("Save read!", cart_save_data);

    if (cart_save_data.status != SAVE_STATUS_VALID_CODE) // this mean the save was never created
    {
        reset();
    }
}

void Save::write()
{
    bn::sram::write(cart_save_data);
    log("Save wrote!", cart_save_data);
}

void Save::reset()
{
    log("Reseting save...");
    cart_save_data.status = SAVE_STATUS_VALID_CODE;
    cart_save_data.latest_story_level = 0;
    cart_save_data.story_money = 50;
    cart_save_data.story_life = 100;
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
