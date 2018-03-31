
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

#include "TX_Asset.hpp"

#include <LunatiX/LX_FileSystem.hpp>

using namespace std;
using namespace tinyxml2;
using namespace Asset;

static TX_Asset *tx_singleton = nullptr;
// Nodes
const char * TX_Asset::ROOT_NODE_STR = "Asset";
const char * TX_Asset::FONT_NODE_STR = "Font";
const char * TX_Asset::IMAGE_NODE_STR = "Image";
const char * TX_Asset::MUSIC_NODE_STR = "Music";
const char * TX_Asset::SOUND_NODE_STR = "Sound";
const char * TX_Asset::LEVEL_NODE_STR = "Level";
const char * TX_Asset::PLAYER_NODE_STR = "Player";
const char * TX_Asset::ITEM_NODE_STR = "Item";
const char * TX_Asset::MISSILE_NODE_STR = "Missile";
const char * TX_Asset::ENEMY_NODE_STR = "Enemy";
const char * TX_Asset::EXPLOSION_NODE_STR = "Explosion";
const char * TX_Asset::BACKGROUND_NODE_STR = "Background";
const char * TX_Asset::UNIT_NODE_STR = "Unit";
const char * TX_Asset::BGI_NODE_STR = "BGImage";
const char * TX_Asset::PARALLAX_NODE_STR = "Parallax";
const char * TX_Asset::SPRITE_NODE_STR = "Sprite";
const char * TX_Asset::COORD_NODE_STR = "Coordinates";
const char * TX_Asset::MENU_NODE_STR = "Menu";
// Path
const char * TX_Asset::PATH_ATTR_STR = "path";
const char * TX_Asset::LEVEL_ATTR_STR = "level";
const char * TX_Asset::ID_ATTR_STR = "id";
const char * TX_Asset::DELAY_ATTR_STR = "delay";
const char * TX_Asset::FILENAME_ATTR_STR = "filename";
const char * TX_Asset::PARALLAX_ATTR_STR = "parallax";
const char * TX_Asset::PARALLAX_YES_STR = "yes";
const char * TX_Asset::X_ATTR_STR = "x";
const char * TX_Asset::Y_ATTR_STR = "y";
const char * TX_Asset::W_ATTR_STR = "w";
const char * TX_Asset::H_ATTR_STR = "h";
// File extension
const char * TTF_EXT = ".ttf";


TX_Asset::TX_Asset()
{
    initArray(missile_coord);
    initArray(coordinates);
    initArray(enemy_coord);
    initArray(parallax);
}

TX_Asset::~TX_Asset()
{
    cleanArray(missile_coord);
    cleanArray(coordinates);
    cleanArray(enemy_coord);
    cleanArray(parallax);
}

void TX_Asset::init()
{
    if(tx_singleton == nullptr)
        tx_singleton = new TX_Asset();
}

void TX_Asset::destroy() noexcept
{
    delete tx_singleton;
}

TX_Asset * TX_Asset::getInstance() noexcept
{
    return tx_singleton;
}

const string TX_Asset::getFontFile() const noexcept
{
    return font_file;
}

const string TX_Asset::getPlayerFile() const noexcept
{
    return player_string;
}

const string TX_Asset::getPlayerShieldFile() const noexcept
{
    return player_shield_string;
}

const string TX_Asset::getItemFile(unsigned int index) const noexcept
{
    return items[index];
}

const string TX_Asset::getMissileFile(unsigned int index) const noexcept
{
    return missiles[index];
}

const string TX_Asset::getMenuImgFile(unsigned int id) const noexcept
{
    return menu_img[id];
}

// Get the root element of the XML file
XMLElement * TX_Asset::getRootElement(XMLHandle& hdl) const noexcept
{
    return hdl.FirstChildElement(ROOT_NODE_STR).ToElement();
}

// Returns the path of a music file according to the id of the level
const string TX_Asset::getLevelMusic(unsigned int id) const noexcept
{
    return level_music.at(id);
}

