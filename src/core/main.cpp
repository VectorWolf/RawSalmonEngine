/*
 * Copyright 2017-2018 Agouti Games Team (see the AUTHORS file)
 *
 * This file is part of the RawSalmonEngine.
 *
 * The RawSalmonEngine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The RawSalmonEngine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the RawSalmonEngine.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <iostream>

#include "core/gameinfo.hpp"
#include "event/ae_move_sustained.hpp"

/// The game Loop!
int main()
{
    GameInfo game(800, 600);
    if(!game.load_map("example.tmx")) {
        std::cerr << "Map couldn't load\n";
    }
    else {
        bool player_input = true;
        if(!game.fetch_player()) {
            std::cerr << "Could not fetch player. Input deactivated!\n";
            player_input = false;
        }
        //Event handler
        SDL_Event e;

        //Main loop flag
        bool quit = false;

        //While application is running
        while( !quit ){
            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 ) {
                //User requests quit
                if( e.type == SDL_QUIT )
                {
                    quit = true;
                }
                //User presses a key
                else if( e.type == SDL_KEYDOWN && e.key.repeat == 0)
                {
                    //Move player or camera based on key press
                    switch( e.key.keysym.sym )
                    {
                        case SDLK_UP:
                        game.m_camera.y -= 50;
                        break;

                        case SDLK_w:
                        if(player_input){game.m_player->add_event(AeMoveSustained::create(Direction::up, e.key.keysym));}
                        break;

                        case SDLK_DOWN:
                        game.m_camera.y += 50;
                        break;

                        case SDLK_s:
                        if(player_input){game.m_player->add_event(AeMoveSustained::create(Direction::down, e.key.keysym));}
                        break;

                        case SDLK_LEFT:
                        game.m_camera.x -= 50;
                        break;

                        case SDLK_a:
                        if(player_input){game.m_player->add_event(AeMoveSustained::create(Direction::left, e.key.keysym));}
                        break;

                        case SDLK_RIGHT:
                        game.m_camera.x += 50;
                        break;

                        case SDLK_d:
                        if(player_input){game.m_player->add_event(AeMoveSustained::create(Direction::right, e.key.keysym));}
                        break;
                    }
                }
            }
            game.update();
            game.render();
        }
    }
    return 0;
}
