
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

#include <iostream>

#include "XMLReader.hpp"

using namespace std;
using namespace tinyxml2;

static TX_Asset *tx_singleton = NULL;



TX_Asset::TX_Asset()
{
    playerStr = NULL;
    playerShieldStr = NULL;
}


TX_Asset::~TX_Asset()
{
    // Empty
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


int TX_Asset::readXMLFile(const char * filename)
{
    XMLDocument doc;
    XMLHandle hdl(&doc);
    XMLElement *tx = NULL;
    XMLElement *elem = NULL;
    XMLElement *playerElement = NULL;
    XMLError err;

    err = doc.LoadFile(filename);

    if(err != XML_SUCCESS)
    {
        cerr << "error #" << err << " : " << doc.ErrorName() << endl;
        return static_cast<int>(err);
    }

    tx = hdl.FirstChildElement("TX_asset").ToElement();

    if(tx == NULL)
    {
        cerr << "Invalid element : expected : <TX_asset>" << endl;
        return static_cast<int>(XML_ERROR_ELEMENT_MISMATCH);
    }

    return 0;
}
