const string TX_Asset::getSound(unsigned int id) const noexcept
{
    return sounds.at(id);
}

const string TX_Asset::getLevelPath(unsigned int id) const noexcept
{
    return level_path.at(id);
}

const string TX_Asset::getLevelBg(unsigned int id) const noexcept
{
    return level_bg.at(id);
}

const TX_ParallaxAsset * TX_Asset::getLevelParallax(unsigned int id) const noexcept
{
    return parallax.at(id);
}

// Get the list of file path to the sprites of enemies
const string TX_Asset::getEnemySpriteFile(unsigned int id) const noexcept
{
    return enemy_path.at(id);
}

const string TX_Asset::getExplosionSpriteFile(unsigned int id) const noexcept
{
    return explosions.at(id);
}

const TX_Anima* TX_Asset::getExplosionAnimation(unsigned int id) const noexcept
{
    return coordinates.at(id);
}

const TX_Anima* TX_Asset::getEnemyAnimation(unsigned int id) const noexcept
{
    return id > enemy_coord.size() ? nullptr : enemy_coord.at(id);
}

const TX_Anima* TX_Asset::getMissileAnimation(unsigned int id) const noexcept
{
    return missile_coord.at(id);
}

const string TX_Asset::getfileName() const noexcept
{
    return XML_FILENAME;
}


unsigned int TX_Asset::getID(const UTF8string& name) const noexcept
{
    const auto NPOS  = std::string::npos;
    const unsigned int ERRID = static_cast<unsigned int>(-1);
    const std::string filename(LX_FileSystem::basename(name).utf8_str());

    size_t i = 0;

    while(i < enemy_path.size() && enemy_path[i].find(filename) == NPOS)
    {
        ++i;
    }

    return i < enemy_path.size() ? i : ERRID;
}

// Read and extract data from an XML file
int TX_Asset::readXMLFile() noexcept
{
    XMLDocument doc;
    XMLHandle hdl(&doc);
    XMLElement *tx, *elem;
    XMLError err;

    LX_Log::logDebug(LX_Log::LX_LogType::APPLICATION,"asset — open %s", XML_FILENAME.c_str());
    err = doc.LoadFile(XML_FILENAME.c_str());

    if(err != XML_SUCCESS)
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,"readXMLFile: error #%d: %s",
                            err, doc.ErrorName());
        return static_cast<int>(err);
    }

    // Get the root element
    if((tx = getRootElement(hdl)) == nullptr)
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,
                            "readXMLFile: Invalid element - expected : TX_asset");
        return static_cast<int>(XML_ERROR_PARSING_ELEMENT);
    }

    // Get The Font element
    elem = tx->FirstChildElement(FONT_NODE_STR);

    if(elem == nullptr)
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,
                            "readXMLFile: Invalid element - expected : Font");
        return static_cast<int>(XML_ERROR_PARSING_ELEMENT);
    }

    return readFontElement(elem);
}


int TX_Asset::readFontElement(XMLElement *font_element) noexcept
{
    string path, filename;
    XMLElement *unit_element;

    LX_Log::logDebug(LX_Log::LX_LogType::APPLICATION,"asset — font file");
    // Get the path attribute of Image
    path = font_element->Attribute(PATH_ATTR_STR);

    if(path.empty())
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,
                            "readFontElement: No attribute - expected : path");
        return static_cast<int>(XML_NO_ATTRIBUTE);
    }

    unit_element = font_element->FirstChildElement(UNIT_NODE_STR);

    if(unit_element == nullptr)
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,
                            "readFontElement: Invalid element - expected : Unit");
        return static_cast<int>(XML_ERROR_PARSING_ELEMENT);
    }

    filename = unit_element->Attribute(FILENAME_ATTR_STR);

    if(filename.empty())
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,
                            "readFontElement: No attribute or empty value");
        return static_cast<int>(XML_NO_ATTRIBUTE);
    }

    size_t pos = filename.find(TTF_EXT);

    if(pos == string::npos || filename.substr(pos) != TTF_EXT)
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,
                            "readFontElement: Bad attribute type - expected : %s",
                            TTF_EXT);
        return static_cast<int>(XML_WRONG_ATTRIBUTE_TYPE);
    }

    font_file = path + filename;
    LX_Log::logDebug(LX_Log::LX_LogType::APPLICATION,"asset — font file: %s", font_file.c_str());

    // Get The Image element
    XMLElement * elem = font_element->NextSiblingElement(IMAGE_NODE_STR);

    if(elem == nullptr)
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,
                            "readXMLFile: Invalid element - expected : Image");
        return static_cast<int>(XML_ERROR_PARSING_ELEMENT);
    }

    // Extract information about images
    return readImageElement(elem);
}

