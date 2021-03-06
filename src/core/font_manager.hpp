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
#ifndef FONT_MANAGER_HPP_INCLUDED
#define FONT_MANAGER_HPP_INCLUDED

#include <string>
#include <map>
#include <SDL_ttf.h>

namespace salmon { namespace internal {

/**
 * @brief A container which loads and stores typefonts with fallback option
 */
class FontManager {
    public:
        FontManager() = default;

        FontManager(const FontManager& other) = delete;
        FontManager& operator=(const FontManager& other) = delete;
        FontManager(FontManager&& other) = delete;
        FontManager& operator=(FontManager&& other) = delete;

        void set_font_path(std::string path) {m_base_path = path;}

        void clear();

        TTF_Font* get_font(std::string name, int pt_size);
    private:
        TTF_Font* get_fallback(int pt_size);

        std::string m_default_font = "OpenSans";
        std::string m_base_path = "../data/fonts/";
        std::map<std::string, std::map<int, TTF_Font*>> m_fonts;
};
}} // namespace salmon::internal

#endif // FONT_MANAGER_HPP_INCLUDED
