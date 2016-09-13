
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

#include <sstream>

namespace Option
{
const char * VOLUME_OPTION_FILE = "config/txo.txconf";
const std::string WARN_MSG = std::string("Some configuration files are missing.\n\n") +
                             std::string("This error happens when you start the game for the first time.\n") +
                             "The game can fix that so that you will not get this error.";

void writeDatum(LX_FileIO::LX_File& wf,unsigned short& v);
void stream(std::ostringstream& ss,unsigned short v);


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

void stream(std::ostringstream& ss,unsigned short v)
{
    if(v >= 100)
        ss << v;
    else if(v > 9)
        ss << " " << v;
    else
        ss << "  " << v;
}


VolumeHandler::VolumeHandler()
    : updated(false),ov_volume(0),mus_volume(0),fx_volume(0)
{
    if(!loadOptFile())
    {
        // Failure → it may append at first start because the file has not been created yet
        ov_volume = LX_Mixer::getOverallVolume();
        mus_volume = LX_Mixer::getMusicVolume();

        if(mus_volume != 0)
            mus_volume = mus_volume * MAX_VOLUME / ov_volume;

        fx_volume = LX_Mixer::getFXVolume();

        if(fx_volume != 0)
            fx_volume = fx_volume * MAX_VOLUME / ov_volume;

        updated = true;
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
    int tag = 0xCF3A1;

    try
    {
        const size_t NBVOL = 3;
        const size_t RDATA_EXPECTED = 1;
        unsigned short volumes[3];

        LX_FileIO::LX_File rf(VOLUME_OPTION_FILE,LX_FILEIO_RDONLY);

        if(rf.read(&tag,sizeof(int),RDATA_EXPECTED) != RDATA_EXPECTED)
        {
            rf.close();
            throw LX_FileIO::IOException("Cannot get the first tag from the option file");
        }

        if(rf.read(volumes,sizeof(unsigned short),NBVOL) != NBVOL)
        {
            rf.close();
            throw LX_FileIO::IOException("Cannot get data the option file");
        }

        ov_volume = volumes[0];
        mus_volume = volumes[1];
        fx_volume = volumes[2];

        if(rf.read(&tag,sizeof(int),RDATA_EXPECTED) != RDATA_EXPECTED)
        {
            rf.close();
            throw LX_FileIO::IOException("Cannot get the last tag from the option file");
        }

        rf.close();
    }
    catch(LX_FileIO::IOException& ioe)
    {
        LX_MSGBox::showMSG(LX_MSG_INFO,"Information",WARN_MSG);
        return false;
    }

    return true;
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

    return true;
}

void VolumeHandler::setOverallVolume(unsigned short nov)
{
    LX_Mixer::setOverallVolume(nov);
    ov_volume = nov > MAX_VOLUME ? MAX_VOLUME : nov;
    updated = true;
}

void VolumeHandler::setMusicVolume(unsigned short nmuv)
{
    LX_Mixer::setMusicVolume(nmuv);
    mus_volume = nmuv > MAX_VOLUME ? MAX_VOLUME : nmuv;
    updated = true;
}

void VolumeHandler::setFXVolume(unsigned short nfxv)
{
    LX_Mixer::setFXVolume(nfxv);
    fx_volume = nfxv > MAX_VOLUME ? MAX_VOLUME : nfxv;
    updated = true;
}


unsigned short VolumeHandler::getOverallVolume() const
{
    return ov_volume;
}

unsigned short VolumeHandler::getMusicVolume() const
{
    return mus_volume;
}

unsigned short VolumeHandler::getFXVolume() const
{
    return fx_volume;
}

std::string VolumeHandler::stringOfOverallVolume() const
{
    std::ostringstream ss;
    stream(ss,getOverallVolume());
    return ss.str().c_str();
}

std::string VolumeHandler::stringOfMusicVolume() const
{
    std::ostringstream ss;
    stream(ss,getMusicVolume());
    return ss.str().c_str();
}

std::string VolumeHandler::stringOfFXVolume() const
{
    std::ostringstream ss;
    stream(ss,getFXVolume());
    return ss.str().c_str();
}

};

