#ifndef STRATEGY_H_INCLUDED
#define STRATEGY_H_INCLUDED



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
*	@file Strategy.hpp
*	@brief The strategy class
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#include <iostream>

class Enemy;

#define DELAY_ENEMY_MISSILE 1000
#define DELAY_ENEMY_ROCKET 5000
#define DELAY_ENEMY_LASER 10000
#define DELAY_ENEMY_BOMB 4000


class Strategy_exception : public std::exception
{
    public :

    std::string str_err;

    Strategy_exception(std::string err);

    const char * what() const throw();

    ~Strategy_exception() throw(){}
};


class Strategy
{
    protected:

    Enemy *target;

    double reference_time;      // The reference time

    double cur_time;            // The current time
    double cur_r_time;
    double cur_b_time;
    double cur_l_time;

    public :

    Strategy(Enemy *newEnemy);

    virtual void proceed(void) = 0;

    ~Strategy(){}

};


class Basic_strategy: public Strategy
{
    double delay_missile;       // The delay between two basic missiles shot

    public:

    Basic_strategy(Enemy *newEnemy);

    void proceed(void);

    ~Basic_strategy(){}

};


/*class Sin_strategy: public Strategy
{

    public:

    Basic_missile(Enemy *newEnemy);

    ~Basic_missile(){}

};*/















#endif // STRATEGY_H_INCLUDED









