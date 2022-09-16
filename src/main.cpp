#include "bn_core.h"
#include "debug.h"
#include "game.h"

int main()
{
    cd::log("main called");

    bn::core::init();

    cd::log("butano initiated");

    cd::Game *game = new cd::Game();

    return game->start_main_loop();
}
