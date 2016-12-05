/**
 * @file   firehose.h
 * @author Istvan Burbank, Peter Friend, James Cassell
 * @date   2016-11-30
 * @brief  Implementation of drink from the firehose game logic
 * 
 * Pinout:
 *
 */

#ifndef FIREHOSE_H
#define FIREHOSE_H
/*******************************/
/* Includes                    */
/*******************************/
//@{

#include "../include.h"

#include "firehose_parameters.h"

////////////////////////////////////
// graphics libraries
#include "../rendering.h"
////////////////////////////////////

#include "view.h"
#include "simulation.h"

#include "../joystick.h"

//@}

/*******************************/
/* GLOBAL Macro Definitions    */
/*******************************/
//@{

//@}

/*********************************/
/* GLOBAL Type(def) Declarations */
/*********************************/
//@{

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

/**
 * @brief Initialize the firehose game before every play.
 *
 * Each time the game ends, must be called before firehose_tick is called again.
 */
void firehose_init(void);

/**
 * @brief Run a tick of the firehose game.
 * @return True if the game is continuing, false if the game has ended.
 *
 * After the game has ended, firehose_init must be called again before this can
 * be called.
 */
bool firehose_tick(void);

//@}

#endif // FIREHOSE_H

