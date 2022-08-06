#include "bn_core.h"
#include "bn_keypad.h"

#include "level.h"
#include "generated/levels_intgrid.h"

int main()
{
    bn::core::init();

    bn::camera_ptr camera = bn::camera_ptr::create(0, 0);

    cd::Level *current_level = cd::levels[0];

    current_level->init(camera);

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

        bn::core::update();
    }
}
