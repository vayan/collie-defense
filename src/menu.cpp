#include "menu.h"
#include "game.h"

using namespace cd;

Menu::Menu()
{
    bg = bn::regular_bg_items::launch_background.create_bg(0, 0);
    bn::music_items::takeawalk.play();
}

Menu::~Menu()
{
}

void Menu::clear()
{
    bg.reset();
    current_screen = MenuScreen::Start;
}

void Menu::switch_screen(MenuScreen screen)
{
    switch (screen)
    {
    case MenuScreen::GameOver:
        bg = bn::regular_bg_items::gameover.create_bg(0, 0);
        break;

    case MenuScreen::Win:
        bg = bn::regular_bg_items::gamewin.create_bg(0, 0);
        break;

    default:
        bg = bn::regular_bg_items::launch_background.create_bg(0, 0);
        break;
    }

    current_screen = screen;
}

bool Menu::on_tick(Game *game, MenuScreen screen)
{
    if (screen != current_screen || !bg.has_value())
    {
        switch_screen(screen);
    }

    bg->set_camera(game->get_camera());

    if (bn::keypad::start_pressed() || bn::keypad::a_pressed())
    {
        return false;
    }

    return true;
}
