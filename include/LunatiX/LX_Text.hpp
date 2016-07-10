#ifndef LX_TEXT_HPP_INCLUDED
#define LX_TEXT_HPP_INCLUDED

/*
*	Copyright (C) 2016 Luxon Jean-Pierre
*	gumichan01.olympe.in
*
*	The LunatiX Engine is a SDL2-based game engine.
*	It can be used for open-source or commercial games thanks to the zlib/libpng license.
*
*   Luxon Jean-Pierre (Gumichan01)
*	luxon.jean.pierre@gmail.com
*/

/**
*	@file LX_Text.hpp
*	@brief The interface of LX_TextInput
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 0.8
*
*/

#include <LunatiX/utils/utf8_string.hpp>

union SDL_Event;

/**
*   @namespace LX_Text
*   @brief The Text input module
*/
namespace LX_Text
{

/**
*   @class LX_RedrawCallback
*   @brief Function object
*
*   This class defines a callback function as a function object
*
*/
class LX_RedrawCallback
{
public:
    LX_RedrawCallback();

    /**
    *   @fn virtual void operator ()(UTF8string& u8str, size_t cursor = 0) = 0;
    *
    *   Virtual function that must implemented in a subclass
    *   @param u8str The utf-8 encoded string that will be displayed
    *   @param cursor The position of the cursor on the string
    */
    virtual void operator ()(UTF8string& u8str, size_t cursor = 0) = 0;
    virtual ~LX_RedrawCallback();
};

/**
*   @class LX_TextInput
*   @brief Text input class
*
*   LX_TextInput is a class that handles text input. The text input is activated
*   at class intantiation and unactivated at class destruction.
*   This class can handle Unicode string in UTF-8 thanks to
*   [UTF8string](https://github.com/Gumichan01/utf8_string).
*
*   The API provides a callback (*LX_RedrawCallback*) function defined as a function object.
*   This callback was designed to redraw the current window that displays
*   the user-typed text at each user input.
*   It is not necessary to handle text input events, because *LX_TextInput* does that properly.
*
*   So, in order to use LX_TextInput, you need to:
*   - Define a class as a subclass of the pure virtuel class *LX_RedrawCallback*.
*   - Define the operator () provided by *LX_RedrawCallback*.
*   - Declare an object of type *LX_TextInput*.
*   - Call the function **eventLoop** of this object giving the callback function in argument.
*
*   Example of code :
*
*       // Define and implement a subclass of LX_RedrawCallback
*       class FuncDraw : public LX_RedrawCallback
*       {
*       public:
*
*           ...
*           void operator ()(UTF8String& u8str)
*           {
*               // Do something
*           }
*           ...
*       }
*
*       ...
*
*       // In a function that uses the callback
*       ...
*       void textInput()
*       {
*           FuncDraw callbck;
*           LX_TextInput input;
*           input.eventLoop(callbck);
*       }
*
*   LX_TextInput also support clipboard handling.
*/

class LX_TextInput
{
    UTF8string u8text;
    size_t cursor;
    bool done;

    LX_TextInput(LX_TextInput& t);
    LX_TextInput(LX_TextInput&& t);
    LX_TextInput& operator =(LX_TextInput t);

    // Save a text in the clipboard get it from it
    void save();
    void paste();

    // Input
    void keyboardInput(SDL_Event& ev);
    void textInput(SDL_Event& ev);
    void textEdit(SDL_Event& ev);

    // Operation on the string
    void u8stringInput(UTF8string& ntext);
    void utf8Pop();
    void backslashKey();
    void deleteKey();

public:

    LX_TextInput();
    void eventLoop(LX_RedrawCallback& redraw);
    ~LX_TextInput();
};

};

#endif // LX_TEXT_HPP_INCLUDED
