#include "player.h"
#include "game.h"

using namespace cd;

Player::Player(bn::camera_ptr _camera)
{
    cursor = Cursor(_camera);
}

Player::~Player()
{
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
}

void Player::on_target_killed(Target *target)
{
    money += target->get_reward();
}

void Player::on_hit(Enemy *enemy)
{
    life -= enemy->get_strenght();
    log("player life is now", life);
    rumble = bn::rumble_toggle_action(200);
}

bn::fixed Player::get_life()
{
    return life;
}
bn::fixed Player::get_money()
{
    return money;
}

bool Player::is_dead()
{
    return life <= 0;
}

void Player::reset()
{
    life = 100;
    money = 0;
}

void Player::spend_money(bn::fixed cost)
{
    money -= cost;
}
