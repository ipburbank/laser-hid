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

#include "projector.h"

//@}

/*******************************/
/* Board configuration         */
/*******************************/
//@{

// 40 MHz
#pragma config FNOSC = FRCPLL, POSCMOD = OFF
#pragma config FPLLIDIV = DIV_2, FPLLMUL = MUL_20 //40 MHz
#pragma config FPBDIV = DIV_1, FPLLODIV = DIV_2 // PB 40 MHz
#pragma config FWDTEN = OFF,  FSOSCEN = OFF, JTAGEN = OFF

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

