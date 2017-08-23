
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

#include "TX_Asset.hpp"


using namespace std;
using namespace tinyxml2;

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

void TX_Asset::destroy()
{
    delete tx_singleton;
}

TX_Asset * TX_Asset::getInstance()
{
    return tx_singleton;
}

const string TX_Asset::getFontFile() const
{
    return font_file;
}

const string TX_Asset::getPlayerFile() const
{
    return player_string;
}

const string TX_Asset::getPlayerShieldFile() const
{
    return player_shield_string;
}

const string TX_Asset::getItemFile(unsigned int index) const
{
    return items[index];
}

const string TX_Asset::getMissileFile(unsigned int index) const
{
    return missiles[index];
}

const string TX_Asset::getMenuImgFile(unsigned int id) const
{
    return menu_img[id];
}

// Get the root element of the XML file
XMLElement * TX_Asset::getRootElement(XMLHandle *hdl)
{
    return (hdl->FirstChildElement(ROOT_NODE_STR).ToElement());
}

// Returns the path of a music file according to the id of the level
const string TX_Asset::getLevelMusic(unsigned int id) const
{
    return level_music.at(id);
}

const string TX_Asset::getSound(unsigned int id) const
{
    return sounds.at(id);
}

const string TX_Asset::getLevelPath(unsigned int id) const
{
    return level_path.at(id);
}

const string TX_Asset::getLevelBg(unsigned int id) const
{
    return level_bg.at(id);
}

const TX_ParallaxAsset& TX_Asset::getLevelParallax(unsigned int id) const
{
    return *(parallax.at(id));
}

// Get the list of file path to the sprites of enemies
const string TX_Asset::getEnemySpriteFile(unsigned int id) const
{
    return enemy_path.at(id);
}

const string TX_Asset::getExplosionSpriteFile(unsigned int id) const
{
    return explosions.at(id);
}

const TX_Anima* TX_Asset::getExplosionAnimation(unsigned int id) const
{
    return coordinates.at(id);
}

const TX_Anima* TX_Asset::getEnemyAnimation(unsigned int id) const
{
    return enemy_coord.at(id);
}

const TX_Anima* TX_Asset::getMissileAnimation(unsigned int id) const
{
    return missile_coord.at(id);
}

const string TX_Asset::getfileName() const
{
    return xml_filename;
}


