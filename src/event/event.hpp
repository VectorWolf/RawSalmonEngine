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
#ifndef EVENT_HPP_INCLUDED
#define EVENT_HPP_INCLUDED

#include <iostream>
#include <string>
#include <map>

#include "actor/cause.hpp"
#include "util/game_types.hpp"
#include "util/tinyxml2.h"

class Actor;
class GameInfo;
class MapData;

template<class Scope>
class Event {
public:
    virtual tinyxml2::XMLError init(tinyxml2::XMLElement* source, MapData& base_map) = 0;
    virtual EventSignal process(Scope& entity) = 0;
    virtual std::string get_type() const = 0;
    virtual ~Event() = 0;

    std::string get_name() const {return m_name;}
    Priority get_priority() const {return m_priority;}
    EventSignal get_signal() const {return m_signal;}

    void set_cause(Cause x) {m_cause = x;}
    Cause get_cause() const {return m_cause;}

    // The actual implementation behaves like a static function, only is virtual for finding the right child class
    virtual Event<Scope>* create(tinyxml2::XMLElement* source, MapData& base_map) const = 0;

    static Event<Scope>* parse(tinyxml2::XMLElement* source, MapData& base_map);

protected:
    std::string m_name = "";
    Priority m_priority = Priority::medium;
    EventSignal m_signal = EventSignal::next;
    Cause m_cause;

    template <class T>
    static bool register_class();

private:

    static std::map<std::string, Event<Scope>>& get_dict();

    //static int initialize_lookup();

    // this baad boy won't be statically initialized ever because its a stl map
    // if i want to register classes themselves, I'll need to get some lazy initialization
    // and put the var as a static inside a getter
    //static std::map<std::string, Event<Scope>> m_event_dict;

    // m_size has to come last because its definition triggers the
    // redefinition of m_event_dict which would otherwise be overwritten
    // by the default definition
    //static int m_size;
};

template<class Scope>
std::map<std::string, Event<Scope>>& Event<Scope>::get_dict() {
    // Lazy initialization of event_dict
    // Can be called during dynamic initialization of static members
    // of other classes (events can finally register themselves before main!)
    static std::map<std::string, Event<Scope>> event_dict;
    return event_dict;
}

template<class Scope>
Event<Scope>::~Event() {}

/// Registers the type of event as an actor event
template <class Scope>
template <class T>
bool Event<Scope>::register_class() {
    get_dict()[T::get_type_static()] = T();
    return true;
}

template<class Scope>
Event<Scope>* Event<Scope>::parse(tinyxml2::XMLElement* source, MapData& base_map) {
    using namespace tinyxml2;

    const char* p_type;
    p_type = source->Attribute("type");

    std::string event_type;
    if(p_type != nullptr) {event_type = p_type;}
    else {
        return nullptr;
    }

    if(get_dict().find(event_type) == get_dict().end()) {
        std::cerr << "Error: Cant parse event of unknown type\n";
        return nullptr;
    }

    // Parse until first property element
    XMLElement* p_property = nullptr;
    XMLElement* p_properties = source->FirstChildElement("properties");
    if(p_properties != nullptr) {
        p_property = p_properties->FirstChildElement("property");
        if(p_property == nullptr) {
            std::cerr << "Error: Missing first property in event with id: " << source->Attribute("id") << "\n";
            return nullptr;
        }
    }
    else {
        std::cerr << "Error: Missing properties in event with id: " << source->Attribute("id") << "\n";
        return nullptr;
    }

    Event<Scope>* parsed_event = get_dict()[event_type].create(source, base_map);
    if(parsed_event == nullptr) {
        std::cerr << "Failed at parsing event with tile id: " << source->Attribute("id") << "\n";
        return nullptr;
    }
    return parsed_event;
}

/*
template <class Scope>
int Event<Scope>::initialize_lookup() {
    return 0;
}

template <>
int Event<Actor>::initialize_lookup() {
    int sum = 0;
    // sum += register_class<AeMove>();
    return sum;
}

template <>
int Event<MapData>::initialize_lookup() {
    int sum = 0;
    // sum += register_class< CLASS NAME >();
    return sum;
}

template <>
int Event<GameInfo>::initialize_lookup() {
    int sum = 0;
    // sum += register_class< CLASS NAME >();
    return sum;
}

template <class Scope>
int Event<Scope>::m_size = Event<Scope>::initialize_lookup();
*/


#endif // EVENT_HPP_INCLUDED
