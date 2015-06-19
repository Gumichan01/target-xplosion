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
#include <vector>
#include <TinyXML/tinyxml2.h>

#define NB_ITEMS 5
#define NB_MISSILES 4


using namespace tinyxml2;


class TX_Asset
{
    std::string playerStr;
    std::string playerShieldStr;

    std::string * items;
    std::string * playerM;
    std::string * enemyM;

    TX_Asset();
    ~TX_Asset();

    XMLElement * getRootElement(XMLHandle *hdl);
    int readImageElement(XMLElement *imageElement);

public:

    static void init(void);
    static TX_Asset * getInstance(void);
    static void destroy(void);

    int readXMLFile(const char *filename="config/asset.xml");
    const char * loadLevelMusic(unsigned int level=0,const char *filename="config/asset.xml");

    const char * playerFile(void);
    const char * playerShieldFile(void);
    const std::string * itemsFiles(void);
    const std::string * playerMissilesFiles(void);
    const std::string * enemyMissilesFiles(void);
};


#endif // XMLREADER_HPP_INCLUDED

