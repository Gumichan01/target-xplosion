
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright © 2018 Luxon Jean-Pierre
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
#include <Lunatix/FileIO.hpp>
#include <Lunatix/Mixer.hpp>
#include <Lunatix/MessageBox.hpp>
#include <Lunatix/Log.hpp>

#include <sstream>

namespace Option
{

const int TX_TAG = 0xCF3A1;
const std::string OPT_FILE( "config/opt.txconf" );

const std::string ENABLED( "Enabled" );
const std::string DISABLED( "Disabled" );

void writeDatum( lx::FileIO::File& wf, void * v, size_t sz );
std::ostringstream& stream( std::ostringstream& ss, unsigned short v ) noexcept;
std::string to_string( unsigned short v );

void writeDatum( lx::FileIO::File& wf, void * v, size_t sz )
{
    size_t written;
    const size_t WRITTEN_DATA_EXPECTED = 1;

    written = wf.write( v, sz, WRITTEN_DATA_EXPECTED );

    if ( written != WRITTEN_DATA_EXPECTED )
        throw lx::FileIO::IOException( "Cannot write data into the option file" );
}

std::ostringstream& stream( std::ostringstream& ss, unsigned short v ) noexcept
{
    if ( v >= 100 )
        ss << v;
    else if ( v > 9 )
        ss << " " << v;
    else
        ss << "  " << v;

    return ss;
}

std::string to_string( unsigned short v )
{
    std::ostringstream ss;
    return stream( ss, v ).str();
}

OptionHandler::OptionHandler()
    : updated( false ), ov_volume( 0 ), mus_volume( 0 ), fx_volume( 0 ), fullscreen( 0 )
{
    if ( !loadOptFile() )
    {
        // Failure → it may append at first start because the file has not been created yet
        ov_volume = lx::Mixer::getOverallVolume();
        mus_volume = lx::Mixer::getMusicVolume();
        fx_volume = lx::Mixer::getFXVolume();

        if ( mus_volume != 0 )
            mus_volume = mus_volume * OPT_MAX_VOLUME / ov_volume;

        if ( fx_volume != 0 )
            fx_volume = fx_volume * OPT_MAX_VOLUME / ov_volume;

        updated = true;
    }
    // Data loaded
    lx::Mixer::setOverallVolume( ov_volume );
    lx::Mixer::setMusicVolume( mus_volume );
    lx::Mixer::setFXVolume( fx_volume );
}

OptionHandler::~OptionHandler()
{
    if ( updated )
    {
        if ( !saveOptFile() )
            lx::Log::logCritical( lx::Log::LogType::APPLICATION,
                                 "Cannot save options → %s:%d", __FILE__, __LINE__ );
    }
}


bool OptionHandler::loadOptFile() noexcept
{
    try
    {
        int tag = TX_TAG;
        const size_t NBVOL = 3;
        const size_t RDATA_EXPECTED = 1;
        unsigned short volumes[3];

        lx::FileIO::File rf( OPT_FILE, lx::FileIO::FileMode::RDONLY );

        if ( rf.read( &tag, sizeof( int ), RDATA_EXPECTED ) != RDATA_EXPECTED )
            throw lx::FileIO::IOException( "Cannot get the first tag from the option file" );

        if ( rf.read( volumes, sizeof( unsigned short ), NBVOL ) != NBVOL )
            throw lx::FileIO::IOException( "Cannot get data the option file" );

        if ( rf.read( &fullscreen, sizeof( uint8_t ), RDATA_EXPECTED ) != RDATA_EXPECTED )
            throw lx::FileIO::IOException( "Cannot get the fullscreen flag from the option file" );

        if ( rf.read( &tag, sizeof( int ), RDATA_EXPECTED ) != RDATA_EXPECTED )
            throw lx::FileIO::IOException( "Cannot get the last tag from the option file" );

        ov_volume = volumes[0];
        mus_volume = volumes[1];
        fx_volume = volumes[2];
    }
    catch ( lx::FileIO::IOException& e )
    {
        lx::Log::log( "%s", e.what() );
        return false;
    }

    return true;
}

bool OptionHandler::saveOptFile()
{
    try
    {
        int tag = TX_TAG;
        const size_t WDATA_EXPECTED = 1;
        lx::FileIO::File wf( OPT_FILE, lx::FileIO::FileMode::WRONLY );

        if ( wf.write( &tag, sizeof( int ), WDATA_EXPECTED ) != WDATA_EXPECTED )
            throw lx::FileIO::IOException( "Cannot write the tag into the option file" );

        writeDatum( wf, &ov_volume, sizeof( unsigned short ) ); // Write the overall volume
        writeDatum( wf, &mus_volume, sizeof( unsigned short ) ); // Write the music volume
        writeDatum( wf, &fx_volume, sizeof( unsigned short ) ); // Write the effect(FX) volume
        writeDatum( wf, &fullscreen, sizeof( uint8_t ) );     // fullscreen flag

        if ( wf.write( &tag, sizeof( int ), WDATA_EXPECTED ) != WDATA_EXPECTED )
            throw lx::FileIO::IOException( "Cannot write the tag into the option file(after closing)" );
    }
    catch ( lx::FileIO::IOException& ioe )
    {
        lx::Log::logCritical( lx::Log::LogType::APPLICATION, "%s", ioe.what() );
        return false;
    }
    catch ( std::exception& e )
    {
        lx::Log::logCritical( lx::Log::LogType::APPLICATION, "Unknown error ↓" );
        lx::Log::logCritical( lx::Log::LogType::APPLICATION, e.what() );
        lx::MSGBox::showMSG( lx::MSGBox::MsgType::ERR, "Unknown error", e.what() );
        throw;
    }

    return true;
}

void OptionHandler::setOverallVolume( unsigned short nov ) noexcept
{
    lx::Mixer::setOverallVolume( nov );
    ov_volume = nov > OPT_MAX_VOLUME ? OPT_MAX_VOLUME : nov;
    updated = true;
}

void OptionHandler::setMusicVolume( unsigned short nmuv ) noexcept
{
    lx::Mixer::setMusicVolume( nmuv );
    mus_volume = nmuv > OPT_MAX_VOLUME ? OPT_MAX_VOLUME : nmuv;
    updated = true;
}

void OptionHandler::setFXVolume( unsigned short nfxv ) noexcept
{
    lx::Mixer::setFXVolume( nfxv );
    fx_volume = nfxv > OPT_MAX_VOLUME ? OPT_MAX_VOLUME : nfxv;
    updated = true;
}

void OptionHandler::setFullscreenFlag( uint8_t flag ) noexcept
{
    if ( flag <= 1 )
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
    return to_string( getOverallVolume() );
}

std::string OptionHandler::stringOfMusicVolume() const noexcept
{
    return to_string( getMusicVolume() );
}

std::string OptionHandler::stringOfFXVolume() const noexcept
{
    return to_string( getFXVolume() );
}

std::string OptionHandler::stringOfFullscreenFlag() const noexcept
{
    return fullscreen == 1 ? ENABLED : DISABLED;
}

}
