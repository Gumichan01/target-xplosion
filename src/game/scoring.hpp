#ifndef SCORING_H_INCLUDED
#define SCORING_H_INCLUDED



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
*	@file scoring.h
*	@brief The scoring file header
*	@author Luxon Jean-Pierre(Gumichan01)
*
*/

#define VAL_SCORE_SIZE 0


class LX_TrueTypeFont;


class Score
{

    unsigned int previous_score;
    unsigned int current_score;
    unsigned int total_score;
    LX_TrueTypeFont *tmp_ttf;


    public:

    Score(unsigned int ps);

    void notify(int newScore);
    void display(void);

    unsigned int get_prev_score(){return previous_score;}
    unsigned int get_cur_score(){return current_score;}
    unsigned int get_total_score(){return total_score;}


    ~Score(){}

};









#endif // SCORING_H_INCLUDED













