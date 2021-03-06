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
#ifndef ACTOR_REF_HPP_INCLUDED
#define ACTOR_REF_HPP_INCLUDED

#include <string>
#include <vector>

#include "./types.hpp"
#include "./collision.hpp"
#include "./data_block.hpp"
#include "./transform.hpp"

namespace salmon {

namespace internal{class Actor;}

class Actor {
    friend class Camera;
    friend class MapData;
    public:
        Actor(internal::Actor& impl);
        Actor(internal::Actor* impl);
        virtual ~Actor() = default;

        /// Returns true if the actor exists / could be found
        bool good() const;

        /**
         * @brief Lets the actor play an animation sequence
         * @param anim The name of the type of animation to be played
         * @param dir The direction of the animation
         * @param speed A modifier for speed which even may be negative
         * @return true if animation wrapped around, false if not
         *
         * @note If animation type and direction combination doesn't exist, the error isn't indicated,
         *       and the animation stays at its prior state
         */
        bool animate(std::string anim = AnimationType::current, Direction dir = Direction::current, float speed = 1.0);

        /**
         * @brief Sets animation to specific frame
         * @param anim The name of the type of animation to be played
         * @param dir The direction of the animation
         * @param frame The number of frame which is set
         * @return false if animation type and direction combination doesn't exist or the frame is out of bounds, true otherwise
         *
         * @note If animation type and direction combination doesn't exist,
         *       the animation stays at its prior state
         */
        bool set_animation(std::string anim = AnimationType::current, Direction dir = Direction::current, int frame = 0);

        /**
         * @brief Lets the actor play an animation sequence
         * @param anim The name of the type of animation to be played
         * @param dir The direction of the animation
         * @param speed A modifier for speed which even may be negative
         * @return an AnimSignal value indicating missing animation, reaching next frame, wrap around, or hitting a trigger frame
         *
         * @note If animation type and direction combination doesn't exist,
         *       and the animation stays at its prior state
         */
        AnimSignal animate_trigger(std::string anim = AnimationType::current, Direction dir = Direction::current, float speed = 1.0);

        /// Returns the type string of the currently active animation
        std::string get_animation() const;
        /// Returns the direction value of the currently active animation
        Direction get_direction() const;
        /// Returns true if there is a valid animation tile representing animation type and direction
        bool valid_anim_state(std::string anim, Direction dir) const;

        /// Return reference to current transform of actor
        Transform& get_transform();

        /// Returns the name of the actor
        std::string get_name() const;
        /// Returns the name of the actor template from which the actor got constructed
        std::string get_template_name() const;
        /// Returns the unique id of the actor
        unsigned get_id() const;

        /**
         * @brief Moves relative to current position respecting potential collision
         * @param x, y The amount of movement in pixels
         * @param target Determine if actors, or tiles, or both are checked for collision
         * @param my_hitboxes A list of hitbox names for this actor to check with
         * @param other_hitboxes A list of hitbox names for collidees to check against
         * @param notify If true a collision may be generated and added to collider and collidee
         * @return false if movement was limited by a collision
         *
         * @note The resolution of collisions works better when moving relative in comparison to moving by absolute coordinates
         */
        bool move_relative(float x, float y, Collidees target, const std::vector<std::string>& my_hitboxes, const std::vector<std::string>& other_hitboxes, bool notify);

        /**
         * @brief Moves relative to the world origin respecting potential collision
         * @param x, y The amount of movement in pixels
         * @param target Determine if actors, or tiles, or both are checked for collision
         * @param my_hitboxes A list of hitbox names for this actor to check with
         * @param other_hitboxes A list of hitbox names for collidees to check against
         * @param notify If true a collision may be generated and added to collider and collidee
         * @return false if movement was limited by a collision
         *
         * @note The resolution of collisions works better when moving relative in comparison to moving by absolute coordinates
         */
        bool move_absolute(float x, float y, Collidees target, const std::vector<std::string>& my_hitboxes, const std::vector<std::string>& other_hitboxes, bool notify);

        /**
         * @brief Moves relative to current position with no collision check
         * @param x, y The amount of movement in pixels
         */
        void move_relative(float x, float y);
        /**
         * @brief Moves relative to the world origin with no collision check
         * @param x, y The amount of movement in pixels
         */
        void move_absolute(float x, float y);

