
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

#include "../utils/tinyxml2.h"

#include <LunatiX/LX_Log.hpp>

template<typename T>
void TX_Asset::initArray(T& ar) noexcept
{
    ar.fill(nullptr);
}

template<typename T>
void TX_Asset::cleanArray(T& ar) noexcept
{
    for(unsigned int i = 0; i < ar.size(); i++)
    {
        delete ar[i];
        ar[i] = nullptr;
    }
}


template<typename T, typename U>
int TX_Asset::readElements_(tinyxml2::XMLElement *elements,
                            T& elem_array, U& coord_array,
                            const std::string& path) noexcept
{
    tinyxml2::XMLElement *unit_element = nullptr;

    if(elements != nullptr)
        unit_element = elements->FirstChildElement(SPRITE_NODE_STR);

    if(unit_element == nullptr)
    {
        LX_setError("readElement_: Invalid element : expected : Sprite\n");
        return static_cast<int>(tinyxml2::XMLError::XML_ERROR_PARSING_ELEMENT);
    }

    const char * upath = elements->Attribute(PATH_ATTR_STR);
    if(upath == nullptr)
    {
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"Invalid attribute");
        return static_cast<int>(tinyxml2::XMLError::XML_WRONG_ATTRIBUTE_TYPE);
    }

    //path += upath;
    return readElementsAttr_(unit_element, elem_array, coord_array, path + upath);
}

template<typename T, typename U>
int TX_Asset::readElementsAttr_(tinyxml2::XMLElement *unit_element,
                                T& elem_array, U& coord_array,
                                const std::string& path) noexcept
{
    while(unit_element != nullptr && unit_element->Attribute(FILENAME_ATTR_STR) != nullptr)
    {
        std::string id_str{""};

        if(unit_element->Attribute(ID_ATTR_STR) != nullptr)
        {
            id_str = unit_element->Attribute(ID_ATTR_STR);
        }

        if(!id_str.empty())
        {
            unsigned int delay;
            unsigned int index;
            tinyxml2::XMLUtil::ToUnsigned(id_str.c_str(), &index);
            elem_array[index] = path + unit_element->Attribute(FILENAME_ATTR_STR);
            LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"asset — #%u: %s", index,
                             elem_array[index].c_str());

            if(unit_element->Attribute(DELAY_ATTR_STR) != nullptr)
            {
                const std::string& delay_str = unit_element->Attribute(DELAY_ATTR_STR);
                tinyxml2::XMLUtil::ToUnsigned(delay_str.c_str(), &delay);
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
int TX_Asset::readUI_(tinyxml2::XMLElement *elements, T& elem_array,
                      const std::string& path, const char *node) noexcept
{
    tinyxml2::XMLElement *unit_element = elements->FirstChildElement(node);

    if(unit_element == nullptr)
    {
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,
                            "readMenuElement: Invalid element - expected : %s",
                            node);
        return static_cast<int>(tinyxml2::XMLError::XML_ERROR_PARSING_ELEMENT);
    }

    const char *mpath = elements->Attribute(PATH_ATTR_STR);
    if(mpath == nullptr)
    {
        LX_Log::logCritical(LX_Log::LX_LOG_APPLICATION,"Invalid path");
        return static_cast<int>(tinyxml2::XMLError::XML_WRONG_ATTRIBUTE_TYPE);
    }

    size_t i = 0;
    while(unit_element != nullptr && unit_element->Attribute(FILENAME_ATTR_STR) != nullptr)
    {
        elem_array[i++] = path + mpath + unit_element->Attribute(FILENAME_ATTR_STR);
        LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION,"asset — unit#%u: %s", i-1,
                         elem_array[i-1].c_str());

        const char * parallax_str = unit_element->Attribute(PARALLAX_ATTR_STR);

        if(parallax_str != nullptr)
        {
            std::string _parallax_str(parallax_str);

            if(_parallax_str == PARALLAX_YES_STR)
                readParallaxElement(unit_element->FirstChildElement(PARALLAX_NODE_STR),
                                    std::string(path + mpath), i-1);
        }

        unit_element = unit_element->NextSiblingElement(node);
    }

    return 0;
}
