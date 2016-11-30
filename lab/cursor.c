/**
 * @file   cursor.c
 * @author Istvan Burbank, Peter Friend, James Cassell
 * @date   2016-11-18
 * @brief  Cursor controlled by joystick
 */

#include "cursor.h"

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

struct spatial_pos cursor_position;

//@}

/*******************************/
/* GLOBAL Function Definitions */
/*******************************/
//@{

void cursor_step(void) {
  struct joystick_vect input = joystick_get_pos();

  // convert units. (see comment in parameters.h)
  /* This will result in joystick values closer to 0 than
     CURSOR_JOYSTICK_UNITS_PER_PIXEL_PER_TICK being ignored (i.e. resulting in
     no cursor movement) due to integer arithmatic truncation. */
  int16_t const delta_x = input.x / CURSOR_JOYSTICK_UNITS_PER_PIXEL_PER_TICK;
  int16_t const delta_y = input.y / CURSOR_JOYSTICK_UNITS_PER_PIXEL_PER_TICK;

  // compute new positions.
  /* The current position must be explicitly converted to a signed value
     otherwise they will both be converted to signed, causing negatives in the
     deltas to make a weird thing happen */
  int16_t new_x = (int16_t) cursor_position.x + delta_x;
  int16_t new_y = (int16_t) cursor_position.y + delta_y;

  // make sure we don't go off the screen
  if (new_x > IMAGE_WIDTH) {
    new_x = IMAGE_WIDTH;
  } else if (new_x < 0) {
    new_x = 0;
  }

  if (new_y > IMAGE_HEIGHT) {
    new_y = IMAGE_HEIGHT;
  } else if (new_y < 0) {
    new_y = 0;
  }

  // update actual position
  cursor_position.x = new_x;
  cursor_position.y = new_y;
}

void cursor_erase(void) {
  rendering_drawMinimalCross((short) cursor_position.x,
                             (short) cursor_position.y,
                             color_blank);
}

void cursor_draw(void) {
  rendering_drawMinimalCross((short) cursor_position.x,
                             (short) cursor_position.y,
                             cursor_fg);
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