        /**
         * @brief Separate this actor from all possible collidees
         * @param target Determines to check against tiles, actors or both
         * @param my_hitboxes A vector of hitbox names to check against for this actor
         * @param other_hitboxes A vector of hitbox names to check against for the collidee
         * @param notify If true a collision object will be added to all actors taking part in a collision
         * @return True if at least one collision happened/got resolved, false if there wasn't any collision
         */
        bool unstuck(Collidees target, const std::vector<std::string>& my_hitboxes, const std::vector<std::string>& other_hitboxes, bool notify);

        /**
         * @brief Separate this actor from all possible collidees by moving in a distinct direction
         * @param x, y The vector indicating the direction used for resolving collisions
         * @note see unstuck for other params
         */
        bool unstuck_along_path(float x, float y,Collidees target, const std::vector<std::string>& my_hitboxes, const std::vector<std::string>& other_hitboxes, bool notify);

        /**
         * @brief Test if this actor is colliding with another actor
         * @param other The other actor to test collision against
         * @param my_hitboxes A vector of hitbox names to check against for this actor
         * @param other_hitboxes A vector of hitbox names to check against for the collidee
         * @param notify If true a collision object will be added to both actors taking part in a collision
         * @return True if a collision occured, false otherwise
         */
        bool check_collision(Actor other, const std::vector<std::string>& my_hitboxes, const std::vector<std::string>& other_hitboxes, bool notify);

        /**
         * @brief Separate this actor from another actor
         * @param my_hitboxes A vector of hitbox names to check against for this actor
         * @param other_hitboxes A vector of hitbox names to check against for the collidee
         * @return True if at least one collision happened/got resolved, false if there wasn't any collision
         */
        bool separate(Actor actor, const std::vector<std::string>& my_hitboxes, const std::vector<std::string>& other_hitboxes);
        /**
         * @brief Separate this actor from another actor by moving along supplied direction vector
         * @param x, y The vector indicating the direction used for resolving collisions
         * @note see basic function for more information
         */
        bool separate(float x, float y, Actor actor, const std::vector<std::string>& my_hitboxes, const std::vector<std::string>& other_hitboxes);
        /**
         * @brief Separate two actors from each other by moving along two distinct vectors
         * @param x1, y1 The direction supplied for this actor
         * @param x2, y2 The direction supplied for the other actor
         * @note see basic function for more information
         */
        bool separate(float x1, float y1, float x2, float y2, Actor actor, const std::vector<std::string>& my_hitboxes, const std::vector<std::string>& other_hitboxes);

        /**
         * @brief Returns true if hitbox is touching other hitboxes of tiles or actors in a specific direction
         * @param target Determines to check collision against tiles, actors or both
         * @param my_hitbox The name of this actors hitbox to check against
         * @param other_hitboxes A list of hitbox names for others to check against
         * @param dir The direction of touch to check against
         * @param tolerance The maximum amount of pixels between the hitboxes to still count them as touching
         * @return true if there is at least one hitbox pair touching
         */
        bool on_ground(Collidees target, std::string my_hitbox, const std::vector<std::string>& other_hitboxes, Direction dir = Direction::down, int tolerance = 0) const;

        /// When set to true, hitboxes scale proportionally when the actor itself is scaled
        void set_resize_hitbox(bool mode);
        /// Returns if hitboxes scale with actor. True by default.
        bool get_resize_hitbox() const;

        /// Returns the number of the currently active animation frame
        int get_current_anim_frame() const;
        /// Returns the number of frames the currently active animation type direction combination has
        int get_anim_frame_count() const;

        /// Returns vector containing all collisions since last clear_collisions() call
        std::vector<Collision> get_collisions();
        /// Clears actor of its detected collisions
        void clear_collisions();

        /// When set to false, collisions won't be stored and directly discarded, when true they will be stored again
        void register_collisions(bool r);

        /// Returns true if actor is currently hidden, false otherwise
        bool get_hidden() const;
        /// When mode is true, rendering will be suspended, when false actor will be rendered again
        void set_hidden(bool mode);

        /// Returns the name of the layer in which the actor resides
        std::string get_layer() const;

        /// Returns the hitbox matching the supplied name. If there is none, w or h is zero.
        Rect get_hitbox(std::string name) const;

        /// Returns reference to DataBlock of this actor which holds all property values supplied via tiled
        DataBlock get_data();

    private:
        internal::Actor* m_impl;
};
}

#endif // ACTOR_REF_HPP_INCLUDED