int TX_Asset::readImageElement(XMLElement *image_element) noexcept
{
    XMLElement *player_element;

    LX_Log::logDebug(LX_Log::LX_LogType::APPLICATION,"asset — get images");

    // Get the path attribute of Image
    string path = image_element->Attribute(PATH_ATTR_STR);

    if(path.empty())
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,
                            "readImageElement: Invalid attribute - expected : path");
        return static_cast<int>(XML_WRONG_ATTRIBUTE_TYPE);
    }

    /*
        Get the elements to load the sprites (player, items, missiles, ...)
    */
    player_element = image_element->FirstChildElement(PLAYER_NODE_STR);

    if(player_element == nullptr)
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,
                            "readImageElement: Invalid element - expected : Player");
        return static_cast<int>(XML_ERROR_PARSING_ELEMENT);
    }

    XMLElement * const pelem = player_element;
    int result = readPlayerElement(player_element, path);
    int result2 = readOtherSiblings(pelem, path);

    if(result + result2 != 0)
        return -1;

    XMLElement * elem = image_element->NextSiblingElement(MUSIC_NODE_STR);

    if(elem == nullptr)
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,
                            "readXMLFile: Invalid element - expected : Music");
        return static_cast<int>(XML_ERROR_PARSING_ELEMENT);
    }

    // Extract information about musics
    return readMusicElement(elem);
}

int TX_Asset::readOtherSiblings(XMLElement * const pelem, const std::string& path) noexcept
{
    XMLElement * enemy_element = pelem->NextSiblingElement(ENEMY_NODE_STR);

    if(enemy_element == nullptr)
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,
                            "readImageElement: Invalid element - expected : Enemy");
        return static_cast<int>(XML_ERROR_PARSING_ELEMENT);
    }

    XMLElement * explosion_element = pelem->NextSiblingElement(EXPLOSION_NODE_STR);

    if(explosion_element == nullptr)
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,
                            "Invalid element - expected : Explosion");
        return static_cast<int>(XML_ERROR_PARSING_ELEMENT);
    }

    XMLElement * bg_element = pelem->NextSiblingElement(BACKGROUND_NODE_STR);

    if(bg_element == nullptr)
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,
                            "Invalid element - expected : Background");
        return static_cast<int>(XML_ERROR_PARSING_ELEMENT);
    }

    XMLElement * menu_element = bg_element->NextSiblingElement(MENU_NODE_STR);

    if(menu_element == nullptr)
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,
                            "Invalid element - expected : Menu");
        return static_cast<int>(XML_ERROR_PARSING_ELEMENT);
    }

    // Get returned values
    return readEnemyElement(enemy_element, path) +
           readExplosionElement(explosion_element, path) +
           readBgElement(bg_element, path) + readMenuElement(menu_element, path);
}


