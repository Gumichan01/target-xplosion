
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

#include "XMLReader.hpp"

static TX_Asset *tx_singleton = NULL;



TX_Asset::TX_Asset()
{
    items = new (nothrow) string[NB_ITEMS];
    player_missiles = new (nothrow) string[PLAYER_MISSILES];
    enemy_missiles = new (nothrow) string[ENEMY_MISSILES];
}


TX_Asset::~TX_Asset()
{
    delete [] enemy_missiles;
    delete [] player_missiles;
    delete [] items;
}


void TX_Asset::init(void)
{
    if(tx_singleton == NULL)
    {
        tx_singleton = new TX_Asset();
    }
}


void TX_Asset::destroy(void)
{
    delete tx_singleton;
}


TX_Asset * TX_Asset::getInstance(void)
{
    return tx_singleton;
}


const char * TX_Asset::playerFile(void)
{
    return playerStr.c_str();
}


const char * TX_Asset::playerShieldFile(void)
{
    return playerShieldStr.c_str();
}

const std::string * TX_Asset::itemsFiles(void)
{
    return items;
}

const std::string * TX_Asset::playerMissilesFiles(void)
{
    return player_missiles;
}


const std::string * TX_Asset::enemyMissilesFiles(void)
{
    return enemy_missiles;
}


XMLElement * TX_Asset::getRootElement(XMLHandle *hdl)
{
    return (hdl->FirstChildElement("TX_asset").ToElement());
}



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

    readImageElement(elem);

    return 0;
}



int TX_Asset::readImageElement(XMLElement *image_element)
{
    XMLElement *player_element = NULL;
    XMLElement *item_element = NULL;
    XMLElement *missile_element = NULL;
    XMLElement *sprite_element = NULL;
    string path;
    int i = 0;

    // Get the path attribute of Image
    path = image_element->Attribute("path");

    if(path.empty())
    {
        cerr << "Invalid attribute : expected : path" << endl;
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Get the player element
    player_element = image_element->FirstChildElement("Player");

    if(player_element == NULL)
    {
        cerr << "Invalid element : expected : Player" << endl;
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Get the first sprite
    sprite_element = player_element->FirstChildElement("Sprite");

    if(sprite_element == NULL)
    {
        cerr << "Invalid element : expected : Sprite" << endl;
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Get the first data and go to the next element
    playerStr = path + sprite_element->Attribute("filename");
    sprite_element = sprite_element->NextSiblingElement("Sprite");

    if(sprite_element == NULL)
    {
        cerr << "Invalid element : expected : Sprite" << endl;
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    playerShieldStr = path + sprite_element->Attribute("filename");

    // Get the item element
    item_element = player_element->NextSiblingElement("Item");
    sprite_element = item_element->FirstChildElement("Sprite");

    // Get the files
    while(i < NB_ITEMS && sprite_element != NULL)
    {
        items[i] = path + sprite_element->Attribute("filename");
        sprite_element = sprite_element->NextSiblingElement("Sprite");
        i++;
    }

    i = 0;
    missile_element = item_element->NextSiblingElement("Missile");

    if(missile_element == NULL)
    {
        cerr << "Invalid element : expected : Missile" << endl;
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    sprite_element = missile_element->FirstChildElement("Sprite");

    while(i < PLAYER_MISSILES && sprite_element != NULL)
    {
        player_missiles[i] = path + sprite_element->Attribute("filename");
        sprite_element = sprite_element->NextSiblingElement("Sprite");
        i++;
    }

    i = 0;

    while(i < ENEMY_MISSILES && sprite_element != NULL)
    {
        enemy_missiles[i] = path + sprite_element->Attribute("filename");
        sprite_element = sprite_element->NextSiblingElement("Sprite");
        i++;
    }

    return 0;
}



const char * TX_Asset::loadLevelMusic(unsigned int level,char *str,const char *filename)
{
    XMLDocument doc;
    XMLHandle hdl(&doc);
    XMLElement *tx = NULL;
    XMLElement *elem = NULL;
    XMLError err;

    string path, level_string, lvl;
    stringstream ss;
    string result;

    ss << level;
    level_string = ss.str();

    if(str == NULL)
        return NULL;

    memset(str,0,DEFAULT_TEXT_SIZE);

    err = doc.LoadFile(filename);

    if(err != XML_SUCCESS)
    {
        cerr << "error #" << err << " : " << doc.ErrorName() << endl;
        return NULL;
    }

    // Get the root element
    if((tx = getRootElement(&hdl)) == NULL)
    {
        cerr << "Invalid element : expected : TX_asset" << endl;
        return NULL;
    }

    // Go to the Music element
    if((elem = tx->FirstChildElement("Image")) == NULL)
    {
        cerr << "Invalid element : expected : Image" << endl;
        return NULL;
    }

    if((elem = elem->NextSiblingElement("Music") ) == NULL)
    {
        cerr << "Invalid element : expected : Music" << endl;
        return NULL;
    }

    path = elem->Attribute("path");
    elem = elem->FirstChildElement("Unit");

    while(elem != NULL && elem->Attribute("level") != NULL)
    {
        lvl = elem->Attribute("level");

        if(lvl.empty() || lvl.compare(level_string))
        {
            elem = elem->NextSiblingElement("Unit");
            continue;
        }

        result = path + elem->Attribute("filename");
        strcpy(str,result.c_str());
        break;
    }

    // No file found
    if(elem == NULL || elem->Attribute("level") == NULL)
        return NULL;

    return str;
}


