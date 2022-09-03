#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_log.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_ptr.h"
#include "debug.h"
#include "generated/levels_intgrid.h"
#include "generated/world_config.h"
#include "level.h"
#include "player.h"
#include "ui.h"

int main()
{
    bn::core::init();

    bn::camera_ptr camera = bn::camera_ptr::create(0, 0);

    int current_level_index = 0;

    cd::Level *current_level = cd::levels[current_level_index];
    cd::Player player = cd::Player(camera);
    cd::UI ui = cd::UI();

    current_level->init(camera);

    cd::log("Start Game!");

    while (true)
    {
        if (ui.is_paused())
        {
        }
        else
        {
            current_level->tick(camera, &player);
            player.on_tick(current_level);
        }

        ui.on_tick(current_level, &player);

        bn::core::update();

        if (player.is_dead())
        {
            cd::log("game over");

            // TODO implement gameover screen - restart for now
            return 0;
        }
        else if (current_level->is_won())
        {
            current_level_index += 1;

            if (current_level_index >= cd::number_of_levels)
            {
                cd::log("end of game");
                // TODO show end screen - restart for now
                return 0;
            }

            cd::log("loading next level number", current_level_index);
            current_level->reset();
            current_level = cd::levels[current_level_index];
            current_level->init(camera);
        }
    }
}
