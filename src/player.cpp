#include "player.h"
#include "game.h"

using namespace cd;

Player::Player(bn::camera_ptr _camera) : camera(_camera)
{
    cursor = Cursor(_camera);
    reset();
}

Player::~Player()
{
    clear_animations();
}

void Player::on_reset_store()
{
    cursor->remove_current_selection(true);
    cursor->hide_shop();
    clear_animations();
}

void Player::on_tick(Game *game)
{
    cursor->on_tick(game);

    if (rumble.has_value())
    {
        rumble->update();

        if (rumble->duration_updates() == 200)
        {
            rumble.reset();
        }
    }

    if (!camera_actions.empty())
    {
        camera_actions.front().update();

        if (camera_actions.front().done())
        {
            camera_actions.erase(camera_actions.begin());
        }
    }
}

void Player::on_target_killed(Target *target)
{
    add_money(target->get_reward());
}

void Player::on_hit(Enemy *enemy)
{
    life -= enemy->get_strenght();
    log("player life is now", life);
    rumble = bn::rumble_toggle_action(200);
    if (camera_actions.empty())
    {
        camera_actions.push_back(bn::camera_move_to_action(camera, 2, camera.x() + 10, camera.y()));
        camera_actions.push_back(bn::camera_move_to_action(camera, 4, camera.x() - 10, camera.y()));
        camera_actions.push_back(bn::camera_move_to_action(camera, 2, camera.x(), camera.y()));
    }
}

bn::fixed Player::get_life()
{
    return life;
}

bn::fixed Player::get_money()
{
    return money;
}

void Player::add_money(bn::fixed amount)
{
    if (money + amount > 999)
    {
        log("player cannot have more than 999 money");
        return;
    }
    log("player earned money", amount);
    money += amount;
}

bool Player::is_dead()
{
    return life <= 0;
}

void Player::clear_animations()
{
    if (!camera_actions.empty())
    {
        camera.set_position(camera_actions.back().final_position());
        camera_actions.clear();
    }
}

void Player::reset()
{
    life = 100;
    money = fallback_money;
    cursor->hide();
    clear_animations();
}

void Player::spend_money(bn::fixed cost)
{
    money -= cost;

    if (money < 0)
    {
        // should never happen but I don't trust myself
        log("player money was negative. reset it to 0.");
        money = 0;
    }
}

void Player::set_money(const bn::fixed amount)
{
    money = amount;
}

void Player::set_life(bn::fixed value)
{
    life = value;
}

void Player::activate()
{
    cursor->show();
}
