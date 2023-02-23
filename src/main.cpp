#include "bn_core.h"
#include "debug.h"
#include "game.h"
#include "menu.h"

int main()
{
    cd::log("main called");

    bn::core::init();

    cd::log("butano initiated");

    cd::Menu *menu = new cd::Menu();
    cd::Game *game = new cd::Game(menu);

    return game->start_main_loop();
}
