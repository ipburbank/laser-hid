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

//@}

/*******************************/
/* GLOBAL Macro Definitions    */
/*******************************/
//@{

/**
 * Projected image height and width in pixels.
 */
#define IMAGE_HEIGHT (240)
#define IMAGE_WIDTH  (352)

//@}

/*********************************/
/* GLOBAL Type(def) Declarations */
/*********************************/
//@{

/**
 * A pixel in RGB color space represented in six bits. Bits not defined here
 * should not be set.
 */
struct pixel {
  unsigned int red   : 2;
  unsigned int blue  : 2;
  unsigned int green : 2;
};

//@}

/*******************************/
/* GLOBAL Variable Definitions */
/*******************************/
//@{

/**
 * Once a pixels value is changed in this array the new value will be projected
 * the next time the pixel is output (there is no v-sync).
 */
extern struct pixel framebuffer[IMAGE_HEIGHT][IMAGE_WIDTH] = {0};

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

#endif // PROJECTOR_H

