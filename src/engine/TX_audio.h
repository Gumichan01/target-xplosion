#ifndef TX_AUDIO_H_INCLUDED
#define TX_AUDIO_H_INCLUDED



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
*	@file TX_Audio.h
*	@brief The TX_Audio library, it will linked will the FMOD API
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 1.0
*	@date July 18th, 2014
*
*/


#include<cstdio>
#include<cstdlib>

#include<iostream>

#include<SDL/SDL.h>
#include<SDL/SDL_mixer.h>

#include "TX_error.h"

/// @todo Support of 2D sound effects

#define TX_AUDIO_STEREO_SOUND 2
#define TX_AUDIO_MONO_SOUND 1
#define TX_AUDIO_DEFAULT_CHUNKSIZE 1024

#define TX_AUDIO_LOOP -1

/**
*   @class TX_Audio
*   @brief The Target Xplosion sound engine
*
*   This FMOD-based sound engine will load and read a various audio files.
*
*
**/
class TX_Audio{

    Mix_Music *music;      /**<The music you want to play (note : TX_Audio only plays one music)*/


    public:

/**
*   @fn TX_Audio()
*
*   Load the TX_Audio library
*
**/
    TX_Audio()
    {

        int err = 0;

        err = Mix_Init(MIX_INIT_OGG);

        if(err == -1)
        {
            fprintf(stderr,"\nException occured in TX_Audio constructor, Mix_Init : %s \n", Mix_GetError());
            throw TX_AUDIO_INIT_ERROR;
        }

        err = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,MIX_DEFAULT_FORMAT,TX_AUDIO_STEREO_SOUND,TX_AUDIO_DEFAULT_CHUNKSIZE);

        if( err < 0)
        {
            fprintf(stderr,"\nException occured in TX_Audio constructor, Mix_OpenAudio : %s \n", Mix_GetError());
            throw TX_AUDIO_OPEN_ERROR;
        }

        music = NULL;

    }

    bool load_music(std::string filename);
    bool play_music();
    void pause_music();
    void stop_music();

    /* [TYPE] load_sample(std::string filename);
    [TYPE] read_sample([TYPE] *sample); */


    ~TX_Audio()
    {
        Mix_FreeMusic(music);
        Mix_CloseAudio();
        Mix_Quit();
    }

};

#endif // TX_AUDIO_H_INCLUDED



















