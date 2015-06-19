
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
#include <iostream>
#include <sstream>

#include "XMLReader.hpp"

using namespace std;
//using namespace tinyxml2;

static TX_Asset *tx_singleton = NULL;



TX_Asset::TX_Asset()
{
    items = new (nothrow) string[NB_ITEMS];
    playerM = new (nothrow) string[NB_MISSILES];
    enemyM = new (nothrow) string[NB_MISSILES];
}


TX_Asset::~TX_Asset()
{
    delete [] enemyM;
    delete [] playerM;
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
    return playerM;
}


const std::string * TX_Asset::enemyMissilesFiles(void)
{
    return enemyM;
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



int TX_Asset::readImageElement(XMLElement *imageElement)
{
    XMLElement *playerElement = NULL;
    XMLElement *itemElement = NULL;
    XMLElement *missileElement = NULL;
    XMLElement *spriteElement = NULL;
    string path;
    int i = 0;

    // Get the path attribute of Image
    path = imageElement->Attribute("path");

    if(path.empty())
    {
        cerr << "Invalid attribute : expected : path" << endl;
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Get the player element
    playerElement = imageElement->FirstChildElement("Player");

    if(playerElement == NULL)
    {
        cerr << "Invalid element : expected : Player" << endl;
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Get the first sprite
    spriteElement = playerElement->FirstChildElement("Sprite");

    if(spriteElement == NULL)
    {
        cerr << "Invalid element : expected : Sprite" << endl;
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    // Get the first data and go to the next element
    playerStr = path + spriteElement->Attribute("filename");
    spriteElement = spriteElement->NextSiblingElement("Sprite");

    if(spriteElement == NULL)
    {
        cerr << "Invalid element : expected : Sprite" << endl;
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    playerShieldStr = path + spriteElement->Attribute("filename");

    // Get the item element
    itemElement = playerElement->NextSiblingElement("Item");
    spriteElement = itemElement->FirstChildElement("Sprite");

    // Get the files
    while(i < NB_ITEMS && spriteElement != NULL)
    {
        items[i] = path + spriteElement->Attribute("filename");
        spriteElement = spriteElement->NextSiblingElement("Sprite");
        i++;
    }

    i = 0;
    missileElement = itemElement->NextSiblingElement("Missile");

    if(missileElement == NULL)
    {
        cerr << "Invalid element : expected : Missile" << endl;
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    spriteElement = missileElement->FirstChildElement("Sprite");

    while(i < NB_MISSILES && spriteElement != NULL)
    {
        playerM[i] = path + spriteElement->Attribute("filename");
        spriteElement = spriteElement->NextSiblingElement("Sprite");
        i++;
    }

    i = 0;

    while(i < NB_MISSILES && spriteElement != NULL)
    {
        enemyM[i] = path + spriteElement->Attribute("filename");
        spriteElement = spriteElement->NextSiblingElement("Sprite");
        i++;
    }

    return 0;
}



const char * TX_Asset::loadLevelMusic(unsigned int level,const char *filename)
{
    XMLDocument doc;
    XMLHandle hdl(&doc);
    XMLElement *tx = NULL;
    XMLElement *elem = NULL;
    XMLError err;

    const string error = "ERROR";
    string path, levelStr, lvl;
    stringstream ss;
    string result = error.c_str();

    ss << level;
    levelStr = ss.str();

    err = doc.LoadFile(filename);

    if(err != XML_SUCCESS)
    {
        cerr << "error #" << err << " : " << doc.ErrorName() << endl;
        return error.c_str();
    }

    // Get the root element
    if((tx = getRootElement(&hdl)) == NULL)
    {
        cerr << "Invalid element : expected : TX_asset" << endl;
        return error.c_str();
    }

    // Go to the Music element
    if((elem = tx->FirstChildElement("Image")) == NULL)
    {
        cerr << "Invalid element : expected : Image" << endl;
        return error.c_str();
    }

    if((elem = elem->NextSiblingElement("Music") ) == NULL)
    {
        cerr << "Invalid element : expected : Music" << endl;
        return error.c_str();
    }

    path = elem->Attribute("path");
    elem = elem->FirstChildElement("Unit");

    while(elem != NULL && elem->Attribute("level") != NULL)
    {
        lvl = elem->Attribute("level");

        if(lvl.empty() || lvl.compare(levelStr))
        {
            elem = elem->NextSiblingElement("Unit");
            continue;
        }

        result = path + elem->Attribute("filename");
        break;
    }

    return result.c_str();
}


