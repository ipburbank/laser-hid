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

//@}

/*******************************/
/* GLOBAL Macro Definitions    */
/*******************************/
//@{

#define NUM_BALLS (385) /// Max number of balls in the simulation

#define FRAMERATE (15) /// Frames per second

// tft specific defines
#define TFT_WIDTH  (ILI9340_TFTHEIGHT)
#define TFT_HEIGHT (ILI9340_TFTWIDTH)

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

//@}

/********************************/
/* GLOBAL Function Declarations */
/********************************/
//@{

//@}

#endif // FIREHOSE_PARAMETERS_H
