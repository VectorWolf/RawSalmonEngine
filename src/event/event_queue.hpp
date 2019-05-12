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
#ifndef EVENT_QUEUE_HPP_INCLUDED
#define EVENT_QUEUE_HPP_INCLUDED

#include <string>
#include <map>
#include <vector>

#include "event/event.hpp"

template<class Scope>
class EventQueue {
    public:
        EventQueue() = default;
        ~EventQueue();

        EventQueue(const EventQueue<Scope>& other);
        EventQueue& operator=(const EventQueue<Scope>& other);

        EventQueue(EventQueue<Scope>&& other) = default;
        EventQueue& operator=(EventQueue<Scope>&& other) = default;


        bool process_events(Scope& target);

        void add_event(Event<Scope>* event);
        unsigned scrap_event(std::string name, Event<Scope>* except = nullptr);

        void set_cooldown(std::string name, float dur_sec) {m_timestamp[name] = SDL_GetTicks() + dur_sec * 1000;}
        Uint32 get_cooldown(std::string name) const {return m_timestamp.at(name);}

        void block_event(std::string name) {m_block[name] = true;}
        void block_event(SDL_Keycode key) {m_block_key[key] = true;}

        void unblock_event(std::string name) {m_block[name] = false;}
        void unblock_event(SDL_Keycode key) {m_block_key[key] = false;}

        bool is_empty() const {return m_event_pipeline.empty();}

        bool is_blocked(std::string name) const;
        bool is_blocked(const SDL_Keysym& key) const;
        bool in_cooldown(std::string name) const;

    private:
        std::map<std::string, Uint32> m_timestamp; ///< Map holding timestamps for use as cooldown functionality
        std::map<std::string, bool> m_block; ///< Map determinig if the event pipeline is blocked for a specific event or event type
        std::map<SDL_Keycode, bool> m_block_key; ///< Map determinig if the event pipeline is blocked for a specific key
        std::vector<Event<Scope>*> m_event_pipeline; ///< Vector of current events to be processed

};

template<class Scope>
EventQueue<Scope>::~EventQueue() {
    for(Event<Scope>* event : m_event_pipeline) {
        delete event;
    }
}

template<class Scope>
EventQueue<Scope>& EventQueue<Scope>::operator=(const EventQueue<Scope>& other) {
    m_timestamp = other.m_timestamp;
    m_block = other.m_block;
    m_block_key = other.m_block_key;

    m_event_pipeline.clear();
    for(Event<Scope>* event : other.m_event_pipeline) {
        m_event_pipeline.push_back(event->clone());
    }
}

template<class Scope>
EventQueue<Scope>::EventQueue(const EventQueue<Scope>& other) {
    *this = other;
}

/**
 * @brief Process the event queue
 * @return a @c bool which indicates if the target should be erased/shutdown
 */
template<class Scope>
bool EventQueue<Scope>::process_events(Scope& target) {
    if(!is_empty()) {
        for(unsigned i = 0; i < m_event_pipeline.size(); i++) {
            Event<Scope>* event = m_event_pipeline[i];
            EventSignal signal = event->process(target);
            if(signal == EventSignal::stop) {
                break;
            }
            else if(signal == EventSignal::end || signal == EventSignal::abort) {
                delete m_event_pipeline[i];
                m_event_pipeline.erase(m_event_pipeline.begin() + i);
                i--;
            }
            else if(signal == EventSignal::erase) {return false;}
        }
    }
    return true;
}

/**
 * @brief Adds the event to the targets event queue
 * @param event The event to be added
 * @note The position where the event is added corresponds to its priority value
 */
template<class Scope>
void EventQueue<Scope>::add_event(Event<Scope>* event) {
    if(!is_blocked(event->get_type()) && !is_blocked(event->get_name())
       && !is_blocked(event->get_cause().get_key())
       && !in_cooldown(event->get_type()) && !in_cooldown(event->get_name())) {
        if(!is_empty()) {
            auto it = m_event_pipeline.end();
            do {
                --it;
                if((*it)->get_priority() >= event->get_priority()) {
                    ++it;
                    m_event_pipeline.insert(it, event);
                    return;
                }
            } while(it != m_event_pipeline.begin());
        }
        m_event_pipeline.insert(m_event_pipeline.begin(), event);
        return;
    }
    else {
        // If event can't be added, delete it!
        delete event;
    }

}

/**
 * @brief Deletes all events with given name or type except one
 * @param name The individual name or type of the event
 * @param except The event which shouldn't be deleted
 * @return the count of events which have been deleted
 */
template<class Scope>
unsigned EventQueue<Scope>::scrap_event(std::string name, Event<Scope>* except) {
    unsigned counter = 0;
    for(unsigned i = 0; i < m_event_pipeline.size(); i++) {
        if(( m_event_pipeline[i]->get_type() == name || m_event_pipeline[i]->get_name() == name) && m_event_pipeline[i] != except) {
            delete m_event_pipeline[i];
            m_event_pipeline.erase(m_event_pipeline.begin() + i);
            i--;
            counter++;
        }
    }
    return counter;
}
/**
 * @brief Return if event pipeline is blocked for a specific event
 * @param name Name of the event or event type
 * @return @c bool indicating if event or event type is currently blocked
 */
template<class Scope>
bool EventQueue<Scope>::is_blocked(std::string name) const {
    if(m_block.find(name) == m_block.end()) {
        return false;
    }
    else {
        return m_block.at(name);
    }
}

/**
 * @brief Return if event pipeline is blocked for a specific key
 * @param key The key which gets checked
 * @return @c bool indicating if key is currently blocked
 */
template<class Scope>
bool EventQueue<Scope>::is_blocked(const SDL_Keysym& key) const {
    if(m_block_key.find(key.sym) == m_block_key.end()) {
        return false;
    }
    else {
        return m_block_key.at(key.sym);
    }
}

/**
 * @brief Return if event pipeline is on cooldown for a specific event
 * @param name Name of the event or event type
 * @return @c bool indicating if event or event type is currently on cooldown
 */
template<class Scope>
bool EventQueue<Scope>::in_cooldown(std::string name) const {
    if(m_timestamp.find(name) == m_timestamp.end()) {
        return false;
    }
    if(m_timestamp.at(name) > SDL_GetTicks()) {
        return true;
    }
    else {return false;}
}


#endif // EVENT_QUEUE_HPP_INCLUDED