int TX_Asset::readMusicElement(XMLElement *music_element) noexcept
{
    string lvl;
    string path;
    XMLElement *unit_element;

    LX_Log::logDebug(LX_Log::LX_LogType::APPLICATION,"asset — music");
    path = music_element->Attribute(PATH_ATTR_STR);    // Music path

    if(path.empty())
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,
                            "readMusicElement: Invalid attribute - expected : path");
        return static_cast<int>(XML_WRONG_ATTRIBUTE_TYPE);
    }

    unit_element = music_element->FirstChildElement(UNIT_NODE_STR);

    if(unit_element == nullptr)
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,
                            "readMusicElement: Invalid element - expected : Unit");
        return static_cast<int>(XML_ERROR_PARSING_ELEMENT);
    }

    while(unit_element != nullptr && unit_element->Attribute(ID_ATTR_STR) != nullptr)
    {
        unsigned id;
        lvl = unit_element->Attribute(ID_ATTR_STR);
        XMLUtil::ToUnsigned(lvl.c_str(), &id);

        level_music[id] = path + unit_element->Attribute(FILENAME_ATTR_STR);

        LX_Log::logDebug(LX_Log::LX_LogType::APPLICATION,"asset — music#%u: %s", id,
                         level_music[id].c_str());

        unit_element = unit_element->NextSiblingElement(UNIT_NODE_STR);
    }

    XMLElement * elem = music_element->NextSiblingElement(SOUND_NODE_STR);

    if(elem == nullptr)
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,
                            "readXMLFile: Invalid element - expected : Sound");
        return static_cast<int>(XML_ERROR_PARSING_ELEMENT);
    }

    // Extract information about sounds
    return readSoundElement(elem);
}


int TX_Asset::readSoundElement(tinyxml2::XMLElement *sound_element) noexcept
{
    string path;
    XMLElement *unit_element;

    LX_Log::logDebug(LX_Log::LX_LogType::APPLICATION,"asset — sound");
    path = sound_element->Attribute(PATH_ATTR_STR);    // Music path

    if(path.empty())
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,
                            "readSoundElement: Invalid attribute - expected : path");
        return static_cast<int>(XML_WRONG_ATTRIBUTE_TYPE);
    }

    unit_element = sound_element->FirstChildElement(UNIT_NODE_STR);

    if(unit_element == nullptr)
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,
                            "readSoundElement: Invalid element - expected : Unit");
        return static_cast<int>(XML_ERROR_PARSING_ELEMENT);
    }

    size_t id = 0;

    while(unit_element != nullptr)
    {
        sounds[id++] = path + unit_element->Attribute(FILENAME_ATTR_STR);

        LX_Log::logDebug(LX_Log::LX_LogType::APPLICATION,"asset — sound#%u: %s", id-1,
                         sounds[id-1].c_str());

        unit_element = unit_element->NextSiblingElement(UNIT_NODE_STR);
    }

    XMLElement * elem = sound_element->NextSiblingElement(LEVEL_NODE_STR);

    if(elem == nullptr)
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,
                            "readXMLFile: Invalid element - expected : Level");
        return static_cast<int>(XML_ERROR_PARSING_ELEMENT);
    }

    return readLevelElement(elem);
}


int TX_Asset::readLevelElement(XMLElement *level_element) noexcept
{
    string path, id;
    XMLElement *unit_element;

    LX_Log::logDebug(LX_Log::LX_LogType::APPLICATION,"asset — level");
    path = level_element->Attribute(PATH_ATTR_STR);

    if(path.empty())
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,
                            "readLevelElement: Invalid attribute - expected : path");
        return static_cast<int>(XML_WRONG_ATTRIBUTE_TYPE);
    }

    unit_element = level_element->FirstChildElement(UNIT_NODE_STR);

    if(unit_element == nullptr)
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,
                            "readLevelElement: Invalid element - expected : Unit");
        return static_cast<int>(XML_ERROR_PARSING_ELEMENT);
    }

    while(unit_element != nullptr && unit_element->Attribute(ID_ATTR_STR) != nullptr)
    {
        unsigned index;
        id = unit_element->Attribute(ID_ATTR_STR);
        XMLUtil::ToUnsigned(id.c_str(), &index);

        level_path[index] = path + unit_element->Attribute(FILENAME_ATTR_STR);

        LX_Log::logDebug(LX_Log::LX_LogType::APPLICATION,"asset — level#%u: %s", index,
                         level_path[index].c_str());

        unit_element = unit_element->NextSiblingElement(UNIT_NODE_STR);
    }

    LX_Log::logDebug(LX_Log::LX_LogType::APPLICATION,"asset — done");
    return 0;
}


