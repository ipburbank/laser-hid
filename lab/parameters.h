/**
 * @file   parameters.h
 * @author Istvan Burbank, Peter Friend, James Cassell
 * @date   2016-10-23
 * @brief  Compile-time parameters to control system.
 *
 * Parameters set here are ones that the program adapts to. The set of all
 * parameters should completely configure the reasonably variable parts of the
 * system.
 */

#ifndef PARAMETERS_H
#define PARAMETERS_H

/**
 * Projected image height and width in pixels.
 */
#define IMAGE_HEIGHT (50)
#define IMAGE_WIDTH  (50)

/* tab is 4 spaces for purposes of text rendering
 */
#define RENDERING_TAB_WIDTH (4)

/*
 * Controls conversion of ADC values into nice direction vectors.
 */

// size of range of positive part of joystick
/* Preliminarily set to 512 because this is half of the range of the unsigned 10
   bit ADC. Thus from 512 to 1024 is suppoed to be a positive setting. */
#define JOYSTICK_RAW_POS_RANGE (1024/2)
// where in the ADC range the neutral position of the joystick is
#define JOYSTICK_RAW_MIDDLE (1024/2)
// size of range of negative part of joystick.
/* Preliminarily set to 512 because this is half of the range of the unsigned 10
   bit ADC. Thus from 0 to 512 is suppoed to be a negative setting. */
#define JOYSTICK_RAW_NEG_RANGE (1024/2)
// range of positive or negative part of output value of joystick (full range is
// twice this)
#define JOYSTICK_OUTPUT_RANGE (100)

/*
 * Controls joystick position to cursor movement rate connection
 */
// joystick_units / (pixel/tick), i.e. dividing joystick units by this gives
// pixels per tick rate. A value of 1024 means the joystick must be pushed all
// the way to an extreme to move a pixel per tick, smaller values mean the
// joystick must be moved less to get movement and can be moved further for
// faster movement.
#define CURSOR_JOYSTICK_UNITS_PER_PIXEL_PER_TICK (JOYSTICK_OUTPUT_RANGE/2)

#define GUI_BUTTON_BORDER_COLOR {.red = 0x2, .green = 0x0, .blue = 0x0}

#define GUI_TEXT_COLOR {.red = 0x0, .green = 0x0, .blue = 0x2}
#define GUI_TEXT_SIZE (2)

#define BG_COLOR {.red = 0x0, .green = 0x0, .blue = 0x0}

#endif // PARAMETERS_H
