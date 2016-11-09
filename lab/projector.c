/**
 * @file   pid.h
 * @author Istvan Burbank, Peter Friend, James Cassell
 * @date   2016-11-05
 * @brief  Projector Output Functions
 *
 */

#include "projector.h"

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

struct projector_color projector_framebuffer[IMAGE_HEIGHT][IMAGE_WIDTH] = {0};

//@}

/*******************************/
/* GLOBAL Function Definitions */
/*******************************/
//@{

bool projector_pixel_equal(struct pixel const a, struct pixel const b) {
  return (a.red == b.red) && (a.green == b.green) && (a.blue == b.blue);
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
