/*
 * Copyright 2017-2019 Agouti Games Team (see the AUTHORS file)
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
#ifndef AE_SPAWN_TEXT_HPP_INCLUDED
#define AE_SPAWN_TEXT_HPP_INCLUDED

#include <vector>
#include <string>

#include "event/event.hpp"
#include "event/property_listener.hpp"
#include "util/game_types.hpp"

class Actor;

/**
 * @brief Write something
 */
class AeSpawnText : public Event<Actor>{
    public:
        tinyxml2::XMLError init(tinyxml2::XMLElement* source, MapData& base_map) override;
        EventSignal process(Actor& scope) override;

        // Covariant return type!
        AeSpawnText* clone() const override {return new AeSpawnText(*this);}

        std::string get_type() const override {return m_alias;}

    private:
        static const bool good;
        static const std::string m_alias;
        PropertyListener<AeSpawnText> m_property_listener;
        // vv Add members with default values
        std::string m_font = "OpenSans", m_color = "#000000", m_primitive_name, m_text, m_layer_name;
        int m_x_pos = 0, m_y_pos = 0, m_width = 1000, m_height = 500, m_font_size = 12;
        bool m_bold = false, m_italic = false, m_underline = false, m_strikeout = false;
        bool m_kerning = true, m_wrap = false, m_center = true;

};

#endif // AE_SPAWN_TEXT_HPP_INCLUDED
