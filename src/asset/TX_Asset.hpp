
/*
*	Target_Xplosion - A classic shoot'em up video game
*	Copyright (C) 2016  Luxon Jean-Pierre
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
*	website : gumichan01.kappatau.fr
*	mail : luxon.jean.pierre@gmail.com
*/

#ifndef XMLREADER_HPP_INCLUDED
#define XMLREADER_HPP_INCLUDED


#include <TinyXML/tinyxml2.h>
#include <LunatiX/LX_AABB.hpp>
#include <LunatiX/LX_Library.hpp>
#include <LunatiX/utils/utf8_string.hpp>

#include <vector>
#include <array>

const unsigned long LEVELS = 6;
const unsigned long NB_ITEMS = 6;
const unsigned long PLAYER_MISSILES = 5;
const unsigned long ENEMY_MISSILES = 6;
const unsigned long NB_ENEMIES = 104;
const unsigned long NB_XPLOSION = 4;
const unsigned long NB_SOUNDS = 5;
const unsigned long DEFAULT_TEXT_SIZE = 32;


namespace tinyxml2
{
class XMLElement;
class XMLHandle;
};

struct TX_Anima
{
    Uint32 delay;
    std::vector<LX_AABB> v;
};

class TX_Asset
{
    // Nodes
    const char * IMAGE_NODE_STR = "Image";
    const char * MUSIC_NODE_STR = "Music";
    const char * SOUND_NODE_STR = "Sound";
    const char * LEVEL_NODE_STR = "Level";
    const char * PLAYER_NODE_STR = "Player";
    const char * ITEM_NODE_STR = "Item";
    const char * MISSILE_NODE_STR = "Missile";
    const char * ENEMY_NODE_STR = "Enemy";
    const char * EXPLOSION_NODE_STR = "Explosion";
    const char * BACKGROUND_NODE_STR = "Background";
    const char * UNIT_NODE_STR = "Unit";
    const char * SPRITE_NODE_STR = "Sprite";
    const char * COORD_NODE_STR = "Coordinates";

    // Attributes
    const char * PATH_ATTR_STR = "path";
    const char * LEVEL_ATTR_STR = "level";
    const char * ID_ATTR_STR = "id";
    const char * DELAY_ATTR_STR = "delay";
    const char * FILENAME_ATTR_STR = "filename";
    const char * X_ATTR_STR = "x";
    const char * Y_ATTR_STR = "y";
    const char * W_ATTR_STR = "w";
    const char * H_ATTR_STR = "h";

    std::string xml_filename;
    std::string player_string;
    std::string player_shield_string;
    std::array<std::string,NB_ITEMS> items;
    std::array<std::string,PLAYER_MISSILES> player_missiles;
    std::array<std::string,ENEMY_MISSILES> enemy_missiles;
    std::array<std::string,NB_XPLOSION> explosions;
    std::array<TX_Anima*,NB_XPLOSION> coordinates;
    std::array<std::string,NB_ENEMIES> enemy_path;
    std::array<std::string,LEVELS> level_music;
    std::array<std::string,LEVELS> level_path;
    std::array<std::string,LEVELS> level_bg;
    std::array<std::string,NB_SOUNDS> sounds;

    TX_Asset();
    TX_Asset(TX_Asset&);
    TX_Asset(TX_Asset&&);
    TX_Asset& operator =(TX_Asset&);
    ~TX_Asset();

    tinyxml2::XMLElement * getRootElement(tinyxml2::XMLHandle *hdl);

    // Read the main elements
    int readImageElement(tinyxml2::XMLElement *image_element);
    int readMusicElement(tinyxml2::XMLElement *music_element);
    int readSoundElement(tinyxml2::XMLElement *sound_element);
    int readLevelElement(tinyxml2::XMLElement *level_element);

    // Read the sons of the main elements
    int readPlayerElement(tinyxml2::XMLElement *player_element,std::string path);
    int readItemElement(tinyxml2::XMLElement *item_element,std::string path);
    int readMissileElement(tinyxml2::XMLElement *missile_element,std::string path);
    int readEnemyElement(tinyxml2::XMLElement *enemy_element,std::string path);
    int readExplosionElement(tinyxml2::XMLElement *explosion_element,std::string path);
    int readCoordElement(tinyxml2::XMLElement *coord_element,TX_Anima& anima);
    int readBgElement(tinyxml2::XMLElement *bg_element,std::string path);

public:

    static void init(void);
    static TX_Asset * getInstance(void);
    static void destroy(void);

    int readXMLFile(const char *filename="config/asset.xml");

    std::string getPlayerFile(void) const;
    std::string getPlayerShieldFile(void) const;
    std::string getItemFile(unsigned int index) const;
    std::string getPlayerMissilesFile(unsigned int index) const;
    std::string getEnemyMissilesFile(unsigned int index) const;
    std::string getLevelMusic(unsigned int id) const;
    std::string getSound(unsigned int id) const;
    std::string getLevelPath(unsigned int id) const;
    std::string getLevelBg(unsigned int id) const;
    std::string getEnemySpriteFile(unsigned int id) const;
    std::string getExplosionSpriteFile(unsigned int id) const;
    const TX_Anima* getAnimation(unsigned int id) const;
    std::string getfileName() const;
};

#endif // XMLREADER_HPP_INCLUDED
