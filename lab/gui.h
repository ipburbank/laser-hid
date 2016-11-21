/**
 * @file   gui.h
 * @author Istvan Burbank, Peter Friend, James Cassell
 * @date   2016-11-21
 * @brief  Relatively high level GUI
 */

#ifndef GUI_H
#define GUI_H

/*******************************/
/* Includes                    */
/*******************************/
//@{

#include "include.h"
#include "parameters.h"

#include "projector.h"
#include "cursor.h"
#include "joystick.h"
#include "spatial.h"

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

//@}

/********************************/
/* GLOBAL Function Declarations */
/********************************/
//@{

/**
 * @brief Draw a button containing provided text.
 *
 * @param top_left The top left corner of the button
 *
 * @param width The width in pixels of the button rectangle
 *
 * @param height The height in pixels of the button rectangle
 *
 * @param text The text to draw in the button, null terminated
 *
 * The button is a transparent rectangle with border color
 * GUI_BUTTON_BORDER_COLOR containing the text with color GUI_TEXT_COLOR at size
 * GUI_TEXT_SIZE. Nothing is done to prevent the text from overflowing the
 * border, or to keep the border at a reasonable size relative to the text.
 */
void gui_draw_button(struct spatial_pos const top_left,
                     short const width, short const height, char* text);

/**
 * @brief Poll whether the button is pushed at this moment.
 *
 * Parameters are the same as the parameters to gui_draw_button.
 */
bool gui_button_pushed(struct spatial_pos const top_left,
                       short const width, short const height);

//@}

#endif // GUI_H

