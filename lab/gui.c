/**
 * @file   gui.c
 * @author Istvan Burbank, Peter Friend, James Cassell
 * @date   2016-11-21
 * @brief  Relatively high level GUI
 */

#include "gui.h"

/*******************************/
/* LOCAL Macro Definitions     */
/*******************************/
//@{

//@}

/********************************/
/* LOCAL Type(def) Declarations */
/********************************/
//@{

//@}

/*******************************/
/* LOCAL Variable Definitions  */
/*******************************/
//@{

//@}

/*******************************/
/* LOCAL Function Declarations */
/*******************************/
//@{

//@}

/*******************************/
/* GLOBAL Variable Definitions */
/*******************************/
//@{

//@}

/*******************************/
/* GLOBAL Function Definitions */
/*******************************/
//@{

void gui_draw_button(struct spatial_pos const top_left,
                     short const width, short const height, char* text) {
  // draw the rectangle
  struct projector_color const border_color = GUI_BUTTON_BORDER_COLOR;
  rendering_drawRect(top_left.x, top_left.y, width, height, border_color);

  // draw the text
  struct projector_color const text_color = GUI_TEXT_COLOR;
  rendering_setTextColor(text_color);
  rendering_setTextSize(GUI_TEXT_SIZE);
  rendering_setCursor(top_left.x + 1, top_left.y + 1);
  rendering_writeString(text);
}

bool gui_button_pushed(struct spatial_pos const top_left,
                       short const width, short const height) {
  return cursor_position.x > top_left.x &&
    cursor_position.x < top_left.x + width &&
    cursor_position.y > top_left.y &&
    cursor_position.y < top_left.y + height &&
    joystick_pushed();
}

void gui_erase_button(struct spatial_pos const top_left,
                      short const width, short const height) {
  struct projector_color const bg_color = BG_COLOR;
  rendering_drawRect(top_left.x, top_left.y, width, height, bg_color);
}

//@}

/*******************************/
/* ISR Definitions             */
/*******************************/
//@{

//@}

/*******************************/
/* LOCAL Function Definitions  */
/*******************************/
//@{

//@}
