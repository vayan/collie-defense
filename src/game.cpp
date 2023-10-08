#include "game.h"

using namespace cd;

Game::Game(Menu *_menu) : menu(_menu)
{
    camera = bn::camera_ptr::create(0, 0);
    save = new Save();
    player = new Player(camera.value());

    log("game manager created!");
}

Game::~Game()
{
}

void Game::start_level(int level_index)
{
    if (level_index >= cd::number_of_levels)
    {
        return;
    }

    reset_current_level();

    player.value()->on_reset_store();

    cd::log("loading level number", level_index);

    current_level = levels[level_index]->copy();

    if (get_game_mode() == GameMode::Single)
    {
        player.value()->reset();
        player.value()->set_money(current_level.value()->get_start_money());
    }

    player.value()->activate();

    current_level.value()->init(camera.value());
    display_memory_left();
}

MenuScreen Game::start_level_loop()
{
    start_level(current_level_index);
    while (true)
    {
        if (bn::keypad::start_pressed())
        {
            log("paused the game");
            toggle_pause();
        }

        if (is_paused)
        {
            if (bn::keypad::down_pressed())
            {
                current_selection_index += 1;
                bn::sound_items::select.play();
            }

            if (bn::keypad::up_pressed())
            {
                current_selection_index -= 1;
                bn::sound_items::select.play();
            }

            if (current_selection_index < 0)
            {
                current_selection_index = menu_elements.size() - 1;
            }

            if (current_selection_index >= menu_elements.size())
            {
                current_selection_index = 0;
            }

            menu_select->set_position(menu_elements.at(current_selection_index).second);

            if (bn::keypad::a_pressed())
            {
                switch (menu_elements.at(current_selection_index).first)
                {
                case MenuScreen::Cancel:
                    log("continue the level");
                    toggle_pause();
                    break;
                case MenuScreen::Restart:
                    log("restart the level");
                    pause_bg.reset();
                    menu_select.reset();
                    current_selection_index = 0;
                    menu_elements.clear();
                    player.value()->reset_to_checkpoint();
                    stop_pause();
                    return MenuScreen::Restart;
                case MenuScreen::Play:
                    log("restart the game");
                    pause_bg.reset();
                    menu_select.reset();
                    current_selection_index = 0;
                    menu_elements.clear();
                    stop_pause();
                    player.value()->on_reset_store();
                    return MenuScreen::Play;
                default:
                    break;
                }
            }
        }

        if (!is_paused)
        {
            current_level.value()->tick(this);
            player.value()->on_tick(this);
        }

        bn::core::update();

        if (player.value()->is_dead())
        {
            cd::log("game over");
            player.value()->on_reset_store();
            current_level.value()->reset();
            save->save_story_progress(0, fallback_money, 100);

            return MenuScreen::GameOver;
        }
        else if (current_level_index >= cd::number_of_levels)
        {
            cd::log("end of game");
            current_level.value()->reset();
            return MenuScreen::Win;
        }
        else if (current_level.value()->is_won() && get_game_mode() == GameMode::Story)
        {
            cd::log("story level finished");
            player.value()->disable();
            player.value()->set_life(player.value()->get_life());
            player.value()->set_money(player.value()->get_money());
            current_level_index += 1;
            save->save_story_progress(current_level_index, player.value()->get_money(), player.value()->get_life());
            return MenuScreen::LevelWin;
        }
        else if (current_level.value()->is_won() && get_game_mode() == GameMode::Single)
        {
            cd::log("score level finished");
            // add a timer or keep life left for scoring?? mhhh
            save->save_level_score(
                current_level_index,
                player.value()->get_life().safe_division(10).round_integer());

            player.value()->disable();
            player.value()->reset();

            return MenuScreen::LevelWin;
        }
    }
    player.value()->on_reset_store();

    return MenuScreen::Start;
}

void Game::reset_current_level()
{
    if (current_level.has_value())
    {
        delete current_level.value();
        current_level.reset();
    }
}

int Game::start_main_loop()
{
    log("starting main loop");

    while (true)
    {
        display_memory_left();
        player.value()->disable();
        start_menu_screen_loop();
        display_memory_left();
        current_level_index = menu->get_selected_level().integer();

        bn::core::update();
        MenuScreen transition_to = start_level_loop();
        display_memory_left();
        menu->set_current_screen(transition_to);
        bn::core::update();
    }
}

void Game::start_menu_screen_loop()
{
    while (menu->on_tick(this))
    {
        bn::core::update();
    }
    menu->clear();
}

void Game::stop_pause()
{
    pause_bg.reset();
    is_paused = false;
}

void Game::toggle_pause()
{
    if (is_paused)
    {
        player.value()->activate();
        stop_pause();
        current_level.value()->get_hud()->set_state(true);
        menu_select.reset();
        current_selection_index = 0;
        menu_elements.clear();

        if (bn::music::paused() && bn::music::playing())
        {
            bn::music::resume();
        }
    }
    else
    {
        if (!bn::music::paused() && bn::music::playing())
        {
            bn::music::pause();
        }
        player.value()->disable();
        is_paused = true;
        pause_bg = bn::regular_bg_items::pause.create_bg(0, 0);
        pause_bg->set_camera(camera);
        pause_bg->set_visible(true);
        pause_bg->set_priority(1);
        current_level.value()->get_hud()->set_state(false);
        menu_select = bn::regular_bg_items::menu_select_item_highlight.create_bg(0, -20);
        menu_select->set_priority(1);
        current_selection_index = 0;
        menu_elements.emplace_back(MenuScreen::Cancel, bn::fixed_point(2, -20));
        menu_elements.emplace_back(MenuScreen::Restart, bn::fixed_point(2, 4));
        menu_elements.emplace_back(MenuScreen::Play, bn::fixed_point(2, 29));
        bn::core::update();
    }
}

Player *Game::get_player()
{
    return player.value();
}

Level *Game::get_current_level()
{
    return current_level.value();
}

int Game::get_current_level_index()
{
    return current_level_index;
}

bn::camera_ptr Game::get_camera()
{
    return camera.value();
}

GameMode Game::get_game_mode()
{
    return game_mode;
}

void Game::set_game_mode(GameMode mode)
{
    game_mode = mode;
}

Save *Game::get_save()
{
    return save;
}
