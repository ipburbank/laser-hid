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

// Size of buffer for UART
#define BUFFERSIZE (12)

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

// Recieves string from user and prints it to the display
static PT_THREAD(protothread_uartThread(struct pt *pt));

//@}

/*******************************/
/* LOCAL Variable Declarations */
/*******************************/
//@{

// Variable used to kepp track of the string we read over UART
static char cmd[BUFFERSIZE];
// Red level from user
static int red;
// Green level from user
static int green;
// Blue level from user
static int blue;

//@}

/*******************************/
/* GLOBAL Variable Definitions */
/*******************************/
//@{

static struct pt pt_uartThread, pt_input, pt_DMA_output;

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
  PT_INIT(&pt_uartThread);

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
    PT_SCHEDULE(protothread_uartThread(&pt_uartThread));
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

static PT_THREAD(protothread_uartThread(struct pt *pt)) {
  PT_BEGIN(pt);

  while (1) {

    // Send propmt to DMA for serial communication and spawn thread to print
    sprintf(PT_send_buffer, "String to display:");
    PT_SPAWN(pt, &pt_DMA_output, PT_DMA_PutSerialBuffer(&pt_DMA_output));

    // create thread to deal with input
    PT_SPAWN(pt, &pt_input, PT_GetSerialBuffer(&pt_input));

    // parse the string returned to us
    sscanf(PT_term_buffer, "%s %d %d %d", cmd, &red, &blue, &green);

    // Erase the old morse code string
    rendering_drawMorseString(0, 0, cmd, color_blank);

    // Decoding the command that we recieved
    // Draw the new line of morse code
    struct color output_color = {
      .red = red,
      .blue = blue,
      .green = green
    };
    rendering_drawMorseString(0, 0, cmd, output_color);
    // never exit while
  } // END WHILE(1)
  PT_END(pt);
}
//@}

