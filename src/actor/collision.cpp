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
#include "actor/collision.hpp"

#include "actor/actor.hpp"
#include "map/tile.hpp"

namespace salmon { namespace internal {

 // Default constructor
Collision::Collision() :
 type{CollisionType::none}
{

}

// Constructor for tile
Collision::Collision(TileInstance tile, std::string my_hitbox, std::string other_hitbox) :
 type{CollisionType::tile}, transform{tile.get_transform()}
{
    data.tile = tile.get_tile();
    my_hitbox_name = my_hitbox;
    other_hitbox_name = other_hitbox;

}

// Constructor for actor
Collision::Collision(Actor* actor, std::string my_hitbox, std::string other_hitbox) :
 type{CollisionType::actor}, transform{actor->get_transform()}
{
    data.actor = actor;
    my_hitbox_name = my_hitbox;
    other_hitbox_name = other_hitbox;
    actor_id = actor->get_id();
}

// Constructor for mouse
Collision::Collision(std::string my_hitbox) :
 type{CollisionType::mouse}
{
    my_hitbox_name = my_hitbox;
}

unsigned Collision::get_actor_id() const {return actor_id;}

TileInstance Collision::get_tile_instance() const {
    return {get_tile(),transform};
}

}} // namespace salmon::internal
