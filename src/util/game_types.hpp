/*
 * Copyright 2017-2020 Agouti Games Team (see the AUTHORS file)
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
#ifndef GAME_TYPES_HPP_INCLUDED
#define GAME_TYPES_HPP_INCLUDED

#include <string>
#include <vector>
#include <SDL.h>
#include "../../include/types.hpp"

namespace salmon { namespace internal {

/**
 * @brief A collection of various enums and handy functions for conversion and parsing
 */

enum class Response{
    on_hit,
    on_collision,
    on_activation,
    on_death,
    on_idle,
    on_always,
    on_spawn,
    on_mouse,
    invalid,
};

enum class PropertyType{
    Boolean,
    Integer,
    Float,
    String,
};

enum class EventSignal{
    next, ///< Process next event in pipeline
    stop, ///< Stop processing events here
    end,  ///< The event succesfully finished, needs to be deleted
    abort,///< The event preemtively finished, needs to be deleted
//    erase,///< Erase the actor who processed this event
    invalid, ///< EventSignal did not properly parse
};

enum class Priority{
    low = 25,
    medium = 50,
    high = 75,
    invalid = -1,
};

// AnimationType str_to_anim_type(const std::string& name);
Direction str_to_direction(const std::string& name);
Priority str_to_priority(const std::string& name);
EventSignal str_to_event_signal(const std::string& name);
Response str_to_response(const std::string& name);
SDL_Color str_to_color(const std::string& name);
void make_path_absolute(std::string& path);

SDL_Point rect_center_difference(const SDL_Rect& first, const SDL_Rect& second);
Rect make_rect(const SDL_Rect& rect);
SDL_Rect make_rect(const Rect& rect);

std::vector<float> dir_to_mov(const Direction dir);
void normalize(float& x, float& y);

}} // namespace salmon::internal

#endif // GAME_TYPES_HPP_INCLUDED
