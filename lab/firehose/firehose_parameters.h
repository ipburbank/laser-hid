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

/**
 * @brief Target frames per second
 *
 * The length of a tick is generally (1 second / FRAMERATE), but it is possible
 * that a tick can run too long, causing the next one to be skipped.
 */
#define FRAMERATE (15) /// Frames per second

#define BARRIER_DIST_FROM_LEFT (IMAGE_WIDTH / 5)
#define BARRIER_LENGTH (IMAGE_HEIGHT / 4)

#define PADDLE_HEIGHT (IMAGE_HEIGHT / 5)
#define PADDLE_DISTANCE_FROM_LEFT (2)
#define DISABLE_PADDLE_OPTIMIZATION (false)

#define SPAWN_BALLS_PER_TICK (66)

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
