
/*
*   Target_Xplosion - A classic shoot'em up video game
<<<<<<< HEAD
*   Copyright © 2018 Luxon Jean-Pierre
=======
*   Copyright © 2017 Luxon Jean-Pierre
>>>>>>> dev
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
*   website: https://gumichan01.github.io/
*   mail: luxon.jean.pierre@gmail.com
*/

#include "OptionHandler.hpp"
#include <LunatiX/LX_FileIO.hpp>
#include <LunatiX/LX_Mixer.hpp>
#include <LunatiX/LX_MessageBox.hpp>
#include <LunatiX/LX_Log.hpp>

#include <sstream>

namespace Option
{
const int TXGEN_TAG = 0xCF3A1;
const std::string OPT_FILE("config/opt.txconf");

const std::string ENABLED("Enabled");
const std::string DISABLED("Disabled");

void writeDatum(LX_FileIO::LX_File& wf, void *v, size_t sz);
void stream(std::ostringstream& ss, unsigned short v) noexcept;


void writeDatum(LX_FileIO::LX_File& wf, void *v, size_t sz)
{
    size_t written;
    const size_t WRITTEN_DATA_EXPECTED = 1;

    written = wf.write(v, sz, WRITTEN_DATA_EXPECTED);

    if(written != WRITTEN_DATA_EXPECTED)
        throw LX_FileIO::IOException("Cannot write data into the option file");
}

void stream(std::ostringstream& ss, unsigned short v) noexcept
{
    if(v >= 100)
        ss << v;
    else if(v > 9)
        ss << " " << v;
    else
        ss << "  " << v;
}


OptionHandler::OptionHandler()
    : updated(false), ov_volume(0), mus_volume(0), fx_volume(0), fullscreen(0)
{
    if(!loadOptFile())
    {
        // Failure → it may append at first start because the file has not been created yet
        ov_volume = LX_Mixer::getOverallVolume();
        mus_volume = LX_Mixer::getMusicVolume();
        fx_volume = LX_Mixer::getFXVolume();

        if(mus_volume != 0)
            mus_volume = mus_volume * OPT_MAX_VOLUME / ov_volume;

        if(fx_volume != 0)
            fx_volume = fx_volume * OPT_MAX_VOLUME / ov_volume;

        updated = true;
    }
    // Data loaded
    LX_Mixer::setOverallVolume(ov_volume);
    LX_Mixer::setMusicVolume(mus_volume);
    LX_Mixer::setFXVolume(fx_volume);
}

OptionHandler::~OptionHandler()
{
    if(updated)
    {
        if(!saveOptFile())
            LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,
                                "Cannot save options → %s:%d", __FILE__, __LINE__);
    }
}


bool OptionHandler::loadOptFile() noexcept
{
    try
    {
        int tag = TXGEN_TAG;
        const size_t NBVOL = 3;
        const size_t RDATA_EXPECTED = 1;
        unsigned short volumes[3];

        LX_FileIO::LX_File rf(OPT_FILE, LX_FileIO::LX_FileMode::RDONLY);

        if(rf.read(&tag, sizeof(int), RDATA_EXPECTED) != RDATA_EXPECTED)
            throw LX_FileIO::IOException("Cannot get the first tag from the option file");

        if(rf.read(volumes, sizeof(unsigned short), NBVOL) != NBVOL)
            throw LX_FileIO::IOException("Cannot get data the option file");

        if(rf.read(&fullscreen, sizeof(uint8_t), RDATA_EXPECTED) != RDATA_EXPECTED)
            throw LX_FileIO::IOException("Cannot get the fullscreen flag from the option file");

        if(rf.read(&tag, sizeof(int), RDATA_EXPECTED) != RDATA_EXPECTED)
            throw LX_FileIO::IOException("Cannot get the last tag from the option file");

        ov_volume = volumes[0];
        mus_volume = volumes[1];
        fx_volume = volumes[2];
    }
    catch(LX_FileIO::IOException& e)
    {
        LX_Log::log("%s", e.what());
        return false;
    }

    return true;
}

bool OptionHandler::saveOptFile()
{
    try
    {
        int tag = TXGEN_TAG;
        const size_t WDATA_EXPECTED = 1;
        LX_FileIO::LX_File wf(OPT_FILE, LX_FileIO::LX_FileMode::WRONLY);

        if(wf.write(&tag, sizeof(int), WDATA_EXPECTED) != WDATA_EXPECTED)
            throw LX_FileIO::IOException("Cannot write the tag into the option file");

        writeDatum(wf, &ov_volume, sizeof(unsigned short));   // Write the overall volume
        writeDatum(wf, &mus_volume, sizeof(unsigned short));  // Write the music volume
        writeDatum(wf, &fx_volume, sizeof(unsigned short));   // Write the effect(FX) volume
        writeDatum(wf, &fullscreen, sizeof(uint8_t));         // fullscreen flag

        if(wf.write(&tag, sizeof(int), WDATA_EXPECTED) != WDATA_EXPECTED)
            throw LX_FileIO::IOException("Cannot write the tag into the option file(after closing)");
    }
    catch(LX_FileIO::IOException& ioe)
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,"%s", ioe.what());
        return false;
    }
    catch(std::exception& e)
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,"Unknown error ↓");
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION, e.what());
        LX_MSGBox::showMSG(LX_MSGBox::LX_MsgType::ERR,"Unknown error", e.what());
        throw;
    }

    return true;
}

void OptionHandler::setOverallVolume(unsigned short nov) noexcept
{
    LX_Mixer::setOverallVolume(nov);
    ov_volume = nov > OPT_MAX_VOLUME ? OPT_MAX_VOLUME : nov;
    updated = true;
}

void OptionHandler::setMusicVolume(unsigned short nmuv) noexcept
{
    LX_Mixer::setMusicVolume(nmuv);
    mus_volume = nmuv > OPT_MAX_VOLUME ? OPT_MAX_VOLUME : nmuv;
    updated = true;
}

void OptionHandler::setFXVolume(unsigned short nfxv) noexcept
{
    LX_Mixer::setFXVolume(nfxv);
    fx_volume = nfxv > OPT_MAX_VOLUME ? OPT_MAX_VOLUME : nfxv;
    updated = true;
}

void OptionHandler::setFullscreenFlag(uint8_t flag) noexcept
{
    if(flag <= 1)
    {
        fullscreen = flag;
        updated = true;
    }
}


unsigned short OptionHandler::getOverallVolume() const noexcept
{
    return ov_volume;
}

unsigned short OptionHandler::getMusicVolume() const noexcept
{
    return mus_volume;
}

unsigned short OptionHandler::getFXVolume() const noexcept
{
    return fx_volume;
}

uint8_t OptionHandler::getFullscreenFlag() const noexcept
{
    return fullscreen;
}


std::string OptionHandler::stringOfOverallVolume() const noexcept
{
    std::ostringstream ss;
    stream(ss, getOverallVolume());
    return ss.str();
}

std::string OptionHandler::stringOfMusicVolume() const noexcept
{
    std::ostringstream ss;
    stream(ss, getMusicVolume());
    return ss.str();
}

std::string OptionHandler::stringOfFXVolume() const noexcept
{
    std::ostringstream ss;
    stream(ss, getFXVolume());
    return ss.str();
}

std::string OptionHandler::stringOfFullscreenFlag() const noexcept
{
    return fullscreen == 1 ? ENABLED : DISABLED;
}

}
