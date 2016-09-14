
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

#include "OptionHandler.hpp"
#include <LunatiX/LX_FileIO.hpp>
#include <LunatiX/LX_Mixer.hpp>
#include <LunatiX/LX_MessageBox.hpp>
#include <LunatiX/LX_Log.hpp>

#include <sstream>

namespace Option
{
const char * VOLUME_OPTION_FILE = "config/txo.txconf";

void writeDatum(LX_FileIO::LX_File& wf,void *v,size_t sz);
void stream(std::ostringstream& ss,unsigned short v);


void writeDatum(LX_FileIO::LX_File& wf,void *v,size_t sz)
{
    size_t written;
    const size_t WRITTEN_DATA_EXPECTED = 1;

    written = wf.write(v,sz,WRITTEN_DATA_EXPECTED);

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


OptionHandler::OptionHandler()
    : updated(false),ov_volume(0),mus_volume(0),fx_volume(0),fullscreen(0)
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

OptionHandler::~OptionHandler()
{
    if(updated)
    {
        if(!saveOptFile())
            LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,
                                "Cannot save options → %s:%d",__FILE__,__LINE__);
    }
}


bool OptionHandler::loadOptFile()
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

        if(rf.read(&fullscreen,sizeof(uint8_t),RDATA_EXPECTED) != RDATA_EXPECTED)
        {
            rf.close();
            throw LX_FileIO::IOException("Cannot get the fullscreen flag the option file");
        }

        if(rf.read(&tag,sizeof(int),RDATA_EXPECTED) != RDATA_EXPECTED)
        {
            rf.close();
            throw LX_FileIO::IOException("Cannot get the last tag from the option file");
        }

        rf.close();
    }
    catch(LX_FileIO::IOException& ioe)
    {
        return false;
    }

    return true;
}

bool OptionHandler::saveOptFile()
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

        writeDatum(wf,&ov_volume,sizeof(unsigned short));   // Write the overall volume
        writeDatum(wf,&mus_volume,sizeof(unsigned short));  // Write the music volume
        writeDatum(wf,&fx_volume,sizeof(unsigned short));   // Write the effect(FX) volume
        writeDatum(wf,&fullscreen,sizeof(uint8_t));         // fullscreen flag

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
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"Unknown error ↓");
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,e.what());
        LX_MSGBox::showMSG(LX_MSG_ERR,"Unknown error",e.what());
        throw e;
    }

    return true;
}

void OptionHandler::setOverallVolume(unsigned short nov)
{
    LX_Mixer::setOverallVolume(nov);
    ov_volume = nov > MAX_VOLUME ? MAX_VOLUME : nov;
    updated = true;
}

void OptionHandler::setMusicVolume(unsigned short nmuv)
{
    LX_Mixer::setMusicVolume(nmuv);
    mus_volume = nmuv > MAX_VOLUME ? MAX_VOLUME : nmuv;
    updated = true;
}

void OptionHandler::setFXVolume(unsigned short nfxv)
{
    LX_Mixer::setFXVolume(nfxv);
    fx_volume = nfxv > MAX_VOLUME ? MAX_VOLUME : nfxv;
    updated = true;
}

void OptionHandler::setFullscreenFlag(uint8_t flag)
{
    if(flag <= 1)
    {
        fullscreen = flag;
        updated = true;
    }
}


unsigned short OptionHandler::getOverallVolume() const
{
    return ov_volume;
}

unsigned short OptionHandler::getMusicVolume() const
{
    return mus_volume;
}

unsigned short OptionHandler::getFXVolume() const
{
    return fx_volume;
}

uint8_t OptionHandler::getFullscreenFlag() const
{
    return fullscreen;
}


std::string OptionHandler::stringOfOverallVolume() const
{
    std::ostringstream ss;
    stream(ss,getOverallVolume());
    return ss.str().c_str();
}

std::string OptionHandler::stringOfMusicVolume() const
{
    std::ostringstream ss;
    stream(ss,getMusicVolume());
    return ss.str().c_str();
}

std::string OptionHandler::stringOfFXVolume() const
{
    std::ostringstream ss;
    stream(ss,getFXVolume());
    return ss.str().c_str();
}

std::string OptionHandler::stringOfFullscreenFlag() const
{
    return fullscreen == 1 ? "Enabled": "Disabled";
}

};

