/**
 * @file   joystick.h
 * @author Istvan Burbank, Peter Friend, James Cassell
 * @date   2016-11-10
 * @brief  Analog joystick input
 */

#ifndef JOYSTICK_H
#define JOYSTICK_H

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
 * @brief vector representing the joystick state.
 *
 * "cart" refers to "cartesian" as opposed to polar. The coordinate system
 * origin is in the upper-left corner, thus a negative x value is the joystick
 * is left of neutral and a negative y value is the joystick being up from
 * neutral.
 */
struct vect_cart {
  int16_t x;
  int16_t y;
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

/**
 * @brief Initialize joystick state and peripherals.
 */
void joystick_init(void);

/**
 * @brief Get most recent recorded joystick direction.
 *
 * The ADC is constantly converting, this returns the most recently sampled
 * vector of the joystick position. This should be polled as frequently as
 * desired, there is no notion of "consuming" a value.
 */
struct vect_cart joystick_get_vect(void);

//@}

#endif // JOYSTICK_H

