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
#include "firehose/firehose.h"

//@}

/*******************************/
/* Board configuration         */
/*******************************/
//@{

// pragma configuration must match SYS_CLOCK in parameters.h

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

static volatile bool tick_trigger_flag = false;

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

    //////////////////////
  /* Configure Timers */
  //////////////////////
  // set up timer3 to trigger at FRAMERATE
  // sys_clock/64 is the clock to the timer after the configured divison
  OpenTimer3(T3_ON | T3_SOURCE_INT | T3_PS_1_64, (SYS_CLOCK/64) / FRAMERATE);
  ConfigIntTimer3(T3_INT_ON | T3_INT_PRIOR_2);

  //////////////////////////////////
  /* Setup system wide interrupts */
  //////////////////////////////////

  INTEnableSystemMultiVectoredInt();

  ///////////////////////
  /* Run the scheduler */
  ///////////////////////

  while (1) {
    while (!tick_trigger_flag) {
      // spin until tick is ready
    }
    // reset the flag early so that if the tick 1 arrives while we are working
    // on the computation of tick 0, as soon tick 0 ends we will go on to work
    // on tick 1, instead of resetting the flag at the end and waiting for tick
    // 2. This will work ok on the assumption that a tick running over is
    // occasional, because if it happens regularly then ticks will rarely start
    // on time.
    tick_trigger_flag = false;

    // run the firehose game
    firehose_init();
    bool firehose_running = true;
    while (firehose_running) {
      firehose_running = firehose_tick();
    }
  }
} // main

//@}

/*******************************/
/* ISR Definitions             */
/*******************************/
//@{

void __ISR(_TIMER_3_VECTOR, IPL2AUTO) Timer3Handler(void) {
  // clear the interrupt flag
  mT3ClearIntFlag();

  // indicate that it is time to execute a frame
  tick_trigger_flag = true;
}

//@}

/*******************************/
/* LOCAL Function Definitions  */
/*******************************/
//@{

//@}

