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
#include "pt_config.h"
// threading library
#include "../pt_cornell_1_2_1.h"
////////////////////////////////////

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
 * @brief Run the firhose game, return when game is finished.
 *
 * Runs protothreads internally.
 */
void firehose_main(void);

//@}

#endif // FIREHOSE_H

