#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_log.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_ptr.h"

#include "level.h"
#include "generated/levels_intgrid.h"
#include "generated/world_config.h"
#include "bn_sprite_items_sheep.h"

int main()
{
    bn::core::init();

    bn::camera_ptr camera = bn::camera_ptr::create(0, 0);

    cd::Level *current_level = cd::levels[0];

    current_level->init(camera);

    bn::fixed_point start = current_level->get_start();
    bn::fixed_point end = current_level->get_end();

    BN_LOG("Start Game!");
    BN_LOG(bn::format<30>("Start: {},{} End: {},{}", start.x(), start.y(), end.x(), end.y()));

    bn::sprite_ptr sprite = bn::sprite_items::sheep.create_sprite(0, 0);

    sprite.set_position(start);
    sprite.set_camera(camera);
    sprite.set_visible(true);

    while (true)
    {
        current_level->tick(camera);

        if (bn::keypad::right_held())
        {
            camera.set_x(camera.x() + 1);
        }

        if (bn::keypad::left_held())
        {
            camera.set_x(camera.x() - 1);
        }

        if (bn::keypad::down_held())
        {
            camera.set_y(camera.y() + 1);
        }

        if (bn::keypad::up_held())
        {
            camera.set_y(camera.y() - 1);
        }

        sprite.set_x(sprite.x() + 0.1);

        bn::core::update();
    }
}
