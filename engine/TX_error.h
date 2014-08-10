#ifndef TX_ERROR_H_INCLUDED
#define TX_ERROR_H_INCLUDED



/*
*
*	Copyright (C)  Luxon Jean-Pierre
*	gumichan01.olympe.in
*
*
*	Luxon Jean-Pierre (Gumichan01)
*	luxon.jean.pierre@gmail.com
*
*/

/**
*	@file TX_error.h
*	@brief The TX_error library
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 1.0
*	@date July 26th, 2014
*
*
*/

// All error defined into the physics class
#define TX_LIST_POINT_ERROR -1  /**< The list of the polygon point could not be created */
#define TX_SDL_RECT_ERROR -2    /**< The SDL_Rect pointer could not be allocated */

// All error defined into the TX_graphics and TX_ttf class
#define TX_SDL_INIT_ERROR -10       /**< Error occured during the SDL subsystems initialization */
#define TX_IMG_INIT_ERROR -11       /**< Error occured during the SDL_Image subsystems initialization */
#define TX_SCREEN_ERROR -12         /**< Error occured during the screen mode setting */
#define TX_TTF_INIT_ERROR -13       /**< Error occured during the TTF engine initialization */
#define TX_FONT_ERROR -14           /**< Error occured during a file font processing */
#define TX_FONT_SCREEN_ERROR -15    /**< Error occured because no screen has been set */

// All error defined into the TX_Audio class
#define TX_AUDIO_INIT_ERROR -30     /**< Error occured during the SDL_Mixer libraries initialization */
#define TX_AUDIO_OPEN_ERROR -31     /**< Error occured during the mixer opening */

#endif // TX_ERROR_H_INCLUDED












