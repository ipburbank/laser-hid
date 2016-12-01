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
 * @brief One-time initialization of the firehose game
 *
 * Called once per reset, even if the game is played multiple times. Must be
 * called before firehose_main()
 */
void firehose_init(void);

/**
 * @brief Run the firhose game, return when game is finished.
 *
 * Runs protothreads internally. firehose_init() must be called before this is
 * run for the first time.
 */
void firehose_main(void);

//@}

#endif // FIREHOSE_H