// Read and extract data from an XML file
int TX_Asset::readXMLFile()
{
    XMLDocument doc;
    XMLHandle hdl(&doc);
    XMLElement *tx, *elem;
    XMLError err;
    ostringstream ss;

    LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"asset — open %s", xml_filename.c_str());
    err = doc.LoadFile(xml_filename.c_str());

    if(err != XML_SUCCESS)
    {
        ss << "readXMLFile: error #" << err << " : " << doc.ErrorName() << endl;
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", ss.str().c_str());
        return static_cast<int>(err);
    }

    // Get the root element
    if((tx = getRootElement(&hdl)) == nullptr)
    {
        ss << "readXMLFile: Invalid element - expected : TX_asset" << endl;
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", ss.str().c_str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Get The Font element
    elem = tx->FirstChildElement(FONT_NODE_STR);

    if(elem == nullptr)
    {
        ss << "readXMLFile: Invalid element - expected : Font" << endl;
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", ss.str().c_str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    if(readFontElement(elem) != 0)
    {
        ss << "readXMLFile: Invalid XML file — font" << endl;
        return LX_SetError(ss.str());
    }

    // Get The Image element
    elem = elem->NextSiblingElement(IMAGE_NODE_STR);

    if(elem == nullptr)
    {
        ss << "readXMLFile: Invalid element - expected : Image" << endl;
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", ss.str().c_str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Extract information about images
    if(readImageElement(elem) != 0)
    {
        ss << "readXMLFile: Invalid XML file — image" << endl;
        return LX_SetError(ss.str());
    }

    elem = elem->NextSiblingElement(MUSIC_NODE_STR);

    if(elem == nullptr)
    {
        ss << "readXMLFile: Invalid element - expected : Music" << endl;
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", ss.str().c_str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Extract information about musics
    if(readMusicElement(elem) != 0)
    {
        ss << "readXMLFile: Invalid XML file — music" << endl;
        return LX_SetError(ss.str());
    }

    elem = elem->NextSiblingElement(SOUND_NODE_STR);

    if(elem == nullptr)
    {
        ss << "readXMLFile: Invalid element - expected : Sound" << endl;
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", ss.str().c_str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Extract information about sounds
    if(readSoundElement(elem) != 0)
    {
        ss << "readXMLFile: Invalid XML file — sound" << endl;
        return LX_SetError(ss.str());
    }

    elem = elem->NextSiblingElement(LEVEL_NODE_STR);

    if(elem == nullptr)
    {
        ss << "readXMLFile: Invalid element - expected : Level" << endl;
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", ss.str().c_str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Extract information about the levels of the game
    if(readLevelElement(elem) != 0)
    {
        ss << "readXMLFile: Invalid XML file – level" << endl;
        return LX_SetError(ss.str());
    }

    LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"asset — done");
    return 0;
}


int TX_Asset::readFontElement(XMLElement *font_element)
{
    string path, filename;
    XMLElement *unit_element;
    ostringstream ss;

    LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"asset — font file");
    // Get the path attribute of Image
    path = font_element->Attribute(PATH_ATTR_STR);

    if(path.empty())
    {
        ss << "readFontElement: No attribute - expected : path" << endl;
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", ss.str().c_str());
        return static_cast<int>(XML_NO_ATTRIBUTE);
    }

    unit_element = font_element->FirstChildElement(UNIT_NODE_STR);

    if(unit_element == nullptr)
    {
        ss << "readFontElement: Invalid element - expected : Unit" << endl;
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", ss.str().c_str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    filename = unit_element->Attribute(FILENAME_ATTR_STR);

    if(filename.empty())
    {
        ss << "readFontElement: No attribute or empty value" << endl;
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", ss.str().c_str());
        return static_cast<int>(XML_NO_ATTRIBUTE);
    }

    size_t pos = filename.find(TTF_EXT);

    if(pos == string::npos || filename.substr(pos) != TTF_EXT)
    {
        ss << "readFontElement: Bad attribute type - expected : "
           << TTF_EXT << "file" << endl;
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", ss.str().c_str());
        return static_cast<int>(XML_WRONG_ATTRIBUTE_TYPE);
    }

    font_file = path + filename;
    LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"asset — font file: %s", font_file.c_str());
    return 0;
}

int TX_Asset::readImageElement(XMLElement *image_element)
{
    string path;
    int err_read_player, err_read_item, err_read_missile;
    int err_read_enemy, err_read_explosion, err_read_bg, err_read_menu;
    XMLElement *player_element;
    XMLElement *item_element;
    XMLElement *missile_element;
    XMLElement *enemy_element;
    XMLElement *explosion_element;
    XMLElement *bg_element;
    XMLElement *menu_element;
    ostringstream ss;

    LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"asset — get images");

    // Get the path attribute of Image
    path = image_element->Attribute(PATH_ATTR_STR);

    if(path.empty())
    {
        ss << "readImageElement: Invalid attribute - expected : path" << endl;
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", ss.str().c_str());
        return static_cast<int>(XML_WRONG_ATTRIBUTE_TYPE);
    }

    /*
        Get the elements to load the sprites
    */
    player_element = image_element->FirstChildElement(PLAYER_NODE_STR);

    if(player_element == nullptr)
    {
        ss << "readImageElement: Invalid element - expected : Player" << endl;
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", ss.str().c_str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Item
    item_element = player_element->NextSiblingElement(ITEM_NODE_STR);

    if(item_element == nullptr)
    {
        ss << "readImageElement: Invalid element - expected : Item" << endl;
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", ss.str().c_str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Missile
    missile_element = item_element->NextSiblingElement(MISSILE_NODE_STR);

    if(missile_element == nullptr)
    {
        ss << "readImageElement: Invalid element - expected : Missile" << endl;
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", ss.str().c_str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Enemy
    enemy_element = missile_element->NextSiblingElement(ENEMY_NODE_STR);

    if(enemy_element == nullptr)
    {
        ss << "readImageElement: Invalid element - expected : Enemy" << endl;
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", ss.str().c_str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    explosion_element = missile_element->NextSiblingElement(EXPLOSION_NODE_STR);

    if(explosion_element == nullptr)
    {
        ss << "Invalid element - expected : Explosion" << endl;
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", ss.str().c_str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    bg_element = missile_element->NextSiblingElement(BACKGROUND_NODE_STR);

    if(bg_element == nullptr)
    {
        ss << "Invalid element - expected : Background" << endl;
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", ss.str().c_str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    menu_element = bg_element->NextSiblingElement(MENU_NODE_STR);

    if(menu_element == nullptr)
    {
        ss << "Invalid element - expected : Menu" << endl;
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", ss.str().c_str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Get returned values
    err_read_player = readPlayerElement(player_element, path.c_str());
    err_read_item = readItemElement(item_element, path.c_str());
    err_read_missile = readMissileElement(missile_element, path.c_str());
    err_read_enemy = readEnemyElement(enemy_element, path.c_str());
    err_read_explosion = readExplosionElement(explosion_element, path.c_str());
    err_read_bg = readBgElement(bg_element, path);
    err_read_menu = readMenuElement(menu_element, path);

    return (err_read_player || err_read_item|| err_read_missile
            || err_read_enemy || err_read_explosion || err_read_bg
            || err_read_menu);
}


int TX_Asset::readMusicElement(XMLElement *music_element)
{
    string lvl;
    string path;
    XMLElement *unit_element;
    ostringstream ss;

    LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"asset — music");
    path = music_element->Attribute(PATH_ATTR_STR);    // Music path

    if(path.empty())
    {
        ss << "readMusicElement: Invalid attribute - expected : path" << endl;
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", ss.str().c_str());
        return static_cast<int>(XML_WRONG_ATTRIBUTE_TYPE);
    }

    unit_element = music_element->FirstChildElement(UNIT_NODE_STR);

    if(unit_element == nullptr)
    {
        ss << "readMusicElement: Invalid element - expected : Unit" << endl;
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", ss.str().c_str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    int i;

    while(unit_element != nullptr && unit_element->Attribute(ID_ATTR_STR) != nullptr)
    {
        size_t id;
        lvl = unit_element->Attribute(ID_ATTR_STR);
        XMLUtil::ToInt(lvl.c_str(),&i);
        id = static_cast<size_t>(i);
        level_music[id] = path + unit_element->Attribute(FILENAME_ATTR_STR);
        LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"asset — music#%u: %s", id,
                         level_music[id].c_str());
        unit_element = unit_element->NextSiblingElement(UNIT_NODE_STR);
    }

    return 0;
}


int TX_Asset::readSoundElement(tinyxml2::XMLElement *sound_element)
{
    string path;
    XMLElement *unit_element;
    ostringstream ss;

    LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"asset — sound");
    path = sound_element->Attribute(PATH_ATTR_STR);    // Music path

    if(path.empty())
    {
        ss << "readSoundElement: Invalid attribute - expected : path" << endl;
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", ss.str().c_str());
        return static_cast<int>(XML_WRONG_ATTRIBUTE_TYPE);
    }

    unit_element = sound_element->FirstChildElement(UNIT_NODE_STR);

    if(unit_element == nullptr)
    {
        ss << "readSoundElement: Invalid element - expected : Unit" << endl;
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", ss.str().c_str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    size_t id = 0;

    while(unit_element != nullptr)
    {
        sounds[id++] = path + unit_element->Attribute(FILENAME_ATTR_STR);
        LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"asset — sound#%u: %s", id-1,
                         sounds[id-1].c_str());
        unit_element = unit_element->NextSiblingElement(UNIT_NODE_STR);
    }

    return 0;
}


int TX_Asset::readLevelElement(XMLElement *level_element)
{
    string path, id;
    XMLElement *unit_element;
    ostringstream ss;

    LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"asset — level");
    path = level_element->Attribute(PATH_ATTR_STR);

    if(path.empty())
    {
        ss << "readLevelElement: Invalid attribute - expected : path" << endl;
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", ss.str().c_str());
        return static_cast<int>(XML_WRONG_ATTRIBUTE_TYPE);
    }

    unit_element = level_element->FirstChildElement(UNIT_NODE_STR);

    if(unit_element == nullptr)
    {
        ss << "readLevelElement: Invalid element - expected : Unit" << endl;
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", ss.str().c_str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    int i;

    while(unit_element != nullptr && unit_element->Attribute(ID_ATTR_STR) != nullptr)
    {
        size_t index;
        id = unit_element->Attribute(ID_ATTR_STR);
        XMLUtil::ToInt(id.c_str(),&i);
        index = static_cast<size_t>(i);
        level_path[index] = path + unit_element->Attribute(FILENAME_ATTR_STR);
        LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"asset — level#%u: %s", index,
                         level_path[index].c_str());
        unit_element = unit_element->NextSiblingElement(UNIT_NODE_STR);
    }

    return 0;
}


int TX_Asset::readPlayerElement(XMLElement *player_element, const string& path)
{
    ostringstream ss;
    XMLElement * sprite_element = player_element->FirstChildElement(SPRITE_NODE_STR);
    LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"asset — player");

    if(sprite_element == nullptr)
    {
        ss << "readPlayerElement: Invalid element - expected : Sprite" << endl;
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", ss.str().c_str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Get the first data and go to the next element
    player_string = path + sprite_element->Attribute(FILENAME_ATTR_STR);
    LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"asset — player: %s", player_string.c_str());
    sprite_element = sprite_element->NextSiblingElement(SPRITE_NODE_STR);

    if(sprite_element == nullptr)
    {
        ss << "readPlayerElement: Invalid element - expected : Sprite" << endl;
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", ss.str().c_str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    player_shield_string = path + sprite_element->Attribute(FILENAME_ATTR_STR);
    LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"asset — player (shield): %s",
                     player_shield_string.c_str());
    return 0;
}


int TX_Asset::readItemElement(XMLElement *item_element, const string& path)
{
    string ipath;
    size_t i = 0;
    ostringstream ss;
    XMLElement * sprite_element = item_element->FirstChildElement(SPRITE_NODE_STR);

    LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"asset — item");
    if(sprite_element == nullptr)
    {
        ss << "readItemElement: Invalid element - expected : Sprite" << endl;
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", ss.str().c_str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Get the path attribute of Item
    ipath = item_element->Attribute(PATH_ATTR_STR);

    if(ipath.empty())
    {
        ss << "readItemElement: Invalid attribute - expected : path" << endl;
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", ss.str().c_str());
        return static_cast<int>(XML_WRONG_ATTRIBUTE_TYPE);
    }

    // Get the files
    while(i < NB_ITEMS && sprite_element != nullptr)
    {
        items[i] = path + ipath + sprite_element->Attribute(FILENAME_ATTR_STR);
        LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION, "asset — item#%u: %s", i,
                         items[i].c_str());
        sprite_element = sprite_element->NextSiblingElement(SPRITE_NODE_STR);
        i++;
    }

    return 0;
}


int TX_Asset::readMissileElement(XMLElement *missile_element, const string& path)
{
    LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"asset — missiles");
    return readElements_(missile_element, missiles, missile_coord, path);
}

int TX_Asset::readEnemyElement(XMLElement *enemy_element, const string& path)
{
    LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"asset — enemies");
    return readElements_(enemy_element, enemy_path, enemy_coord, path);
}

int TX_Asset::readExplosionElement(XMLElement *explosion_element, const string& path)
{
    LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"asset — explosion");
    return readElements_(explosion_element, explosions, coordinates, path);
}

int TX_Asset::readCoordElement(tinyxml2::XMLElement *coord_element, TX_Anima& anima)
{
    LX_AABB box = {0,0,0,0};
    string value;

    while(coord_element != nullptr && coord_element->Attribute(X_ATTR_STR) != nullptr
            && coord_element->Attribute(Y_ATTR_STR) != nullptr
            && coord_element->Attribute(W_ATTR_STR) != nullptr
            && coord_element->Attribute(H_ATTR_STR) != nullptr)
    {
        // Get X
        value = coord_element->Attribute(X_ATTR_STR);
        XMLUtil::ToInt(value.c_str(), &box.x);
        // Get Y
        value = coord_element->Attribute(Y_ATTR_STR);
        XMLUtil::ToInt(value.c_str(), &box.y);
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


int TX_Asset::readBgElement(tinyxml2::XMLElement *bg_element,
                            const std::string& path)
{
    LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"asset — background");
    return readUI_(bg_element, level_bg, path, BGI_NODE_STR);
}

int TX_Asset::readParallaxElement(tinyxml2::XMLElement *para_element,
                                  const std::string& path, size_t lvl_index)
{
    if(para_element == nullptr)
    {
        LX_Log::logError(LX_Log::LX_LOG_APPLICATION,"invalid BGImage node from readParallaxElement()");
        return static_cast<int>(tinyxml2::XML_ERROR_ELEMENT_MISMATCH);
    }

    size_t i = 0;
    TX_ParallaxAsset * passet = new TX_ParallaxAsset();
    std::array<std::string, NB_PARALLAX> parallax_arr;

    while(para_element != nullptr && para_element->Attribute(FILENAME_ATTR_STR) != nullptr)
    {
        parallax_arr[i++] = path + para_element->Attribute(FILENAME_ATTR_STR);
        LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"asset — parallax#%u: %s", i-1,
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
                              const std::string& path)
{
    LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"asset — menu");
    return readUI_(menu_element, menu_img, path);
}

