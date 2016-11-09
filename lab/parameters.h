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

#endif // PARAMETERS_H
