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
 #include "util/parse.hpp"

#include <iostream>
#include <tinyxml2.h>

#include "graphics/texture.hpp"
#include "util/logger.hpp"

namespace salmon { namespace internal {

 /**
 * @brief Converts the xmlelement to a proper Rect with checking
 * @param source The @c XMLElement which holds the information
 * @param rect The rect which gets produced
 * @return @c XMLError Indicating success or failure
 * @note Only the first hitbox gets parsed, multiple hitboxes lead to an error
 */
tinyxml2::XMLError parse::hitbox(tinyxml2::XMLElement* source, Rect& rect) {
    using namespace tinyxml2;
    XMLError eResult;

    if(source->FirstChildElement("ellipse") != nullptr) {
        Logger(Logger::error) << "Hitbox can't be an ellipse!";
        return XML_WRONG_ATTRIBUTE_TYPE;
    }
    if(source->FirstChildElement("polygon") != nullptr) {
        Logger(Logger::error) << "Hitbox can't be a polygon!";
        return XML_WRONG_ATTRIBUTE_TYPE;
    }
    if(source->FirstChildElement("polyline") != nullptr) {
        Logger(Logger::error) << "Hitbox can't be a polyline!";
        return XML_WRONG_ATTRIBUTE_TYPE;
    }
    if(source->NextSiblingElement("object") != nullptr) {
        Logger(Logger::error) << "Multiple hitboxes are not supported!";
        return XML_WRONG_ATTRIBUTE_TYPE;
    }
    Rect temp_rec;
    float temp;

    eResult = source->QueryFloatAttribute("x", &temp);
    if(eResult != XML_SUCCESS) return eResult;
    temp_rec.x = temp;

    eResult = source->QueryFloatAttribute("y", &temp);
    if(eResult != XML_SUCCESS) return eResult;
    temp_rec.y = temp;

    eResult = source->QueryFloatAttribute("width", &temp);
    if(eResult != XML_SUCCESS) return eResult;
    temp_rec.w = temp;

    eResult = source->QueryFloatAttribute("height", &temp);
    if(eResult != XML_SUCCESS) return eResult;
    temp_rec.h = temp;

    rect = temp_rec;
    return XML_SUCCESS;
}

/**
 * @brief Converts the xmlelement to multiple proper Rects with checking
 * @param source The @c XMLElement which holds the information
 * @param rects The rects which get produced
 * @return @c XMLError Indicating success or failure
 */
tinyxml2::XMLError parse::hitboxes(tinyxml2::XMLElement* source, std::map<std::string, Rect>& rects) {
    using namespace tinyxml2;
    XMLError eResult;

    while(source != nullptr) {
        if(source->FirstChildElement("ellipse") != nullptr) {
            Logger(Logger::error) << "Hitbox can't be an ellipse!";
            return XML_WRONG_ATTRIBUTE_TYPE;
        }
        if(source->FirstChildElement("polygon") != nullptr) {
            Logger(Logger::error) << "Hitbox can't be a polygon!";
            return XML_WRONG_ATTRIBUTE_TYPE;
        }
        if(source->FirstChildElement("polyline") != nullptr) {
            Logger(Logger::error) << "Hitbox can't be a polyline!";
            return XML_WRONG_ATTRIBUTE_TYPE;
        }

        Rect temp_rec;
        float temp;
        std::string name;
        const char* p_name = source->Attribute("name");
        if(p_name == nullptr) {name = DEFAULT_HITBOX;}
        else{name = p_name;}

        eResult = source->QueryFloatAttribute("x", &temp);
        if(eResult != XML_SUCCESS) return eResult;
        temp_rec.x = temp;

        eResult = source->QueryFloatAttribute("y", &temp);
        if(eResult != XML_SUCCESS) return eResult;
        temp_rec.y = temp;

        eResult = source->QueryFloatAttribute("width", &temp);
        if(eResult != XML_SUCCESS) return eResult;
        temp_rec.w = temp;

        eResult = source->QueryFloatAttribute("height", &temp);
        if(eResult != XML_SUCCESS) return eResult;
        temp_rec.h = temp;

        if(rects.find(name) != rects.end()) {
            Logger(Logger::error) << "Possible multiple definition of hitbox: " << name << " !";
            return XML_ERROR_PARSING_ATTRIBUTE;
        }

        rects[name] = temp_rec;

        source = source->NextSiblingElement("object");
    }
    return XML_SUCCESS;
}

/**
 * @brief Set the blendmode of a texture according to XML information
 * @param source The @c XMLElement which holds the information
 * @param img The texture which blendmode gets set
 * @return @c XMLError Indicating success or failure
 */
tinyxml2::XMLError parse::blendmode(tinyxml2::XMLElement* source, Texture& img) {
    using namespace tinyxml2;
    const char* p_mode;
    p_mode = source->Attribute("value");
    std::string mode(p_mode);
    if(p_mode == nullptr) return XML_ERROR_PARSING_ATTRIBUTE;
    else if(mode == "NONE") img.setBlendMode(SDL_BLENDMODE_NONE);
    else if(mode == "ALPHA") img.setBlendMode(SDL_BLENDMODE_BLEND);
    else if(mode == "ADD") img.setBlendMode(SDL_BLENDMODE_ADD);
    else if(mode == "COLOR") img.setBlendMode(SDL_BLENDMODE_MOD);
    else {
        Logger(Logger::error) << "Unknown blend mode: " << mode << " specified";
        return XML_ERROR_PARSING_ATTRIBUTE;
    }
    return XML_SUCCESS;
}

/**
 * @brief Parse the background color of a map to a variable
 * @param source The @c XMLElement which holds the information
 * @param color The referencte where the resulting color will be stored
 * @return @c CMLError Indicating sucess or failure
 */
tinyxml2::XMLError parse::bg_color(tinyxml2::XMLElement* source, SDL_Color& color) {
    using namespace tinyxml2;

    const char* p_bg_color;
    p_bg_color = source->Attribute("backgroundcolor");
    if(p_bg_color == nullptr) {
        Logger(Logger::warning) << "Map is missing a custom backgroundcolor, will use white as default";
        color = {255,255,255,255};

        return XML_ERROR_PARSING_ATTRIBUTE;
    }
    else {
        color = str_to_color(p_bg_color);
        return XML_SUCCESS;
    }
}

}} // namespace salmon::internal
