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
    cursor.value().on_tick(level);
}
