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

/**
 * The phase shift in pixels between lasers. May only be positive numbers which
 * represent how many pixels after left side of the frame the color should
 * start. Values must be less than `PHASE_SHIFT_PADDING`.
 *
 * One laser should have a zero shift, this is the one to the right of which the
 * others are shifted.
 */
/**
 * @brief Shift to the right of the red laser.
 *
 * How many pixels to the right the red laser is of the laser with zero
 * shift. Must be smaller than PHASE_SHIFT_PADDING.
 */
#define RED_PHASE_SHIFT   (0)

/**
 * @brief Shift to the right of the green laser.
 *
 * How many pixels to the right the red laser is of the laser with zero
 * shift. Must be smaller than PHASE_SHIFT_PADDING.
 */
#define GREEN_PHASE_SHIFT (0)

/**
 * @brief Shift to the right of the blue laser.
 *
 * How many pixels to the right the red laser is of the laser with zero
 * shift. Must be smaller than PHASE_SHIFT_PADDING.
 */
#define BLUE_PHASE_SHIFT  (0)

/**
 * How much wider to make the frame buffer to accomodate the phase shifts.
 */
#define PHASE_SHIFT_PADDING (0)

/* tab is 4 spaces for purposes of text rendering
 */
#define RENDERING_TAB_WIDTH (4)

/**
 * @brief Min voltage to output to drive the y-axis mirror
 *
 * This is in the 10-bit data format accepted by the DAC
 */
#define Y_AXIS_MIN_COMMAND (2.5 * 1023)

/**
 * @brief Max voltage to output to drive the y-axis mirror
 *
 * This is in the 10-bit data format accepted by the DAC
 */
#define Y_AXIS_MAX_COMMAND (5   * 1023)

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

#endif // PARAMETERS_H
