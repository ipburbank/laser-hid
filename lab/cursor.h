/**
 * @file   cursor.h
 * @author Istvan Burbank, Peter Friend, James Cassell
 * @date   2016-11-18
 * @brief  Cursor controlled by joystick
 */

#ifndef CURSOR_H
#define CURSOR_H

/*******************************/
/* Includes                    */
/*******************************/
//@{

#include "include.h"
#include "parameters.h"

#include "joystick.h"
#include "spatial.h"
#include "rendering.h"

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

/********************************/
/* GLOBAL Variable Declarations */
/********************************/
//@{

/**
 * @brief The current position of the cursor
 */
extern struct spatial_pos cursor_position;

//@}

/********************************/
/* GLOBAL Function Declarations */
/********************************/
//@{

/**
 * @brief Step the cursor logic, moving cursor if needed
 */
void cursor_step(void);

/**
 * @brief Erase the cursor at cursor_position.
 */
void cursor_erase(void);

/**
 * @brief Draw the cursor at cursor_position.
 */
void cursor_draw(void);

//@}

#endif // CURSOR_H

