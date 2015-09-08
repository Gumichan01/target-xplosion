
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
*	@file XMLReader.cpp
*	@brief The XML reader implementation of Target Xplosion
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <new>
#include <sstream>
#include <cstdlib>

#include "XMLReader.hpp"

static TX_Asset *tx_singleton = NULL;



TX_Asset::TX_Asset()
{
    items = new (nothrow) string[NB_ITEMS];
    player_missiles = new (nothrow) string[PLAYER_MISSILES];
    enemy_missiles = new (nothrow) string[ENEMY_MISSILES];
    level_music = new (nothrow) string[LEVELS];
}


TX_Asset::~TX_Asset()
{
    delete [] level_music;
    delete [] enemy_missiles;
    delete [] player_missiles;
    delete [] items;
}


void TX_Asset::init(void)
{
    if(tx_singleton == NULL)
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


const char * TX_Asset::getPlayerFile(void)
{
    return player_string.c_str();
}


const char * TX_Asset::getPlayerShieldFile(void)
{
    return player_shield_string.c_str();
}

const std::string * TX_Asset::getItemFiles(void)
{
    return items;
}

const std::string * TX_Asset::getPlayerMissilesFiles(void)
{
    return player_missiles;
}


const std::string * TX_Asset::getEnemyMissilesFiles(void)
{
    return enemy_missiles;
}

// Get the root element of the XML file
XMLElement * TX_Asset::getRootElement(XMLHandle *hdl)
{
    return (hdl->FirstChildElement("TX_asset").ToElement());
}

// Returns the path of a music file according to the id of the level
const char * TX_Asset::levelMusic(int id)
{
    if(id >= 0 && id < LEVELS)
        return level_music[id].c_str();
    else
        return NULL;
}


// Read and extract data from an XML file
int TX_Asset::readXMLFile(const char * filename)
{
    XMLDocument doc;
    XMLHandle hdl(&doc);
    XMLElement *tx = NULL;
    XMLElement *elem = NULL;
    XMLError err;

    err = doc.LoadFile(filename);

    if(err != XML_SUCCESS)
    {
        cerr << "error #" << err << " : " << doc.ErrorName() << endl;
        return static_cast<int>(err);
    }

    // Get the root element
    if((tx = getRootElement(&hdl)) == NULL)
    {
        cerr << "Invalid element : expected : TX_asset" << endl;
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Get The Image element
    elem = tx->FirstChildElement("Image");

    if(elem == NULL)
    {
        cerr << "Invalid element : expected : Image" << endl;
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Extract information about images
    if(readImageElement(elem) != 0)
    {
        cerr << "Invalid XML file" << endl;
        return -1;
    }

    elem = elem->NextSiblingElement("Music");

    if(elem == NULL)
    {
        cerr << "Invalid element : expected : Music" << endl;
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Extract information about musics
    if(readMusicElement(elem) != 0)
    {
        cerr << "Invalid XML file" << endl;
        return -1;
    }

    return 0;
}


int TX_Asset::readImageElement(XMLElement *image_element)
{
    XMLElement *player_element = NULL;
    XMLElement *item_element = NULL;
    XMLElement *missile_element = NULL;
    string path;

    // Get the path attribute of Image
    path = image_element->Attribute("path");

    if(path.empty())
    {
        cerr << "Invalid attribute : expected : path" << endl;
        return static_cast<int>(XML_WRONG_ATTRIBUTE_TYPE);
    }

    /**
        Get the player element and path to the sprites
    */
    player_element = image_element->FirstChildElement("Player");

    if(player_element == NULL)
    {
        cerr << "Invalid element : expected : Player" << endl;
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    if(readPlayerElement(player_element, path.c_str()) != 0)
    {
        cerr << "Invalid XML file" << endl;
        return -1;
    }

    /** Player element done */

    /**
        Get the item element
    */
    item_element = player_element->NextSiblingElement("Item");

    if(item_element == NULL)
    {
        cerr << "Invalid element : expected : Item" << endl;
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    readItemElement(item_element,path.c_str());

    /** Item done */

    /**
        Get the missile element
    */

    missile_element = item_element->NextSiblingElement("Missile");

    if(missile_element == NULL)
    {
        cerr << "Invalid element : expected : Missile" << endl;
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    readMissileElement(missile_element,path.c_str());

    /** Missile element */

    ///@todo Enemies

    return 0;
}


int TX_Asset::readMusicElement(XMLElement *music_element)
{
    XMLElement *unit_element = NULL;
    string path, result;
    string lvl;
    int i;

    // Music path
    path = music_element->Attribute("path");

    if(path.empty())
    {
        cerr << "Invalid attribute : expected : path" << endl;
        return static_cast<int>(XML_WRONG_ATTRIBUTE_TYPE);
    }

    unit_element = music_element->FirstChildElement("Unit");

    if(unit_element == NULL)
    {
        cerr << "Invalid element : expected : Unit" << endl;
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    while(unit_element != NULL && unit_element->Attribute("level") != NULL)
    {
        lvl = unit_element->Attribute("level");

        if(!lvl.empty())
        {
            i = atoi(lvl.c_str());
            result = path + unit_element->Attribute("filename");
            level_music[i] = result;
        }

        unit_element = unit_element->NextSiblingElement("Unit");
    }

    return 0;
}


int TX_Asset::readPlayerElement(XMLElement *player_element,string path)
{
    XMLElement * sprite_element = NULL;

    // Get the first sprite
    sprite_element = player_element->FirstChildElement("Sprite");

    if(sprite_element == NULL)
    {
        cerr << "Invalid element : expected : Sprite" << endl;
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Get the first data and go to the next element
    player_string = path + sprite_element->Attribute("filename");
    sprite_element = sprite_element->NextSiblingElement("Sprite");

    if(sprite_element == NULL)
    {
        cerr << "Invalid element : expected : Sprite" << endl;
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    player_shield_string = path + sprite_element->Attribute("filename");

    return 0;
}


int TX_Asset::readItemElement(XMLElement *item_element,string path)
{
    XMLElement * sprite_element = NULL;
    int i = 0;

    sprite_element = item_element->FirstChildElement("Sprite");

    if(sprite_element == NULL)
    {
        cerr << "Invalid element : expected : Sprite" << endl;
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Get the files
    while(i < NB_ITEMS && sprite_element != NULL)
    {
        items[i] = path + sprite_element->Attribute("filename");
        sprite_element = sprite_element->NextSiblingElement("Sprite");
        i++;
    }

    return 0;
}


int TX_Asset::readMissileElement(XMLElement *missile_element,string path)
{
    XMLElement * sprite_element = NULL;
    int i = 0;
    int j = 0;

    sprite_element = missile_element->FirstChildElement("Sprite");

    if(sprite_element == NULL)
    {
        cerr << "Invalid element : expected : Sprite" << endl;
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    while(i < PLAYER_MISSILES && sprite_element != NULL)
    {
        player_missiles[i] = path + sprite_element->Attribute("filename");
        sprite_element = sprite_element->NextSiblingElement("Sprite");
        i++;
    }

    while(j < ENEMY_MISSILES && sprite_element != NULL)
    {
        enemy_missiles[j] = path + sprite_element->Attribute("filename");
        sprite_element = sprite_element->NextSiblingElement("Sprite");
        j++;
    }

    return 0;
}










