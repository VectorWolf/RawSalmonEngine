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
#ifndef TEXTURE_HPP_INCLUDED
#define TEXTURE_HPP_INCLUDED

#include <memory>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

namespace salmon { namespace internal {

/**
 * @brief Stores and manages an SDL Texture
 */
class Texture
{
	public:
		//Initializes variables
		Texture();

		bool valid() const;

		//Deallocates texture
		void free();

		//Loads image at specified path
		bool loadFromFile(SDL_Renderer* renderer, std::string path );
		// Loads color keyed image at specified path
		bool loadFromFile(SDL_Renderer* renderer, std::string path , SDL_Color color);

		//Creates image from font string
		bool loadFromRenderedText( SDL_Renderer* renderer, std::string textureText, SDL_Color textColor, TTF_Font *font, Uint32 wrap = 0);

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );

		//Renders texture at given point
		void render(int x, int y, const SDL_Rect* clip = nullptr) const;
		void render_resize(const SDL_Rect* clip, const SDL_Rect* dest) const;
		void render_extra(int x, int y, const SDL_Rect* clip, double angle, bool x_flip = false, bool y_flip = false, SDL_Point* center = nullptr) const;
		void render_extra_resize(const SDL_Rect* clip, const SDL_Rect* dest, double angle, bool x_flip = false, bool y_flip = false, SDL_Point* center = nullptr) const;

		//Gets image dimensions
		int getWidth() const;
		int getHeight() const;

        struct Deleter {
            void operator()(SDL_Texture* p) {
                if(p != nullptr) {SDL_DestroyTexture(p);}
            }
        };

	private:
		//The actual hardware texture
		std::shared_ptr<SDL_Texture> mTexture; ///< The actual hardware texture

		SDL_Renderer* mRenderer;

		//Image dimensions
		int mWidth;
		int mHeight;
};
}} // namespace salmon::internal

#endif // TEXTURE_HPP_INCLUDED
