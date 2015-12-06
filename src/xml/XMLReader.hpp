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
#include <array>
#include <TinyXML/tinyxml2.h>

#define LEVELS 21
#define NB_ITEMS 6
#define PLAYER_MISSILES 5
#define ENEMY_MISSILES 6
#define ENEMY_SPRITES 200
#define DEFAULT_TEXT_SIZE 32


namespace tinyxml2
{
class XMLElement;
class XMLHandle;
};

using namespace tinyxml2;

class TX_Asset
{
    std::string player_string;
    std::string player_shield_string;
    std::array<std::string,NB_ITEMS> items;
    std::array<std::string,PLAYER_MISSILES> player_missiles;
    std::array<std::string,ENEMY_MISSILES> enemy_missiles;
    std::array<std::string,ENEMY_SPRITES> enemy_sprites_path;
    std::array<std::string,LEVELS> level_music;
    std::array<std::string,LEVELS> level_path;

    TX_Asset();
    ~TX_Asset();

    XMLElement * getRootElement(XMLHandle *hdl);

    // Read the main elements
    int readImageElement(XMLElement *image_element);
    int readMusicElement(XMLElement *music_element);
    int readLevelElement(XMLElement *level_element);

    // Read the sons of the main elements
    int readPlayerElement(XMLElement *player_element,std::string path);
    int readItemElement(XMLElement *item_element,std::string path);
    int readMissileElement(XMLElement *missile_element,std::string path);
    int readEnemyElement(XMLElement *enemy_element,std::string path);

public:

    static void init(void);
    static TX_Asset * getInstance(void);
    static void destroy(void);

    int readXMLFile(const char *filename="config/asset.xml");

    const char * getPlayerFile(void);
    const char * getPlayerShieldFile(void);
    std::string getItemFile(unsigned int index);
    std::string getPlayerMissilesFile(unsigned int index);
    std::string getEnemyMissilesFile(unsigned int index);
    const char * getLevelMusic(int id);
    const char * getLevelPath(unsigned int id);
    std::string getEnemySpriteFile(unsigned int id);
};

#endif // XMLREADER_HPP_INCLUDED

