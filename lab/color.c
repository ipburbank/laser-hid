/**
 * @file   color.c
 * @author Istvan Burbank, Peter Friend, James Cassell
 * @date   2016-11-28
 * @brief  Projector Output Functions
 *
 */

#include "color.h"

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

struct color const color_blank = {0};

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

bool color_equal(struct color const a,
                 struct color const b) {
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
