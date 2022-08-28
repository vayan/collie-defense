#include "player.h"

using namespace cd;

Player::Player(bn::camera_ptr _camera)
{
    cursor = Cursor(_camera);
}

Player::~Player()
{
}

void Player::on_tick(Level *level)
{
    if (life <= 0)
    {
        level->on_gameover();
        life = 100; // TODO temp for restart
    }

    cursor.value().on_tick(level);
}

void Player::on_target_killed(Target *target)
{
    money += target->get_reward();
}

void Player::on_hit(Enemy *enemy)
{
    life -= enemy->get_strenght();
}

bn::fixed Player::get_life()
{
    return life;
}
bn::fixed Player::get_money()
{
    return money;
}
