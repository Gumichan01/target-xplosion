#ifndef OBSERVER_H_INCLUDED
#define OBSERVER_H_INCLUDED



/*
*
*	Copyright (C)  Luxon Jean-Pierre
*	gumichan01.olympe.in
*
*
*	Luxon Jean-Pierre (Gumichan01)
*	luxon.jean.pierre@gmail.com
*
*/

/**
*	@file Observer
*	@brief The observer class
*	@author Luxon Jean-Pierre(Gumichan01)
*	@version 0.2
*	@date September 12th, 2014
*
*
*
*/

class Observer
{

    public:

    virtual void update() = 0;
    virtual ~Observer(){}

};



#endif // OBSERVER_H_INCLUDED







