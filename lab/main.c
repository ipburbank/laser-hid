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

  ///////////////////////
  /* Set Up PT Threads */
  ///////////////////////

  PT_setup();

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
    // e.g. PT_SCHEDULE(protothread_uartThread(&pt_uartThread));
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

