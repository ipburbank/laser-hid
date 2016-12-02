/**
 * @file   main.c
 * @author Istvan Burbank, Peter Friend, James Cassell
 * @date   2016-11-01
 * @brief  Entry point of ECE4760 Laser HID project
 *
 * Resources:
 *
 * Pinout:
 */

/*******************************/
/* Includes                    */
/*******************************/
//@{

#include "include.h"

#include "parameters.h"

////////////////////////////////////
// clock AND protoThreads configure!
// You MUST check this file!
#include "config.h"
// threading library
#include "pt_cornell_1_2_1.h"
////////////////////////////////////

#include "projector.h"
#include "rendering.h"

//@}

/*******************************/
/* LOCAL Macro Definitions     */
/*******************************/
//@{

//@}

/*******************************/
/* LOCAL typdef Dececlarations */
/*******************************/
//@{

//@}

/*******************************/
/* LOCAL Function Declarations */
/*******************************/
//@{

//@}

/*******************************/
/* LOCAL Variable Declarations */
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

int main(void) {
  //////////////////////
  /* Set Up Projector */
  //////////////////////

  projector_init();

  //////////////////////////////////
  /* Setup system wide interrupts */
  //////////////////////////////////

  INTEnableSystemMultiVectoredInt();

  ///////////////////////
  /* Run the scheduler */
  ///////////////////////

  while (1) {
    rendering_fillRect(0, 0,
                       IMAGE_WIDTH / 3, IMAGE_HEIGHT,
                       color_purest_red);
    rendering_fillRect(IMAGE_WIDTH / 3, 0,
                       IMAGE_WIDTH / 3, IMAGE_HEIGHT,
                       color_purest_green);
    rendering_fillRect((IMAGE_WIDTH / 3) * 2, 0,
                       IMAGE_WIDTH / 3, IMAGE_HEIGHT,
                       color_purest_blue);
  }
} // main

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

