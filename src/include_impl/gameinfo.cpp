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
#include "gameinfo.hpp"

#include "core/gameinfo.hpp"

namespace salmon {

GameInfo::GameInfo() : m_impl{std::make_unique<internal::GameInfo>()} {}
GameInfo::~GameInfo() {}

void GameInfo::render() {m_impl->render();}
bool GameInfo::update() {return m_impl->update();}

bool GameInfo::load_map(std::string mapfile, bool absolute) {return m_impl->load_map(mapfile, absolute);}
void GameInfo::close_map() {m_impl->close_map();}
MapData GameInfo::get_map() {return m_impl->get_map();}

Window& GameInfo::get_window() {return m_impl->get_window();}

bool GameInfo::set_linear_filtering(bool mode) {return m_impl->set_linear_filtering(mode);}

void GameInfo::add_preload_directory(std::string dir) {
    m_impl->get_preloader().add_directory(m_impl->get_resource_path() + dir);
}
bool GameInfo::preload(float seconds) {
    return m_impl->get_preloader().load_recursive(static_cast<Uint32>(seconds * 1000));
}

AudioManager GameInfo::get_audio_manager() {return m_impl->get_audio_manager();}
DataBlock GameInfo::get_data() {return m_impl->get_data();}
InputCache GameInfo::get_input_cache() {return m_impl->get_input_cache();}

} // namespace salmon
