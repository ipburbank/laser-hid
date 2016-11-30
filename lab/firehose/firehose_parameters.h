/**
 * @file   firehose_parameters.h
 * @author Istvan Burbank, Peter Friend, James Cassell
 * @date   2016-10-11
 * @brief  Implementation of drink from the firehose game logic
 * 
 * Pinout:
 *
 */

#ifndef FIREHOSE_PARAMETERS_H
#define FIREHOSE_PARAMETERS_H
/*******************************/
/* Includes                    */
/*******************************/
//@{

#include "../color.h"

#include "../parameters.h"

//@}

/*******************************/
/* GLOBAL Macro Definitions    */
/*******************************/
//@{

#define NUM_BALLS (385) /// Max number of balls in the simulation

#define FRAMERATE (15) /// Frames per second

#define BARRIER_DIST_FROM_LEFT (80)
#define BARRIER_LENGTH (60)

#define PADDLE_HEIGHT (20)
#define PADDLE_DISTANCE_FROM_LEFT (20)
#define DISABLE_PADDLE_OPTIMIZATION (false)

#define SPAWN_TIME (10) /// time between ball spawns in msec
#define GAME_LENGTH (120) // How long user gets to play the game

//@}

/*******************************/
/* GLOBAL Variable Definitions */
/*******************************/
//@{

//@}

/********************************/
/* GLOBAL Variable Declarations */
/********************************/
//@{

extern struct color const ball_color_fg;

extern struct color const paddle_color_fg;

extern struct color const barrier_color_fg;

extern struct color const stats_neg_color_fg;

extern struct color const stats_color_fg;

extern struct color const game_over_color_fg;

//@}

/********************************/
/* GLOBAL Function Declarations */
/********************************/
//@{

//@}

#endif // FIREHOSE_PARAMETERS_H
