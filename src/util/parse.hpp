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
#ifndef PARSE_HPP_INCLUDED
#define PARSE_HPP_INCLUDED

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <map>

#include "util/game_types.hpp"
#include "util/tinyxml2.h"

class MapData;
class SoundEffect;
class Texture;

namespace parse{
    tinyxml2::XMLError hitbox(tinyxml2::XMLElement* source, SDL_Rect& rect);
    tinyxml2::XMLError hitboxes(tinyxml2::XMLElement* source, std::map<std::string, SDL_Rect>& rects);
    tinyxml2::XMLError blendmode(tinyxml2::XMLElement* source, Texture& img);

    tinyxml2::XMLError bg_color(tinyxml2::XMLElement* source, SDL_Color& color);
}

/**
 * @brief A helper class which parses event properties and passes them to previously added references
 */
class Parser{
    public:
        Parser(MapData& base_map);

        void add(bool& value, std::string name) {m_bool[name] = &value;}
        void add(int& value, std::string name) {m_int[name] = &value;}
        void add(float& value, std::string name) {m_float[name] = &value;}
        void add(std::string& value, std::string name) {m_string[name] = &value;}

        void add(Priority& value, std::string name) {m_priority[name] = &value;}
        void add(EventSignal& value, std::string name) {m_signal[name] = &value;}
        void add(Direction& value, std::string name) {m_direction[name] = &value;}
        void add(AnimationType& value, std::string name) {m_animation_type[name] = &value;}

        void add(std::vector<std::string>& value) {m_iterate = &value;}

        tinyxml2::XMLError parse(tinyxml2::XMLElement* source);
        tinyxml2::XMLError parse_ignore_unknown(tinyxml2::XMLElement* source);

    private:
        static constexpr int type_count = 9;
        MapData* m_base_map;

        std::map<std::string, bool*> m_bool;
        tinyxml2::XMLError parse_bool(tinyxml2::XMLElement* source);

        std::map<std::string, int*> m_int;
        tinyxml2::XMLError parse_int(tinyxml2::XMLElement* source);

        std::map<std::string, float*> m_float;
        tinyxml2::XMLError parse_float(tinyxml2::XMLElement* source);

        std::map<std::string, std::string*> m_string;
        tinyxml2::XMLError parse_string(tinyxml2::XMLElement* source);


        std::map<std::string, Priority*> m_priority;
        tinyxml2::XMLError parse_priority(tinyxml2::XMLElement* source);

        std::map<std::string, EventSignal*> m_signal;
        tinyxml2::XMLError parse_signal(tinyxml2::XMLElement* source);

        std::map<std::string, Direction*> m_direction;
        tinyxml2::XMLError parse_direction(tinyxml2::XMLElement* source);

        std::map<std::string, AnimationType*> m_animation_type;
        tinyxml2::XMLError parse_animation_type(tinyxml2::XMLElement* source);


        std::vector<std::string>* m_iterate = nullptr;
        tinyxml2::XMLError parse_iteration(tinyxml2::XMLElement* source);


        typedef tinyxml2::XMLError(Parser::* ParsePointer)(tinyxml2::XMLElement* source);
        ParsePointer parsers[type_count];

};



#endif // PARSE_HPP_INCLUDED
