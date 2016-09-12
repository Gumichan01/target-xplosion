
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
#include <LunatiX/LX_MessageBox.hpp>
#include <LunatiX/LX_Log.hpp>

namespace Option
{

const unsigned short MAX_VOLUME = 100;
const char * VOLUME_OPTION_FILE = "config/txo.txconf";

void writeDatum(LX_FileIO::LX_File& wf,unsigned short& v);


void writeDatum(LX_FileIO::LX_File& wf,unsigned short& v)
{
    size_t written;
    const size_t WRITTEN_DATA_EXPECTED = 1;

    written = wf.write(&v,sizeof(unsigned short),WRITTEN_DATA_EXPECTED);

    if(written != WRITTEN_DATA_EXPECTED)
    {
        wf.close();
        throw LX_FileIO::IOException("Cannot write data into the option file");
    }
}


VolumeHandler::VolumeHandler()
    : updated(false),ov_volume(0),mus_volume(0),fx_volume(0)
{
    if(!loadOptFile())
    {
        // Failure → it may append at first start because the file has not been created yet
        ov_volume = LX_Mixer::getOverallVolume();
        mus_volume = LX_Mixer::getMusicVolume();
        fx_volume = LX_Mixer::getFXVolume();
    }
    else
    {
        // Data loaded
        LX_Mixer::setOverallVolume(ov_volume);
        LX_Mixer::setMusicVolume(mus_volume);
        LX_Mixer::setFXVolume(fx_volume);
    }
}

VolumeHandler::~VolumeHandler()
{
    if(updated)
    {
        if(!saveOptFile())
            LX_MSGBox::showMSG(LX_MSG_ERR,"Volume options","Cannot save options");
    }
}


bool VolumeHandler::loadOptFile()
{
    /// @todo load the option file (v.txconf)
    return false;
}

bool VolumeHandler::saveOptFile()
{
    // Tag used to check the file
    int tag = 0xCF3A1;

    try
    {
        const size_t WDATA_EXPECTED = 1;
        LX_FileIO::LX_File wf(VOLUME_OPTION_FILE,LX_FILEIO_WRONLY);

        if(wf.write(&tag,sizeof(int),WDATA_EXPECTED) != WDATA_EXPECTED)
        {
            wf.close();
            throw LX_FileIO::IOException("Cannot write the tag into the option file");
        }

        writeDatum(wf,ov_volume);   // Write the overall volume
        writeDatum(wf,mus_volume);  // Write the music volume
        writeDatum(wf,fx_volume);   // Write the effect(FX) volume

        if(wf.write(&tag,sizeof(int),WDATA_EXPECTED) != WDATA_EXPECTED)
        {
            wf.close();
            throw LX_FileIO::IOException("Cannot write the tag into the option file(after closing)");
        }

        wf.close();
    }
    catch(LX_FileIO::IOException& ioe)
    {
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s",ioe.what());
        return false;
    }
    catch(std::exception& e)
    {
        LX_MSGBox::showMSG(LX_MSG_ERR,"Unknown error",e.what());
        throw e;
    }

    return false;
}

void VolumeHandler::setOverallVolume(unsigned short nov)
{
    LX_Mixer::setOverallVolume(nov);
    ov_volume = LX_Mixer::getOverallVolume();
    updated = true;
}

void VolumeHandler::setMusicVolume(unsigned short nmuv)
{
    LX_Mixer::setMusicVolume(nmuv);
    mus_volume = LX_Mixer::getMusicVolume();
    updated = true;
}

void VolumeHandler::setFXVolume(unsigned short nfxv)
{
    LX_Mixer::setFXVolume(nfxv);
    fx_volume = LX_Mixer::getFXVolume();
    updated = true;
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

