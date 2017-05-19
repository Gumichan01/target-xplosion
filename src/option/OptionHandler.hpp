
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright © 2017  Luxon Jean-Pierre
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

#ifndef VOLUMEHANDLER_HPP_INCLUDED
#define VOLUMEHANDLER_HPP_INCLUDED

#include <string>
#include <cstdint>

namespace Option
{

const unsigned short MAX_VOLUME = 100;

class OptionHandler
{
    bool updated;
    unsigned short ov_volume;
    unsigned short mus_volume;
    unsigned short fx_volume;
    uint8_t fullscreen;

    OptionHandler(const OptionHandler&);
    OptionHandler& operator =(const OptionHandler&);

    bool loadOptFile();
    bool saveOptFile();

public:

    OptionHandler();

    void setOverallVolume(unsigned short nov);
    void setMusicVolume(unsigned short nmuv);
    void setFXVolume(unsigned short nfxv);
    void setFullscreenFlag(uint8_t flag);

    unsigned short getOverallVolume() const;
    unsigned short getMusicVolume() const;
    unsigned short getFXVolume() const;
    uint8_t getFullscreenFlag() const;

    std::string stringOfOverallVolume() const;
    std::string stringOfMusicVolume() const ;
    std::string stringOfFXVolume() const ;
    std::string stringOfFullscreenFlag() const;

    ~OptionHandler();
};

};

#endif // VOLUMEHANDLER_HPP_INCLUDED