int TX_Asset::readPlayerElement(XMLElement *player_element, const string& path) noexcept
{
    XMLElement * sprite_element = player_element->FirstChildElement(SPRITE_NODE_STR);
    LX_Log::logDebug(LX_Log::LX_LogType::APPLICATION,"asset — player");

    if(sprite_element == nullptr)
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,
                            "readPlayerElement: Invalid element - expected : Sprite");
        return static_cast<int>(XML_ERROR_PARSING_ELEMENT);
    }

    // Get the first data and go to the next element
    player_string = path + sprite_element->Attribute(FILENAME_ATTR_STR);
    LX_Log::logDebug(LX_Log::LX_LogType::APPLICATION,"asset — player: %s",
                     player_string.c_str());
    sprite_element = sprite_element->NextSiblingElement(SPRITE_NODE_STR);

    if(sprite_element == nullptr)
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,
                            "readPlayerElement: Invalid element - expected : Sprite");
        return static_cast<int>(XML_ERROR_PARSING_ELEMENT);
    }

    player_shield_string = path + sprite_element->Attribute(FILENAME_ATTR_STR);
    LX_Log::logDebug(LX_Log::LX_LogType::APPLICATION,"asset — player (shield): %s",
                     player_shield_string.c_str());

    // Item
    XMLElement * item_element = player_element->NextSiblingElement(ITEM_NODE_STR);

    if(item_element == nullptr)
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,
                            "readImageElement: Invalid element - expected : Item");
        return static_cast<int>(XML_ERROR_PARSING_ELEMENT);
    }

    // Item
    return readItemElement(item_element, path);
}


int TX_Asset::readItemElement(XMLElement *item_element, const string& path) noexcept
{
    XMLElement * sprite_element = item_element->FirstChildElement(SPRITE_NODE_STR);

    LX_Log::logDebug(LX_Log::LX_LogType::APPLICATION,"asset — item");
    if(sprite_element == nullptr)
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,
                            "readItemElement: Invalid element - expected : Sprite");
        return static_cast<int>(XML_ERROR_PARSING_ELEMENT);
    }

    // Get the path attribute of Item
    string ipath = item_element->Attribute(PATH_ATTR_STR);

    if(ipath.empty())
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,
                            "readItemElement: Invalid attribute - expected : path");
        return static_cast<int>(XML_WRONG_ATTRIBUTE_TYPE);
    }

    size_t i = 0;

    // Get the files
    while(i < NB_ITEMS && sprite_element != nullptr)
    {
        items[i] = path + ipath + sprite_element->Attribute(FILENAME_ATTR_STR);
        LX_Log::logDebug(LX_Log::LX_LogType::APPLICATION, "asset — item#%u: %s", i,
                         items[i].c_str());
        sprite_element = sprite_element->NextSiblingElement(SPRITE_NODE_STR);
        i++;
    }

    XMLElement * missile_element = item_element->NextSiblingElement(MISSILE_NODE_STR);

    if(missile_element == nullptr)
    {
        LX_Log::logCritical(LX_Log::LX_LogType::APPLICATION,
                            "readImageElement: Invalid element - expected : Missile");
        return static_cast<int>(XML_ERROR_PARSING_ELEMENT);
    }

    // Missile
    return readMissileElement(missile_element, path);
}


