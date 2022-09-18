#include "menu.h"
#include "game.h"

using namespace cd;

Menu::Menu()
{
    bg = bn::regular_bg_items::launch_background.create_bg(0, 0);
}

Menu::~Menu()
{
}

bool Menu::on_tick(Game *game)
{
    bg.value().set_camera(game->get_camera());

    if (bn::keypad::start_pressed())
    {
        return false;
    }

    return true;
}
