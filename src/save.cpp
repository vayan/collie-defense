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
    cart_save_data.story_money = 20;
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

const char *Save::encode_for_qrcode()
{
    static std::string encoded; // bad bad using std::string

    encoded = "HTTP://TD.GY/";

    read();
    bn::fixed story_progress = (cart_save_data.latest_story_level.safe_multiplication(9)).safe_division(number_of_levels).ceil_integer();

    encoded += convertToBase36(number_of_levels.integer());

    encoded += convertToBase36(story_progress.integer());
    encoded += ".";

    int i = number_of_levels.integer() - 1;

    while (i >= 0)
    {
        int result = 0;
        int power = 1;
        while (i >= 0 && power < 1000000)
        {
            bn::fixed level_score = cart_save_data.score_per_level[i];

            if (level_score <= 0)
            {
                level_score = 1;
            }
            if (level_score == 10)
            {
                level_score = 9;
            }
            result += level_score.integer() * power;
            power *= 10;
            i--;
        }
        encoded += convertToBase36(result);
        if (i >= 0)
        {
            encoded += ".";
        }
    }

    return encoded.c_str();
}