int TX_Asset::readMissileElement(XMLElement *missile_element, const string& path) noexcept
{
    LX_Log::logDebug(LX_Log::LX_LogType::APPLICATION,"asset — missiles");
    return readElements_(missile_element, missiles, missile_coord, path);
}

int TX_Asset::readEnemyElement(XMLElement *enemy_element, const string& path) noexcept
{
    LX_Log::logDebug(LX_Log::LX_LogType::APPLICATION,"asset — enemies");
    return readElements_(enemy_element, enemy_path, enemy_coord, path);
}

int TX_Asset::readExplosionElement(XMLElement *explosion_element, const string& path) noexcept
{
    LX_Log::logDebug(LX_Log::LX_LogType::APPLICATION,"asset — explosion");
    return readElements_(explosion_element, explosions, coordinates, path);
}

int TX_Asset::readBgElement(tinyxml2::XMLElement *bg_element,
                            const std::string& path) noexcept
{
    LX_Log::logDebug(LX_Log::LX_LogType::APPLICATION,"asset — background");
    return readUI_(bg_element, level_bg, path, BGI_NODE_STR);
}


int TX_Asset::readCoordElement(tinyxml2::XMLElement *coord_element, TX_Anima& anima) noexcept
{
    LX_Graphics::LX_ImgRect box = {{0,0},0,0};
    string value;

    while(coord_element != nullptr && coord_element->Attribute(X_ATTR_STR) != nullptr
            && coord_element->Attribute(Y_ATTR_STR) != nullptr
            && coord_element->Attribute(W_ATTR_STR) != nullptr
            && coord_element->Attribute(H_ATTR_STR) != nullptr)
    {
        // Get X
        value = coord_element->Attribute(X_ATTR_STR);
        XMLUtil::ToInt(value.c_str(), &box.p.x);
        // Get Y
        value = coord_element->Attribute(Y_ATTR_STR);
        XMLUtil::ToInt(value.c_str(), &box.p.y);
        // Get the Width
        value = coord_element->Attribute(W_ATTR_STR);
        XMLUtil::ToInt(value.c_str(), &box.w);
        // Get the Height
        value = coord_element->Attribute(H_ATTR_STR);
        XMLUtil::ToInt(value.c_str(), &box.h);
        anima.v.push_back(box);

        coord_element = coord_element->NextSiblingElement(COORD_NODE_STR);
    }

    return 0;
}


int TX_Asset::readParallaxElement(tinyxml2::XMLElement *para_element,
                                  const std::string& path, size_t lvl_index) noexcept
{
    if(para_element == nullptr)
    {
        LX_Log::logError(LX_Log::LX_LogType::APPLICATION,"invalid BGImage node from readParallaxElement()");
        return static_cast<int>(tinyxml2::XML_ERROR_PARSING_ELEMENT);
    }

    size_t i = 0;
    TX_ParallaxAsset * passet = new TX_ParallaxAsset();
    std::array<std::string, NB_PARALLAX> parallax_arr;

    while(para_element != nullptr && para_element->Attribute(FILENAME_ATTR_STR) != nullptr)
    {
        parallax_arr[i++] = path + para_element->Attribute(FILENAME_ATTR_STR);

        LX_Log::logDebug(LX_Log::LX_LogType::APPLICATION,"asset — parallax#%u: %s", i-1,
                         parallax_arr[i-1].c_str());

        para_element = para_element->NextSiblingElement(PARALLAX_NODE_STR);
    }

    passet->parallax01_bg = parallax_arr[0];
    passet->parallax02_bg = parallax_arr[1];
    passet->parallax03_bg = parallax_arr[2];
    parallax[lvl_index] = passet;

    return 0;
}


int TX_Asset::readMenuElement(tinyxml2::XMLElement *menu_element,
                              const std::string& path) noexcept
{
    LX_Log::logDebug(LX_Log::LX_LogType::APPLICATION,"asset — menu");
    return readUI_(menu_element, menu_img, path);
}
