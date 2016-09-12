
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright (C) 2016  Luxon Jean-Pierre
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Luxon Jean-Pierre (Gumichan01)
*   website : gumichan01.kappatau.fr
*   mail : luxon.jean.pierre@gmail.com
*/

#include "VolumeHandler.hpp"
#include <LunatiX/LX_FileIO.hpp>
#include <LunatiX/LX_Mixer.hpp>

namespace Option
{

const unsigned short MAX_VOLUME = 100;

VolumeHandler::VolumeHandler()
    : updated(false),ov_volume(0),mus_volume(0),fx_volume(0)
{
    if(!loadOptFile())
    {
        ov_volume = LX_Mixer::getOverallVolume() / MAX_VOLUME;
        mus_volume = LX_Mixer::getMusicVolume() / MAX_VOLUME;
        fx_volume = LX_Mixer::getFXVolume() / MAX_VOLUME;
    }
}

VolumeHandler::~VolumeHandler()
{
    if(!saveOptFile())
    {
        /// @todo error on saveOptFile()
    }
}


bool VolumeHandler::loadOptFile()
{
    /// @todo load the option file (v.txconf)
    return false;
}

bool VolumeHandler::saveOptFile()
{
    /// @todo load the option file (v.txconf)
    return false;
}

void VolumeHandler::setOverallVolume(unsigned short nov)
{
    LX_Mixer::setOverallVolume(nov);
    ov_volume = LX_Mixer::getOverallVolume() / MAX_VOLUME;
}

void VolumeHandler::setMusicVolume(unsigned short nmuv)
{
    LX_Mixer::setMusicVolume(nmuv);
    mus_volume = LX_Mixer::getMusicVolume() / MAX_VOLUME;
}

void VolumeHandler::setFXVolume(unsigned short nfxv)
{
    LX_Mixer::setFXVolume(nfxv);
    fx_volume = LX_Mixer::getFXVolume() / MAX_VOLUME;
}


unsigned short VolumeHandler::getOverallVolume()
{
    return ov_volume;
}

unsigned short VolumeHandler::getMusicVolume()
{
    return mus_volume;
}

unsigned short VolumeHandler::getFXVolume()
{
    return fx_volume;
}

};

