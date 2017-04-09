
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

#include <LunatiX/LX_Log.hpp>

template<typename T>
void TX_Asset::initArray(T& ar)
{
    ar.fill(nullptr);
}

template<typename T>
void TX_Asset::cleanArray(T& ar)
{
    for(unsigned int i = 0; i < ar.size(); i++)
    {
        if(ar[i] != nullptr)
        {
            delete ar[i];
            ar[i] = nullptr;
        }
    }
}


template<typename T, typename U>
int TX_Asset::readElements_(tinyxml2::XMLElement *elements,
                            T& elem_array, U& coord_array,
                            std::string path)
{
    std::ostringstream ss;
    tinyxml2::XMLElement *unit_element = nullptr;

    if(elements != nullptr)
        unit_element = elements->FirstChildElement(SPRITE_NODE_STR);

    if(unit_element == nullptr)
    {
        ss << "readElement_: Invalid element : expected : Sprite" << "\n";
        LX_SetError(ss.str());
        return static_cast<int>(tinyxml2::XML_ERROR_ELEMENT_MISMATCH);
    }

    LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", unit_element->Name());

    const char * upath = elements->Attribute(PATH_ATTR_STR);
    if(upath == nullptr)
    {
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"Invalid attribute");
        return static_cast<int>(tinyxml2::XML_WRONG_ATTRIBUTE_TYPE);
    }

    unsigned j;
    size_t index;
    uint32_t delay;
    std::string id_str;
    std::string delay_str;

    path += upath;
    while(unit_element != nullptr && unit_element->Attribute(FILENAME_ATTR_STR) != nullptr)
    {
        {
            const char *tmp = unit_element->Attribute(ID_ATTR_STR);

            if(tmp != nullptr)
                id_str = tmp;
            else
                id_str.clear();
        }

        if(!id_str.empty())
        {
            unsigned i;
            tinyxml2::XMLUtil::ToUnsigned(id_str.c_str(),&i);
            index = static_cast<size_t>(i);
            elem_array[index] = path + unit_element->Attribute(FILENAME_ATTR_STR);
            LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"asset — #%u: %s", i,
                             elem_array[i].c_str());

            if(unit_element->Attribute(DELAY_ATTR_STR) != nullptr)
            {
                delay_str = unit_element->Attribute(DELAY_ATTR_STR);
                tinyxml2::XMLUtil::ToUnsigned(delay_str.c_str(),&j);
                delay = static_cast<uint32_t>(j);
            }
            else
                delay = 0;

            tinyxml2::XMLElement *coord_element = unit_element->FirstChildElement(COORD_NODE_STR);

            if(coord_element != nullptr)
            {
                TX_Anima* anima = new TX_Anima();
                anima->delay = delay;
                readCoordElement(coord_element, *anima);
                coord_array[index] = anima;
            }
        }

        unit_element = unit_element->NextSiblingElement(SPRITE_NODE_STR);
    }

    return 0;
}


template<typename T>
int TX_Asset::readUI_(tinyxml2::XMLElement *elements,
                             T& elem_array, std::string path)
{
    std::ostringstream ss;
    tinyxml2::XMLElement *unit_element = elements->FirstChildElement(UNIT_NODE_STR);
    LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"asset — menu");

    if(unit_element == nullptr)
    {
        ss << "readMenuElement: Invalid element - expected : Unit" << std::endl;
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"%s", ss.str().c_str());
        return static_cast<int>(tinyxml2::XML_ERROR_ELEMENT_MISMATCH);
    }

    const char *mpath = elements->Attribute(PATH_ATTR_STR);
    if(mpath == nullptr)
    {
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"Invalid path");
        return static_cast<int>(tinyxml2::XML_WRONG_ATTRIBUTE_TYPE);
    }

    size_t i = 0;
    while(unit_element != nullptr && unit_element->Attribute(FILENAME_ATTR_STR) != nullptr)
    {
        elem_array[i++] = path + mpath + unit_element->Attribute(FILENAME_ATTR_STR);
        LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"asset — unit#%u: %s", i-1,
                         elem_array[i-1].c_str());
        unit_element = unit_element->NextSiblingElement(UNIT_NODE_STR);
    }

    return 0;
}

