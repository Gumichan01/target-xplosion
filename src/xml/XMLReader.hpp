#ifndef XMLREADER_HPP_INCLUDED
#define XMLREADER_HPP_INCLUDED



/*
*	Target_Xplosion - The classic shoot'em up video game
*	Copyright (C) 2015  Luxon Jean-Pierre
*
*	This program is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*	Luxon Jean-Pierre (Gumichan01)
*	website : gumichan01.olympe.in
*	mail : luxon.jean.pierre@gmail.com
*/

/**
*	@file XMLReader.hpp
*	@brief The XML reader of Target Xplosion
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/
#include <iostream>
#include <TinyXML/tinyxml2.h>

#define NB_ITEMS 6
#define PLAYER_MISSILES 5
#define ENEMY_MISSILES 6
#define LEVELS 2

#define DEFAULT_TEXT_SIZE 32


namespace tinyxml2
{

class XMLElement;
class XMLHandle;

};


using namespace std;
using namespace tinyxml2;


class TX_Asset
{
    string player_string;
    string player_shield_string;

    string * items;
    string * player_missiles;
    string * enemy_missiles;

    //string * enemy_sprites_path;
    string * level_music;

    TX_Asset();
    ~TX_Asset();

    XMLElement * getRootElement(XMLHandle *hdl);
    int readImageElement(XMLElement *image_element);
    int readMusicElement(XMLElement *music_element);

public:

    static void init(void);
    static TX_Asset * getInstance(void);
    static void destroy(void);

    int readXMLFile(const char *filename="config/asset.xml");
    const char * loadLevelMusic(unsigned int level=0,char *str=NULL,const char *filename="config/asset.xml");

    const char * playerFile(void);
    const char * playerShieldFile(void);
    const std::string * itemsFiles(void);
    const std::string * playerMissilesFiles(void);
    const std::string * enemyMissilesFiles(void);
    const char * levelMusic(int id);
};


#endif // XMLREADER_HPP_INCLUDED

