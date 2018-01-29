#ifndef ACTOR_HPP_INCLUDED
#define ACTOR_HPP_INCLUDED
#include <string>
#include <vector>
#include <map>
#include "game_types.hpp"
#include "tinyxml2.h"

/**
 * @brief Parse, store and manage all actors
 *
 * @note Actors can only be constructed from parsed @c ActorTemplate
 */

class Actor{

    public:
        Actor(Uint16 tile_id);          ///< Construct actor from tile_id corresponding to an ActorTemplate
        Actor(ActorTemplate& templ);    ///< Construct actor from an ActorTemplate

        tinyxml2::XMLError init_actor(tinyxml2::XMLElement* source);
        bool update();
        void render(int x_cam, int y_cam) const;

        static tinyxml2::XMLError add_template(tinyxml2::XMLElement* source, Uint16 tile_id);
        static void add_animation(std::string name, AnimationType anim, Direction dir, Tile* tile);


    private:
        float m_x;
        float m_y;
        unsigned m_width;
        unsigned m_height;
        std::string m_name;
        float m_base_speed;
        // float m_current_speed;

        Behaviour m_AI;
        AnimationType m_anim_state = AnimationType::idle; ///< Currently active animation
        Direction m_direction; ///< Current direction facing
        SDL_Rect m_hitbox;
        std::map<AnimationType, std::map<Direction, Tile>> m_animations; ///< 2D Map which stores all animation tiles

        static std::map<std::string, ActorTemplate> m_templates; ///< List of all actor templates by name
        static std::map<Uint16, std::string> m_gid_to_temp_name; ///< List of actor template names by globa tile id

};


#endif // ACTOR_HPP_INCLUDED
