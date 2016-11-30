/**
 * @file   view.h
 * @author Istvan Burbank, Peter Friend, James Cassell
 * @date   2016-10-07
 * @brief  Implementation of ECE4760 Lab3
 * 
 * Pinout:
 */

#ifndef VIEW_H
#define VIEW_H
/*******************************/
/* Includes                    */
/*******************************/
//@{

#include "firehose_include.h"
#include <stdio.h>
#include "../rendering.h"

//@}

/********************************/
/* GLOBAL Variable Declarations */
/********************************/
//@{

//@}

/********************************/
/* GLOBAL Function Declarations */
/********************************/
//@{

// initialize tft shit, sets up the driver
void view_init(void);

// draw ball at new_pos
void view_draw_ball(struct xy_vec const new_pos);

// top is the y-position of the top of the paddle
void view_redraw_paddle(short old_top, short new_top);

// draw the barriers. Draws in same place every time, so no kind of erasing and
// redrawing. Might need to be redrawn if the ball goes on top of them,
// otherwise drawn only during some kind of init.
void view_draw_barriers(void);

void view_remove_ball(struct xy_vec pos);

// draw the stats that are update slowly
void view_draw_slow_stats(int fps, int num_secs);

// draw the stats that are updated rapidly
void view_draw_fast_stats(int score, int num_balls);

//@}

#endif	/* VIEW_H */
