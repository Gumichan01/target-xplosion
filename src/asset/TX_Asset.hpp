
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

#ifndef XMLREADER_HPP_INCLUDED
#define XMLREADER_HPP_INCLUDED

#include <LunatiX/LX_AABB.hpp>
#include <LunatiX/LX_Error.hpp>
#include <TinyXML/tinyxml2.h>

#include <sstream>
#include <vector>
#include <array>

const unsigned long LEVELS = 6;
const unsigned long MUSICS = 12;
const unsigned long NB_ITEMS = 6;
const unsigned long PLAYER_MISSILES = 5;
const unsigned long NB_MISSILES = PLAYER_MISSILES + 6;
const unsigned long NB_ENEMIES = 111;
const unsigned long NB_XPLOSION = 10;
const unsigned long NB_SOUNDS = 15;
const unsigned long NB_MENU_IMG = 8;
const unsigned long DEFAULT_TEXT_SIZE = 32;


namespace tinyxml2
{
class XMLElement;
class XMLHandle;
}

struct TX_Anima
{
    uint32_t delay;
    std::vector<LX_AABB> v;
};

struct TX_ParallaxAsset
{
    std::string parallax01_bg;
    std::string parallax02_bg;
    std::string parallax03_bg;
};

class TX_Asset
{
    // Nodes
    static const char * ROOT_NODE_STR;
    static const char * FONT_NODE_STR;
    static const char * IMAGE_NODE_STR;
    static const char * MUSIC_NODE_STR;
    static const char * SOUND_NODE_STR;
    static const char * LEVEL_NODE_STR;
    static const char * PLAYER_NODE_STR;
    static const char * ITEM_NODE_STR;
    static const char * MISSILE_NODE_STR;
    static const char * ENEMY_NODE_STR;
    static const char * EXPLOSION_NODE_STR;
    static const char * BACKGROUND_NODE_STR;
    static const char * UNIT_NODE_STR;
    static const char * BGI_NODE_STR;
    static const char * PARALLAX_NODE_STR;
    static const char * SPRITE_NODE_STR;
    static const char * COORD_NODE_STR;
    static const char * MENU_NODE_STR;

    // Attributes
    static const char * PATH_ATTR_STR;
    static const char * LEVEL_ATTR_STR;
    static const char * ID_ATTR_STR;
    static const char * DELAY_ATTR_STR;
    static const char * FILENAME_ATTR_STR;
    static const char * PARALLAX_ATTR_STR;
    static const char * PARALLAX_YES_STR;
    static const char * X_ATTR_STR;
    static const char * Y_ATTR_STR;
    static const char * W_ATTR_STR;
    static const char * H_ATTR_STR;

    static const unsigned long NB_PARALLAX = 3;
    const std::string xml_filename = "config/asset.xml";
    // Player
    std::string font_file;
    std::string player_string;
    std::string player_shield_string;
    std::array<std::string, NB_ITEMS> items;
    // Missiles
    std::array<std::string, NB_MISSILES> missiles;
    std::array<TX_Anima*, NB_MISSILES> missile_coord;
    // Explosion
    std::array<std::string, NB_XPLOSION> explosions;
    std::array<TX_Anima*, NB_XPLOSION> coordinates;
    // Enemies
    std::array<std::string, NB_ENEMIES> enemy_path;
    std::array<TX_Anima*, NB_ENEMIES> enemy_coord;
    // Level (music, path, baskground)
    std::array<std::string, MUSICS> level_music;
    std::array<std::string, LEVELS> level_path;
    std::array<std::string, LEVELS> level_bg;
    std::array<TX_ParallaxAsset*, LEVELS> parallax;
    std::array<std::string, NB_SOUNDS> sounds;
    // Menu
    std::array<std::string, NB_MENU_IMG> menu_img;

    TX_Asset();
    TX_Asset(TX_Asset&);
    TX_Asset(TX_Asset&&);
    TX_Asset& operator =(TX_Asset&);
    ~TX_Asset();

    tinyxml2::XMLElement * getRootElement(tinyxml2::XMLHandle *hdl);

    static int readCoordElement(tinyxml2::XMLElement *coord_element,
                                TX_Anima& anima);

    template<typename T> static void initArray(T& ar);
    template<typename T> static void cleanArray(T& ar);

    // Read the main elements
    int readFontElement(tinyxml2::XMLElement *font_element);
    int readImageElement(tinyxml2::XMLElement *image_element);
    int readMusicElement(tinyxml2::XMLElement *music_element);
    int readSoundElement(tinyxml2::XMLElement *sound_element);
    int readLevelElement(tinyxml2::XMLElement *level_element);

    // Read the sons of the main elements
    int readPlayerElement(tinyxml2::XMLElement *player_element, const std::string& path);
    int readItemElement(tinyxml2::XMLElement *item_element, const std::string& path);
    int readMissileElement(tinyxml2::XMLElement *missile_element, const std::string& path);
    int readEnemyElement(tinyxml2::XMLElement *enemy_element, const std::string& path);
    int readExplosionElement(tinyxml2::XMLElement *explosion_element, const std::string& path);
    int readBgElement(tinyxml2::XMLElement *bg_element, const std::string& path);
    int readParallaxElement(tinyxml2::XMLElement *para_element, const std::string& path,
                            size_t lvl_index);
    int readMenuElement(tinyxml2::XMLElement *menu_element, const std::string& path);

    template<typename T, typename U>
    static readElements_(tinyxml2::XMLElement *elements,
                             T& elem_array, U& coord_array,
                             std::string path);
    template<typename T>
    int readUI_(tinyxml2::XMLElement *elements,
                       T& elem_array, const std::string& path, const char *node = UNIT_NODE_STR);

public:

    static void init();
    static TX_Asset * getInstance();
    static void destroy();

    int readXMLFile();

    const std::string getFontFile() const;
    const std::string getPlayerFile() const;
    const std::string getPlayerShieldFile() const;
    const std::string getItemFile(unsigned int index) const;
    const std::string getMissileFile(unsigned int index) const;
    const std::string getLevelMusic(unsigned int id) const;
    const std::string getSound(unsigned int id) const;
    const std::string getLevelPath(unsigned int id) const;
    const std::string getLevelBg(unsigned int id) const;
    const TX_ParallaxAsset *getLevelParallax(unsigned int id) const;
    const std::string getEnemySpriteFile(unsigned int id) const;
    const std::string getExplosionSpriteFile(unsigned int id) const;
    const std::string getMenuImgFile(unsigned int id) const;
    const std::string getfileName() const;

    const TX_Anima* getExplosionAnimation(unsigned int id) const;
    const TX_Anima* getEnemyAnimation(unsigned int id) const;
    const TX_Anima* getMissileAnimation(unsigned int id) const;
};

#include "TX_Asset.tpp"

#endif // XMLREADER_HPP_INCLUDED
