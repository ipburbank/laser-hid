/**
 * @file   color.h
 * @author Istvan Burbank, Peter Friend, James Cassell
 * @date   2016-11-28
 * @brief  Color types and utilities.
 *
 */

#ifndef COLOR_H
#define COLOR_H

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
 * A pixel in RGB color space represented in six bits. Bits not defined here
 * should not be set.
 * A GCC-specific pragma is used to ensure pixels are stored in only one byte.
 */
#pragma pack(1)
struct color {
  char red   : 2;
  char blue  : 2;
  char green : 2;
};

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

extern struct color const color_blank;

//@}

/********************************/
/* GLOBAL Function Declarations */
/********************************/
//@{

/**
 * @brief Return true if a and b represent exactly the same color
 *
 * Two structs that represent the same color might not be the same bits because
 * the spec allows "padding" needed to satisfy alignment to contain any value.
 */
bool color_equal(struct color const a,
                 struct color const b);

//@}

#endif // COLOR_H

