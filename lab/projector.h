/**
 * @file   pid.h
 * @author Istvan Burbank, Peter Friend, James Cassell
 * @date   2016-11-05
 * @brief  Projector Output Functions
 *
 * This module contains the functions used to output using the projector.  A
 * carousel spins with four mirrors on it. For each mirror the optical sensor is
 * pulsed signaling the start of the next row. Once that signal is received, the
 * pixels are clocked out by DMA. At the end of the row, the mirror position is
 * updated to the next row.
 *
 */

#ifndef PROJECTOR_H
#define PROJECTOR_H

/*******************************/
/* Includes                    */
/*******************************/
//@{

#include "include.h"

#include "parameters.h"
#include "color.h"

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
 * @brief Initialize projector peripherals, variables etc. MUST be called before
 * calling any other projector functions!
 */
void projector_init( void );

void projector_start();

/**
 * @brief Set the color of a pixel at the specified location.
 *
 * Once a pixels value is changed, the new value will be projected the next time
 * the pixel is output (there is no v-sync).
 *
 * @param color Color to be displayed
 * @param x     Number of pixels from left side
 * @param y     Number of pixels from the top
 */
void projector_set_pixel(struct color const color,
                         unsigned int x, unsigned int y);

//@}

#endif // PROJECTOR_H

