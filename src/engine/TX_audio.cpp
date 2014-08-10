

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
*	@file TX_Audio.cpp
*	@brief The Target_Xplosion sound engine
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 1.0
*	@date July 23th, 2014
*
*/

#include "TX_Audio.h"


/**
*   @fn bool TX_Audio::load_music(std::string filename)
*
*   Load the music specified in the music file
*
*   @param filename the file you want to open
*
*   @return TRUE if all is OK, FALSE otherwise
*
*/
bool TX_Audio::load_music(std::string filename)
{
    Mix_FreeMusic(music);

    music = Mix_LoadMUS(filename.c_str());

    if(music == NULL)
    {
        fprintf(stderr,"\nException occured in TX_Audio::load_music / Mix_LoadMUS :  %s \n", Mix_GetError());
        return false;
    }

    return true;
}


/**
*   @fn bool TX_Audio::play_music()
*
*   Play the music specified in the TX_Audio class
*
*   @return TRUE if all is OK, FALSE otherwise
*
*/
bool TX_Audio::play_music()
{
    int err;

    err = Mix_PlayMusic(music,TX_AUDIO_LOOP);

    if(err == -1)
    {
        fprintf(stderr,"\nException occured in TX_Audio::load_music / Mix_PlayMusic :  %s \n", Mix_GetError());
        return false;
    }

    return true;
}


/**
*   @fn void TX_Audio::pause_music()
*
*   Pause or resume the current music
*
*/
void TX_Audio::pause_music()
{
    if(Mix_PausedMusic())
    {
        Mix_ResumeMusic();
    }
    else
    {
        Mix_PauseMusic();
    }
}

void TX_Audio::stop_music()
{
    if(!Mix_PlayingMusic())
    {
        Mix_PlayMusic(music,-1);
    }
    else
    {
        Mix_HaltMusic();
    }
}





















