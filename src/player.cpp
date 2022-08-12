#include "player.h"

using namespace cd;

Player::Player()
{
    cursor = Cursor();
}

Player::~Player()
{
}

void Player::on_tick(Level *level)
{
    cursor.on_tick(level);
}
