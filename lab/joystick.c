/**
 * @file   joystick.c
 * @author Istvan Burbank, Peter Friend, James Cassell
 * @date   2016-11-10
 * @brief  Analog joystick input
 *
 * This module has no state kept in C because the ADC runs continuously as fast
 * as possible and when the module is polled it just polls the ADC registers.
 */

#include "joystick.h"

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

//@}

/*******************************/
/* GLOBAL Function Definitions */
/*******************************/
//@{

void joystick_init(void) {
  PORTSetPinsDigitalIn(IOPORT_A, BIT_2);

  // ensure starts turned off
  CloseADC10();

  // Open the ADC
  uint32_t const config1 =
    // turn on
    ADC_MODULE_ON |
    // for now we don't care about this value
    ADC_IDLE_CONTINUE |
    // result in hardware buffer registers are formatted as uint16_t
    ADC_FORMAT_INTG16 |
    // internal timer for when to end sampling and begin conversion
    ADC_CLK_AUTO |
    // ADC samples continuously, as fast as it likes
    ADC_AUTO_SAMPLING_ON |
    // you can tell the ADC to hold instead of sample, but we want it to sample
    ADC_SAMP_ON;

  uint32_t const config2 =
    // use internal VREFs
    ADC_VREF_AVDD_AVSS |
    // TODO
    ADC_OFFSET_CAL_DISABLE |
    // scan through channels
    ADC_SCAN_ON |
    // interrrupt every two conversions (an interrupt per compelte joystick
    // state)
    ADC_SAMPLES_PER_INT_2 |
    // don't alternate muxA and muxB, always use muxA
    ADC_ALT_INPUT_OFF;

  uint32_t const config3 =
    // number of ADC clock cycles to sample before beginning conversion
    ADC_SAMPLE_TIME_6 |
    // clock derived from PBCLK
    ADC_CONV_CLK_SYSTEM |
    // ADC clock prescaler: ADC clock period (TAD) is 2 * TPB
    ADC_CONV_CLK_Tcy2;

  // which ports are analog inputs
  uint32_t const configport =
    ENABLE_AN0_ANA | ENABLE_AN1_ANA;

  // which ports we _don't_ scan (scan only AN0 and AN1)
  uint32_t const configscan =
    SKIP_SCAN_AN2 | SKIP_SCAN_AN3 | SKIP_SCAN_AN4 | SKIP_SCAN_AN5 |
    SKIP_SCAN_AN6 | SKIP_SCAN_AN7 | SKIP_SCAN_AN8 | SKIP_SCAN_AN9 |
    SKIP_SCAN_AN10 | SKIP_SCAN_AN11 | SKIP_SCAN_AN12 | SKIP_SCAN_AN13 |
    SKIP_SCAN_AN14 | SKIP_SCAN_AN15;

  OpenADC10(config1, config2, config3, configport, configscan);
}

struct joystick_vect joystick_get_pos(void) {
  // the ADC is a 10 bit output, and we ask for it in "Integer 16 bit", which is
  // implicitly unsigned (see family reference manual register 17-1).

  // these won't overflow because the ADC value is only in the bottom 10 bits
  int16_t const x = (int16_t) ReadADC10(0);
  int16_t const y = (int16_t) ReadADC10(1);

  int16_t const x_centered = x - JOYSTICK_RAW_MIDDLE;
  int16_t const y_centered = y - JOYSTICK_RAW_MIDDLE;

  // these scale factors convert into a uniform scale, so that
  // -JOYSTICK_OUTPUT_RANGE <= output <= JOYSTICK_OUTPUT_RANGE. The scale
  // factors have to be signed so that the multiplication produces a signed
  // value instead of converting the signed operand to an unsigned.
  static int16_t const pos_scale_factor =
    JOYSTICK_OUTPUT_RANGE / JOYSTICK_RAW_POS_RANGE;
  static int16_t const neg_scale_factor =
    JOYSTICK_OUTPUT_RANGE / JOYSTICK_RAW_NEG_RANGE;

  struct joystick_vect current;

  if (x_centered > 0) {
    current.x = x_centered * pos_scale_factor;
  } else {
    current.x = x_centered * neg_scale_factor;
  }

  if (y_centered > 0) {
    current.y = y_centered * pos_scale_factor;
  } else {
    current.y = y_centered * neg_scale_factor;
  }

  return current;
}

inline bool joystick_pushed(void) {
  return PORTReadBits(IOPORT_A, BIT_2) != 0x0;
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
