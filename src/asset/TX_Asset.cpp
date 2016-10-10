
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright (C) 2016  Luxon Jean-Pierre
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
*   website : gumichan01.kappatau.fr
*   mail : luxon.jean.pierre@gmail.com
*/

#include "TX_Asset.hpp"

#include <LunatiX/LX_Error.hpp>
#include <LunatiX/LX_Log.hpp>

#include <sstream>
#include <new>


using namespace std;
using namespace tinyxml2;

static TX_Asset *tx_singleton = nullptr;

TX_Asset::TX_Asset()
{
    coordinates.fill(nullptr);
}

TX_Asset::~TX_Asset()
{
    for(unsigned int i = 0; i < NB_XPLOSION; i++)
    {
        if(coordinates[i] != nullptr)
        {
            delete coordinates[i];
            coordinates[i] = nullptr;
        }
    }
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

string TX_Asset::getPlayerFile() const
{
    return player_string;
}

string TX_Asset::getPlayerShieldFile() const
{
    return player_shield_string;
}

string TX_Asset::getItemFile(unsigned int index) const
{
    return items[index];
}

string TX_Asset::getPlayerMissilesFile(unsigned int index) const
{
    return player_missiles[index];
}

string TX_Asset::getEnemyMissilesFile(unsigned int index) const
{
    return enemy_missiles[index];
}

string TX_Asset::getMenuImgFile(unsigned int id) const
{
    return menu_img[id];
}

// Get the root element of the XML file
XMLElement * TX_Asset::getRootElement(XMLHandle *hdl)
{
    return (hdl->FirstChildElement(ROOT_NODE_STR).ToElement());
}

// Returns the path of a music file according to the id of the level
string TX_Asset::getLevelMusic(unsigned int id) const
{
    return level_music.at(id);
}

string TX_Asset::getSound(unsigned int id) const
{
    return sounds.at(id);
}

string TX_Asset::getLevelPath(unsigned int id) const
{
    return level_path.at(id);
}

string TX_Asset::getLevelBg(unsigned int id) const
{
    return level_bg.at(id);
}

// Get the list of file path to the sprites of enemies
string TX_Asset::getEnemySpriteFile(unsigned int id) const
{
    return enemy_path.at(id);
}

string TX_Asset::getExplosionSpriteFile(unsigned int id) const
{
    return explosions.at(id);
}

const TX_Anima* TX_Asset::getAnimation(unsigned int id) const
{
    return coordinates.at(id);
}


string TX_Asset::getfileName() const
{
    return xml_filename;
}


// Read and extract data from an XML file
int TX_Asset::readXMLFile(const char * filename)
{
    XMLDocument doc;
    XMLHandle hdl(&doc);
    XMLElement *tx = nullptr, *elem = nullptr;
    XMLError err;
    ostringstream ss;

    xml_filename = filename;
    err = doc.LoadFile(filename);

    if(err != XML_SUCCESS)
    {
        ss << "error #" << err << " : " << doc.ErrorName() << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(err);
    }

    // Get the root element
    if((tx = getRootElement(&hdl)) == nullptr)
    {
        ss << "Invalid element : expected : TX_asset" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Get The Image element
    elem = tx->FirstChildElement(IMAGE_NODE_STR);

    if(elem == nullptr)
    {
        ss << "Invalid element : expected : Image" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Extract information about images
    if(readImageElement(elem) != 0)
    {
        ss << "Invalid XML file" << "\n";
        return LX_SetError(ss.str());
    }

    elem = elem->NextSiblingElement(MUSIC_NODE_STR);

    if(elem == nullptr)
    {
        ss << "Invalid element : expected : Music" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Extract information about musics
    if(readMusicElement(elem) != 0)
    {
        ss << "Invalid XML file" << "\n";
        return LX_SetError(ss.str());
    }

    elem = elem->NextSiblingElement(SOUND_NODE_STR);

    if(elem == nullptr)
    {
        ss << "Invalid element : expected : Sound" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Extract information about sounds
    if(readSoundElement(elem) != 0)
    {
        ss << "Invalid XML file" << "\n";
        return LX_SetError(ss.str());
    }

    elem = elem->NextSiblingElement(LEVEL_NODE_STR);

    if(elem == nullptr)
    {
        ss << "Invalid element : expected : Level" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Extract information about the levels of the game
    if(readLevelElement(elem) != 0)
    {
        ss << "Invalid XML file" << "\n";
        return LX_SetError(ss.str());
    }

    return 0;
}


int TX_Asset::readImageElement(XMLElement *image_element)
{
    string path;
    int err_read_player, err_read_item, err_read_missile;
    int err_read_enemy, err_read_explosion, err_read_bg, err_read_menu;
    XMLElement *player_element = nullptr;
    XMLElement *item_element = nullptr;
    XMLElement *missile_element = nullptr;
    XMLElement *enemy_element = nullptr;
    XMLElement *explosion_element = nullptr;
    XMLElement *bg_element = nullptr;
    XMLElement *menu_element = nullptr;
    ostringstream ss;

    // Get the path attribute of Image
    path = image_element->Attribute(PATH_ATTR_STR);

    if(path.empty())
    {
        ss << "Invalid attribute : expected : path" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_WRONG_ATTRIBUTE_TYPE);
    }

    /*
        Get the elements to load the sprites
    */
    player_element = image_element->FirstChildElement(PLAYER_NODE_STR);

    if(player_element == nullptr)
    {
        ss << "Invalid element : expected : Player" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Item
    item_element = player_element->NextSiblingElement(ITEM_NODE_STR);

    if(item_element == nullptr)
    {
        ss << "Invalid element : expected : Item" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Missile
    missile_element = item_element->NextSiblingElement(MISSILE_NODE_STR);

    if(missile_element == nullptr)
    {
        ss << "Invalid element : expected : Missile" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Enemy
    enemy_element = missile_element->NextSiblingElement(ENEMY_NODE_STR);

    if(enemy_element == nullptr)
    {
        ss << "Invalid element : expected : Enemy" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    explosion_element = missile_element->NextSiblingElement(EXPLOSION_NODE_STR);

    if(explosion_element == nullptr)
    {
        ss << "Invalid element : expected : Explosion" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    bg_element = missile_element->NextSiblingElement(BACKGROUND_NODE_STR);

    if(bg_element == nullptr)
    {
        ss << "Invalid element : expected : Background" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    menu_element = bg_element->NextSiblingElement(MENU_NODE_STR);

    if(menu_element == nullptr)
    {
        ss << "Invalid element : expected : Menu" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Get returned values
    err_read_player = readPlayerElement(player_element, path.c_str());
    err_read_item = readItemElement(item_element,path.c_str());
    err_read_missile = readMissileElement(missile_element,path.c_str());
    err_read_enemy = readEnemyElement(enemy_element,path.c_str());
    err_read_explosion = readExplosionElement(explosion_element,path.c_str());
    err_read_bg = readBgElement(bg_element,path.c_str());
    err_read_menu = readMenuElement(menu_element,path.c_str());

    return (err_read_player || err_read_item|| err_read_missile
            || err_read_enemy || err_read_explosion || err_read_bg || err_read_menu);
}


int TX_Asset::readMusicElement(XMLElement *music_element)
{
    string lvl;
    string path;
    XMLElement *unit_element = nullptr;
    ostringstream ss;

    path = music_element->Attribute(PATH_ATTR_STR);    // Music path

    if(path.empty())
    {
        ss << "Invalid attribute : expected : path" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_WRONG_ATTRIBUTE_TYPE);
    }

    unit_element = music_element->FirstChildElement(UNIT_NODE_STR);

    if(unit_element == nullptr)
    {
        ss << "Invalid element : expected : Unit" << "\n";
        LX_SetError(ss.str());
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
        unit_element = unit_element->NextSiblingElement(UNIT_NODE_STR);
    }

    return 0;
}


int TX_Asset::readSoundElement(tinyxml2::XMLElement *sound_element)
{
    string path;
    XMLElement *unit_element = nullptr;
    ostringstream ss;

    path = sound_element->Attribute(PATH_ATTR_STR);    // Music path

    if(path.empty())
    {
        ss << "Invalid attribute : expected : path" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_WRONG_ATTRIBUTE_TYPE);
    }

    unit_element = sound_element->FirstChildElement(UNIT_NODE_STR);

    if(unit_element == nullptr)
    {
        ss << "Invalid element : expected : Unit" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    size_t id = 0;

    while(unit_element != nullptr)
    {
        sounds[id++] = path + unit_element->Attribute(FILENAME_ATTR_STR);
        unit_element = unit_element->NextSiblingElement(UNIT_NODE_STR);
    }

    return 0;
}


int TX_Asset::readLevelElement(XMLElement *level_element)
{
    string path, id;
    XMLElement *unit_element = nullptr;
    ostringstream ss;

    path = level_element->Attribute(PATH_ATTR_STR);

    if(path.empty())
    {
        ss << "Invalid attribute : expected : path" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_WRONG_ATTRIBUTE_TYPE);
    }

    unit_element = level_element->FirstChildElement(UNIT_NODE_STR);

    if(unit_element == nullptr)
    {
        ss << "Invalid element : expected : Unit" << "\n";
        LX_SetError(ss.str());
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
        unit_element = unit_element->NextSiblingElement(UNIT_NODE_STR);
    }

    return 0;
}


int TX_Asset::readPlayerElement(XMLElement *player_element,string path)
{
    ostringstream ss;
    XMLElement * sprite_element = nullptr;

    // Get the first sprite
    sprite_element = player_element->FirstChildElement(SPRITE_NODE_STR);

    if(sprite_element == nullptr)
    {
        ss << "Invalid element : expected : Sprite" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Get the first data and go to the next element
    player_string = path + sprite_element->Attribute(FILENAME_ATTR_STR);
    sprite_element = sprite_element->NextSiblingElement(SPRITE_NODE_STR);

    if(sprite_element == nullptr)
    {
        ss << "Invalid element : expected : Sprite" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    player_shield_string = path + sprite_element->Attribute(FILENAME_ATTR_STR);
    return 0;
}


int TX_Asset::readItemElement(XMLElement *item_element,string path)
{
    size_t i = 0;
    ostringstream ss;
    XMLElement * sprite_element = nullptr;
    sprite_element = item_element->FirstChildElement(SPRITE_NODE_STR);

    if(sprite_element == nullptr)
    {
        ss << "Invalid element : expected : Sprite" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Get the files
    while(i < NB_ITEMS && sprite_element != nullptr)
    {
        items[i] = path + sprite_element->Attribute(FILENAME_ATTR_STR);
        sprite_element = sprite_element->NextSiblingElement(SPRITE_NODE_STR);
        i++;
    }

    return 0;
}


int TX_Asset::readMissileElement(XMLElement *missile_element,string path)
{
    ostringstream ss;
    XMLElement * sprite_element = missile_element->FirstChildElement(SPRITE_NODE_STR);

    if(sprite_element == nullptr)
    {
        ss << "Invalid element : expected : Sprite" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    size_t i = 0, j = 0;

    while(i < PLAYER_MISSILES && sprite_element != nullptr)
    {
        string s = sprite_element->Attribute(FILENAME_ATTR_STR);

        if(!s.empty())
            player_missiles[i] = path + s;
        else
        {
            LX_Log::logWarning(LX_Log::LX_LOG_APPLICATION,
                               "TX_Asset - player missile data #%d is missing in %s",
                               i,xml_filename.c_str());
        }

        sprite_element = sprite_element->NextSiblingElement(SPRITE_NODE_STR);
        i++;
    }

    while(j < ENEMY_MISSILES && sprite_element != nullptr)
    {
        string s = sprite_element->Attribute(FILENAME_ATTR_STR);

        if(!s.empty())
            enemy_missiles[j] = path + s;
        else
        {
            LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,
                             "TX_Asset - enemy missile data #%d is missing in %s",
                             i+j+1,xml_filename.c_str());
        }

        sprite_element = sprite_element->NextSiblingElement(SPRITE_NODE_STR);
        j++;
    }

    return 0;
}


int TX_Asset::readEnemyElement(XMLElement *enemy_element,string path)
{
    int i;
    size_t index;
    string id;
    ostringstream ss;
    XMLElement *unit_element = nullptr;
    unit_element = enemy_element->FirstChildElement(SPRITE_NODE_STR);

    if(unit_element == nullptr)
    {
        ss << "Invalid element : expected : Sprite" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    while(unit_element != nullptr && unit_element->Attribute(ID_ATTR_STR) != nullptr)
    {
        id = unit_element->Attribute(ID_ATTR_STR);

        if(!id.empty())
        {
            XMLUtil::ToInt(id.c_str(),&i);
            index = static_cast<size_t>(i);
            enemy_path[index] = path + unit_element->Attribute(FILENAME_ATTR_STR);
        }

        unit_element = unit_element->NextSiblingElement(SPRITE_NODE_STR);
    }

    return 0;
}

int TX_Asset::readExplosionElement(tinyxml2::XMLElement *explosion_element,const std::string& path)
{
    ostringstream ss;
    XMLElement *unit_element = nullptr;
    unit_element = explosion_element->FirstChildElement(SPRITE_NODE_STR);

    if(unit_element == nullptr)
    {
        ss << "Invalid element : expected : Sprite" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    int j;
    string id;
    size_t i = 0;
    uint32_t delay;

    while(unit_element != nullptr && unit_element->Attribute(FILENAME_ATTR_STR) != nullptr)
    {
        explosions[i] = path + unit_element->Attribute(FILENAME_ATTR_STR);

        if(unit_element->Attribute(DELAY_ATTR_STR) != nullptr)
        {
            id = unit_element->Attribute(DELAY_ATTR_STR);
            XMLUtil::ToInt(id.c_str(),&j);
            delay = static_cast<uint32_t>(j);

            XMLElement *coord_element = unit_element->FirstChildElement(COORD_NODE_STR);

            if(coord_element != nullptr)
            {
                TX_Anima* anima = new TX_Anima();
                anima->delay = delay;
                readCoordElement(coord_element,*anima);
                coordinates[i] = anima;
            }
        }

        i += 1;
        unit_element = unit_element->NextSiblingElement(SPRITE_NODE_STR);
    }

    return 0;
}

int TX_Asset::readCoordElement(tinyxml2::XMLElement *coord_element,TX_Anima& anima)
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
        XMLUtil::ToInt(value.c_str(),&box.x);
        // Get Y
        value = coord_element->Attribute(Y_ATTR_STR);
        XMLUtil::ToInt(value.c_str(),&box.y);
        // Get the Width
        value = coord_element->Attribute(W_ATTR_STR);
        XMLUtil::ToInt(value.c_str(),&box.w);
        // Get the Height
        value = coord_element->Attribute(H_ATTR_STR);
        XMLUtil::ToInt(value.c_str(),&box.h);
        anima.v.push_back(box);

        coord_element = coord_element->NextSiblingElement(COORD_NODE_STR);
    }

    return 0;
}


int TX_Asset::readBgElement(tinyxml2::XMLElement *bg_element,const std::string& path)
{
    ostringstream ss;
    XMLElement *unit_element = nullptr;
    unit_element = bg_element->FirstChildElement(UNIT_NODE_STR);

    if(unit_element == nullptr)
    {
        ss << "Invalid element : expected : Unit" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    size_t i = 0;

    while(unit_element != nullptr && unit_element->Attribute(FILENAME_ATTR_STR) != nullptr)
    {
        level_bg[i++] = path + unit_element->Attribute(FILENAME_ATTR_STR);
        unit_element = unit_element->NextSiblingElement(UNIT_NODE_STR);
    }

    return 0;
}

int TX_Asset::readMenuElement(tinyxml2::XMLElement *menu_element,const std::string& path)
{
    ostringstream ss;
    XMLElement *unit_element = nullptr;
    unit_element = menu_element->FirstChildElement(UNIT_NODE_STR);

    if(unit_element == nullptr)
    {
        ss << "Invalid element : expected : Unit" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    size_t i = 0;

    while(unit_element != nullptr && unit_element->Attribute(FILENAME_ATTR_STR) != nullptr)
    {
        menu_img[i++] = path + unit_element->Attribute(FILENAME_ATTR_STR);
        unit_element = unit_element->NextSiblingElement(UNIT_NODE_STR);
    }

    return 0;
}

