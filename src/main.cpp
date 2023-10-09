#include "bn_core.h"
#include "debug.h"
#include "game.h"
#include "menu.h"
#include "save.h"

int main()
{
    cd::log("main called");

    bn::core::init();

    cd::log("butano initiated");

    cd::Save *save = new cd::Save();
    cd::Menu *menu = new cd::Menu();
    cd::Game *game = new cd::Game(menu, save);

    cd::display_memory_left();

    return game->start_main_loop();
}
