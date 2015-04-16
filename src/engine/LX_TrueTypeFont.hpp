#ifndef LX_TTF_H_INCLUDED
#define LX_TTF_H_INCLUDED



/*
*	Copyright (C) 2014-2015 Luxon Jean-Pierre
*	gumichan01.olympe.in
*
*	The LunatiX-engine is a SDL-based game engine.
*	It can be used for open-source or commercial games thanks to the zlib/libpng license.
*
*   Luxon Jean-Pierre (Gumichan01)
*	luxon.jean.pierre@gmail.com
*/

/**
*	@file LX_TrueTypeFont.h
*	@brief The LX_TrueTypeFont library
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 0.2
*	@date July 15th, 2014
*
*
*
*/


#include <SDL2/SDL_ttf.h>


struct SDL_Surface;
struct SDL_Texture;
struct SDL_Color;


#define LX_TTF_DEFAULT_FONT_SIZE 24    /**< The default value of the font size*/
#define LX_WHITE_COLOR 255
#define LX_BLACK_COLOR 0


/**
*   @enum LX_TTF_TypeText
*   @brief The LX_TTF_TypeText enumeration
*
*   This enumeration describes the type of the text you want to draw.
*
*/
typedef enum{LX_TTF_SOLID,LX_TTF_SHADED,LX_TTF_BLENDED} LX_TTF_TypeText;



/**
*   @class LX_TrueTypeFont
*   @brief The LunatiX_engine True type Font (TTF) module.
*
*   This class describes the LX_TrueTypeFont library. It manages the True type Font.
*
*   @note This class supports the UTF-8 format
*
*/
class LX_TrueTypeFont{

    std::string font_str;       /**< The font file */
    unsigned int font_size;     /**< The font size */
    SDL_Color font_color;       /**< The font color */


    void init(std::string font_file, SDL_Color *color, int size);
    SDL_Surface * drawText(LX_TTF_TypeText type, std::string text, Uint8 r, Uint8 g, Uint8 b, unsigned int size);

    public:

    LX_TrueTypeFont(SDL_Color *color);
    LX_TrueTypeFont(std::string font_file, SDL_Color *color);
    LX_TrueTypeFont(std::string font_file, SDL_Color *color, int size);

    int sizeOfText(std::string text, int *w, int *h);
    int sizeOfText(TTF_Font *ttf, std::string text, int *w, int *h);

    SDL_Surface * draw_SolidText(std::string text);
    SDL_Surface * draw_SolidText(std::string text, unsigned int size);

    SDL_Surface * draw_ShadedText(std::string text, SDL_Color bg);
    SDL_Surface * draw_ShadedText(std::string text, Uint8 r, Uint8 g, Uint8 b);
    SDL_Surface * draw_ShadedText(std::string text, Uint8 r, Uint8 g, Uint8 b, unsigned int size);

    SDL_Surface * draw_BlendedText(std::string text);
    SDL_Surface * draw_BlendedText(std::string text, unsigned int size);

    SDL_Texture * drawTextToTexture(LX_TTF_TypeText type,std::string text, unsigned int size);

    void setColor(SDL_Color *color);

    ~LX_TrueTypeFont();

};


#endif // LX_TTF_H_INCLUDED














