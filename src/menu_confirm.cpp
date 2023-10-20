#include "menu_confirm.h"

using namespace cd;

MenuConfirm::MenuConfirm(bn::fixed_point _pos)
{
    select_confirm = bn::sprite_items::confirm_loader.create_sprite(_pos);
    select_confirm_loading = bn::create_sprite_animate_action_once(
        select_confirm.value(),
        8,
        bn::sprite_items::confirm_loader.tiles_item(),
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17);

    select_confirm->set_bg_priority(0);
}

MenuConfirm::~MenuConfirm()
{
}

bool MenuConfirm::accepted()
{
    while (bn::keypad::start_held() || bn::keypad::a_held())
    {
        if (select_confirm_loading->done())
        {
            log("menu confirm accepted");
            return true;
        }

        select_confirm_loading->update();
        bn::core::update();
    }
    log("menu confirm rejected");
    return false;
}
