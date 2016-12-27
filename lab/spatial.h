/**
 * @file   spatial.h
 * @author Istvan Burbank, Peter Friend, James Cassell
 * @date   2016-11-18
 * @brief  Spatial types and relationships toolkit
 */

#ifndef SPATIAL_H
#define SPATIAL_H

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

//@}

/*********************************/
/* GLOBAL Type(def) Declarations */
/*********************************/
//@{

/**
 * @brief A spatial position within the display.
 *
 * 0 <= x <= IMAGE_WIDTH, 0 <= y <= IMAGE_HEIGHT
 *
 * The type of these are uint16_t in hopes that the struct will be small.
 */
struct spatial_pos {
  uint16_t x;
  uint16_t y;
};

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

#endif // SPATIAL_H

