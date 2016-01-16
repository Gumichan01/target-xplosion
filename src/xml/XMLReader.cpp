
/*
*	Target_Xplosion - The classic shoot'em up video game
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
*	website : gumichan01.olympe.in
*	mail : luxon.jean.pierre@gmail.com
*/

/**
*	@file XMLReader.cpp
*	@brief The XML reader implementation of Target Xplosion
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <new>
#include <sstream>
#include <LunatiX/LX_Error.hpp>

#include "XMLReader.hpp"

using namespace std;
using namespace tinyxml2;

static TX_Asset *tx_singleton = nullptr;


TX_Asset::TX_Asset()
{
    // Empty
}

TX_Asset::~TX_Asset()
{
    // Empty
}


void TX_Asset::init(void)
{
    if(tx_singleton == nullptr)
        tx_singleton = new TX_Asset();
}

void TX_Asset::destroy(void)
{
    delete tx_singleton;
}

TX_Asset * TX_Asset::getInstance(void)
{
    return tx_singleton;
}

const char * TX_Asset::getPlayerFile(void) const
{
    return player_string.c_str();
}

const char * TX_Asset::getPlayerShieldFile(void) const
{
    return player_shield_string.c_str();
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

// Get the root element of the XML file
XMLElement * TX_Asset::getRootElement(XMLHandle *hdl)
{
    return (hdl->FirstChildElement("TX_asset").ToElement());
}

// Returns the path of a music file according to the id of the level
const char * TX_Asset::getLevelMusic(unsigned int id) const
{
    if(id < LEVELS)
        return level_music[id].c_str();
    else
        return nullptr;
}


const char * TX_Asset::getLevelPath(unsigned int id) const
{
    if(id < LEVELS)
        return level_path[id].c_str();
    else
        return nullptr;
}


// Get the list of file path to the sprites of enemies
string TX_Asset::getEnemySpriteFile(unsigned int id) const
{
    return enemy_sprites_path[id];
}


// Read and extract data from an XML file
int TX_Asset::readXMLFile(const char * filename)
{
    XMLDocument doc;
    XMLHandle hdl(&doc);
    XMLElement *tx = nullptr;
    XMLElement *elem = nullptr;
    XMLError err;
    ostringstream ss;

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
    elem = tx->FirstChildElement("Image");

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
        return LX_SetError(ss.str());;
    }

    elem = elem->NextSiblingElement("Music");

    if(elem == nullptr)
    {
        ss << "Invalid element : expected : Music" << "\n";
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Extract information about musics
    if(readMusicElement(elem) != 0)
    {
        ss << "Invalid XML file" << "\n";
        return LX_SetError(ss.str());;
    }

    elem = elem->NextSiblingElement("Level");

    if(elem == nullptr)
    {
        ss << "Invalid element : expected : Level" << "\n";
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Extract information about the levels of the game
    if(readLevelElement(elem) != 0)
    {
        ss << "Invalid XML file" << "\n";
        return LX_SetError(ss.str());;
    }

    return 0;
}


int TX_Asset::readImageElement(XMLElement *image_element)
{
    string path;
    int err_read_player, err_read_item;
    int err_read_missile, err_read_enemy;
    XMLElement *player_element = nullptr;
    XMLElement *item_element = nullptr;
    XMLElement *missile_element = nullptr;
    XMLElement *enemy_element = nullptr;
    ostringstream ss;

    // Get the path attribute of Image
    path = image_element->Attribute("path");

    if(path.empty())
    {
        ss << "Invalid attribute : expected : path" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_WRONG_ATTRIBUTE_TYPE);
    }

    /*
        Get the elements to the sprites
    */
    player_element = image_element->FirstChildElement("Player");
    item_element = player_element->NextSiblingElement("Item");
    missile_element = item_element->NextSiblingElement("Missile");
    enemy_element = missile_element->NextSiblingElement("Enemy");

    if(player_element == nullptr)
    {
        ss << "Invalid element : expected : Player" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    if(item_element == nullptr)
    {
        ss << "Invalid element : expected : Item" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    if(missile_element == nullptr)
    {
        ss << "Invalid element : expected : Missile" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    if(enemy_element == nullptr)
    {
        ss << "Invalid element : expected : Enemy" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Get returned values
    err_read_player = readPlayerElement(player_element, path.c_str());
    err_read_item = readItemElement(item_element,path.c_str());
    err_read_missile = readMissileElement(missile_element,path.c_str());
    err_read_enemy = readEnemyElement(enemy_element,path.c_str());

    return (err_read_player || err_read_item
            || err_read_missile || err_read_enemy); // 0 on SUCCESS
}


int TX_Asset::readMusicElement(XMLElement *music_element)
{
    string lvl;
    string path;
    XMLElement *unit_element = nullptr;
    ostringstream ss;

    path = music_element->Attribute("path");    // Music path

    if(path.empty())
    {
        ss << "Invalid attribute : expected : path" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_WRONG_ATTRIBUTE_TYPE);
    }

    unit_element = music_element->FirstChildElement("Unit");

    if(unit_element == nullptr)
    {
        ss << "Invalid element : expected : Unit" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    int i;
    unsigned long id;

    while(unit_element != nullptr && unit_element->Attribute("level") != nullptr)
    {
        lvl = unit_element->Attribute("level");
        XMLUtil::ToInt(lvl.c_str(),&i);
        id = static_cast<unsigned long>(i);
        level_music[id] = path + unit_element->Attribute("filename");
        unit_element = unit_element->NextSiblingElement("Unit");
    }

    return 0;
}


int TX_Asset::readLevelElement(XMLElement *level_element)
{
    string path, id;
    XMLElement *unit_element = nullptr;
    ostringstream ss;

    path = level_element->Attribute("path");

    if(path.empty())
    {
        ss << "Invalid attribute : expected : path" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_WRONG_ATTRIBUTE_TYPE);
    }

    unit_element = level_element->FirstChildElement("Unit");

    if(unit_element == nullptr)
    {
        ss << "Invalid element : expected : Unit" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    int i;
    unsigned long index;

    while(unit_element != nullptr && unit_element->Attribute("id") != nullptr)
    {
        id = unit_element->Attribute("id");
        XMLUtil::ToInt(id.c_str(),&i);
        index = static_cast<unsigned long>(i);
        level_path[index] = path + unit_element->Attribute("filename");
        unit_element = unit_element->NextSiblingElement("Unit");
    }

    return 0;
}


int TX_Asset::readPlayerElement(XMLElement *player_element,string path)
{
    ostringstream ss;
    XMLElement * sprite_element = nullptr;

    // Get the first sprite
    sprite_element = player_element->FirstChildElement("Sprite");

    if(sprite_element == nullptr)
    {
        ss << "Invalid element : expected : Sprite" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Get the first data and go to the next element
    player_string = path + sprite_element->Attribute("filename");
    sprite_element = sprite_element->NextSiblingElement("Sprite");

    if(sprite_element == nullptr)
    {
        ss << "Invalid element : expected : Sprite" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    player_shield_string = path + sprite_element->Attribute("filename");
    return 0;
}


int TX_Asset::readItemElement(XMLElement *item_element,string path)
{
    unsigned long i = 0;
    ostringstream ss;
    XMLElement * sprite_element = nullptr;
    sprite_element = item_element->FirstChildElement("Sprite");

    if(sprite_element == nullptr)
    {
        ss << "Invalid element : expected : Sprite" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Get the files
    while(i < NB_ITEMS && sprite_element != nullptr)
    {
        items[i] = path + sprite_element->Attribute("filename");
        sprite_element = sprite_element->NextSiblingElement("Sprite");
        i++;
    }

    return 0;
}


int TX_Asset::readMissileElement(XMLElement *missile_element,string path)
{
    unsigned long i = 0, j = 0;
    ostringstream ss;
    XMLElement * sprite_element = nullptr;
    sprite_element = missile_element->FirstChildElement("Sprite");

    if(sprite_element == nullptr)
    {
        ss << "Invalid element : expected : Sprite" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    while(i < PLAYER_MISSILES && sprite_element != nullptr)
    {
        player_missiles[i] = path + sprite_element->Attribute("filename");
        sprite_element = sprite_element->NextSiblingElement("Sprite");
        i++;
    }

    while(j < ENEMY_MISSILES && sprite_element != nullptr)
    {
        enemy_missiles[j] = path + sprite_element->Attribute("filename");
        sprite_element = sprite_element->NextSiblingElement("Sprite");
        j++;
    }

    return 0;
}


int TX_Asset::readEnemyElement(XMLElement *enemy_element,string path)
{
    int i;
    unsigned long index;
    string id;
    ostringstream ss;
    XMLElement *unit_element = nullptr;
    unit_element = enemy_element->FirstChildElement("Unit");

    if(unit_element == nullptr)
    {
        ss << "Invalid element : expected : Unit" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    while(unit_element != nullptr && unit_element->Attribute("id") != nullptr)
    {
        id = unit_element->Attribute("id");

        if(!id.empty())
        {
            XMLUtil::ToInt(id.c_str(),&i);
            index = static_cast<unsigned long>(i);
            enemy_sprites_path[index] = path +
                                        unit_element->Attribute("filename");
        }

        unit_element = unit_element->NextSiblingElement("Unit");
    }

    return 0;
}

