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
#define LEVELS 21
#define ENEMIES_SPRITES 200

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

    string * enemy_sprites_path;
    string * level_music;
    string * level_path;

    TX_Asset();
    ~TX_Asset();

    XMLElement * getRootElement(XMLHandle *hdl);

    // Read the main elements
    int readImageElement(XMLElement *image_element);
    int readMusicElement(XMLElement *music_element);
    int readLevelElement(XMLElement *level_element);

    // Read the sons of the main elements
    int readPlayerElement(XMLElement *player_element,string path);
    int readItemElement(XMLElement *item_element,string path);
    int readMissileElement(XMLElement *missile_element,string path);
    int readEnemyElement(XMLElement *enemy_element,string path);

public:

    static void init(void);
    static TX_Asset * getInstance(void);
    static void destroy(void);

    int reavxMLFile(const char *filename="config/asset.xml");

    const char * getPlayerFile(void);
    const char * getPlayerShieldFile(void);
    const string * getItemFiles(void);
    const string * getPlayerMissilesFiles(void);
    const string * getEnemyMissilesFiles(void);
    const char * getLevelMusic(int id);
    const char * getLevelPath(int id);
    const string * getEnemySpriteFiles(void);
};


#endif // XMLREADER_HPP_INCLUDED

