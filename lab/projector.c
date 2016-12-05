/**
 * @file   pid.h
 * @author Istvan Burbank, Peter Friend, James Cassell
 * @date   2016-11-05
 * @brief  Projector Output Functions
 *
 * The projector consists of a spinning x-scan mirror that scans left to right,
 * and a y-scan mirror that scans from top to bottom. At the start of each row
 * the "row trigger" (a photo transistor output) goes high. After that edge,
 * pixels are clocked out by DMA triggered by the pixel clock. After all of the
 * pixels in that row have been clocked out, the DMA interrupts the CPU which
 * moves the y-scan mirror to the next scan line (allowing it to settle before
 * pixels are clocked out for the next row), the DMA is configured the next row,
 * and the clock is reset.
 *
 * Externally, the photo transistor is latched with the ability to be reset by
 * the CPU. The latched signal triggers an ISR via a Change Notification, which
 * clears the timer (sets it to 0) and calls `trigger_row()` which starts
 * clocking out pixels with dma. After the DMA finishes outputting the row it
 * triggers an interrupt that eventually (not timing critical as extraneous
 * interrupts are ignored by the DMA controller after the transfer is complete)
 * resets the latch (halting the timer) and resets the timer to 0.
 *
 */

#include "projector.h"

/*******************************/
/* LOCAL Macro Definitions     */
/*******************************/
//@{

// DAC Configs
#define DAC_A          (0b0 << 15)
#define DAC_B          (0b1 << 15)
#define DAC_GAIN_VREF  (0b1 << 13)
#define DAC_GAIN_2VREF (0b0 << 13)
#define DAC_ACTIVE     (0b1 << 12)
#define DAC_INACTIVE   (0b0 << 12)

#define PIXEL_DMA_CHN       (0)
#define Y_MIRROR_SPI_CHN    (1)
#define Y_MIRROR_SPI_CONFIG (DAC_A | DAC_GAIN_VREF | DAC_ACTIVE)

/**
 * @brief How many clock ticks the pixel is on
 */
#define PIXEL_ON_TIME (400)
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

/**
 * @brief The current row (scan line) being displayed.
 * This is updated in the ISR and thus is volatile.
 */
volatile uint8_t current_row;

//@}

/*******************************/
/* LOCAL Function Declarations */
/*******************************/
//@{

/**
 * @brief Configure the DMA's source address to output the provided row from the start.
 * Includes setup of timers, interrupts, etc.
 *
 * @param pixel color to display
 */
static void configure_dma_for_row(uint8_t row_number);

/**
 * @brief Trigger outputting currently configured row.
 *
 * Clears Timer1
 */
static void trigger_row(void);

/**
 * @brief Set the y axis mirrors angle to direct the laser to the appropriate place for
 * the given row.
 *
 * @param pixel color to display
 */
static void update_y_axis_position(uint8_t row_number);

/**
 * Set the lasers to the specified color.
 *
 * @param pixel color to display
 */
static void write_pixel(struct color const pixel);

//@}

/*******************************/
/* GLOBAL Variable Definitions */
/*******************************/
//@{

struct color projector_framebuffer[IMAGE_HEIGHT]
                                  [IMAGE_WIDTH + PHASE_SHIFT_PADDING] = {0};

//@}

/*******************************/
/* GLOBAL Function Definitions */
/*******************************/
//@{

void projector_init() {
  ////////////////////////
  /* Set Up Pixel CLock */
  ////////////////////////

  // Open Timer1 with the following configuration:
  //  - T1_ON         :: Timer is turned on
  //  - T1_SOURCE_INT :: Clock source is internal
  //  - T1_PS_1_1     :: Prescalar is 1 to 1 (timer sees PBCLK)
  //
  // The timer's period is used to fine-tune how long each pixel lasts;
  // one pixel displayed every time the timer wraps around.
  OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_1, PIXEL_ON_TIME);

  // set up the pin to reset the Gate Latch, set initially high
  // (the signal is active low)
  PORTSetPinsDigitalOut(IOPORT_A, BIT_3);
  mPORTASetBits(BIT_3);

  ////////////////
  /* Set Up CN  */
  ////////////////

  // see reference manual 12.3.3.1
  CNCONASET = BIT_15; // enable CN on Port A (set bit 15 of CNCON to 1)
  PORTSetPinsDigitalIn(IOPORT_A, BIT_4); // RA4 is input
  unsigned int ignore = PORTA; // clear interrupt
  IPC8SET  = ((unsigned int) 5) << 18; // set CN interrupt priority
                                       // (IPC6<20:18>=5, defaults to 0 at POR)
  IFS1CLR  = BIT_13; // clear CN interrupt flag (set bit 0 of IFS1 to 0)
  CNENASET = BIT_4;  // enable for RA4 (set bit 4 of CNENA to 1)
                     // see family data sheet 11.1.4
  IEC1SET  = BIT_13; // enable CN interrupt (set bit 0 of IEC1 to 1)

  ////////////////
  /* Set Up DMA */
  ////////////////

  // Set up the pins as an output
  PORTSetPinsDigitalOut(IOPORT_B, BIT_0 | BIT_1   // red
                                | BIT_2 | BIT_3   // blue
                                | BIT_4 | BIT_5); // green

  // Configure DMA interrupt vector and enable DMA interrupts on CPU
  INTSetVectorPriority(INT_VECTOR_DMA(PIXEL_DMA_CHN), INT_PRIORITY_LEVEL_5);
  INTSetVectorSubPriority(INT_VECTOR_DMA(PIXEL_DMA_CHN),
                          INT_SUB_PRIORITY_LEVEL_3);
  INTEnable(INT_SOURCE_DMA(PIXEL_DMA_CHN), INT_ENABLED);

  configure_dma_for_row(0);

  ////////////////
  /* Set Up SPI */
  ////////////////

  // SPI Slave Sync output on pin 11
  PPSOutput(1, RPB15, SS1);

  // SPI1 data output on pin 14
  PPSOutput(2, RPB8, SDO1);

  // SPI channel 1 channel A drives the y axis control mirror.
  // The messages we send are 16 bits long (4 control bits, 12 signal bits).
  // The config flags do the following:
  //  - SPICON_MSTEN  :: Set to master mode
  //  - SPICON_MODE16 :: Sets the word size to 16 bits
  //  - SPICON_ON     :: Module ON Control
  //  - SPICON_FRMPOL :: Polarity of frame signal
  //  - SPICON_CKP    :: Reverse clock edges
  //  - SPICON_FRMEN  :: Use automatic framing: Serial clock is always on
  //                     even when no data is being sent and Slave Select/CS
  //                     pulses before each word
  // The fpbDiv = 2 sets baud rate to Fpb / fpbDiv.
  SpiChnOpen(Y_MIRROR_SPI_CHN, SPICON_MSTEN | SPICON_MODE16 | SPICON_ON
             | SPICON_FRMPOL | SPICON_CKP | SPICON_FRMEN, 2);
}

static void trigger_row(void) {
  WriteTimer1(0);
  DmaChnEnable(PIXEL_DMA_CHN);
}

void projector_set_pixel(struct color const color,
                         unsigned int x, unsigned int y) {
  projector_framebuffer[y][x + RED_PHASE_SHIFT  ].red   = color.red;
  projector_framebuffer[y][x + GREEN_PHASE_SHIFT].green = color.green;
  projector_framebuffer[y][x + BLUE_PHASE_SHIFT ].blue  = color.blue;
}

//@}

/*******************************/
/* ISR Definitions             */
/*******************************/
//@{

/**
 * This interrupt is called when all of the pixels from a row have been
 * displayed. The mirror should be moved to the next line, the Timer Gate latch
 * reset, timer reset, and DMA configured for the next row.
 *
 * We also turn off the lasers clearing the last pixel. This could also be
 * achieved by adding an always-black pixel to the end of the row and having DMA
 * output it, an option that should be considered based on the performance of
 * this solution.
 */
void __ISR(_DMA0_VECTOR, IPL5SOFT) EndOfRowHandler(void) {
  // acknowledge the INT controller, we're servicing int
  INTClearFlag(INT_SOURCE_DMA(DMA_CHANNEL0));

  // turn off the lasers
  write_pixel(color_blank);

  current_row = (current_row + 1) % IMAGE_HEIGHT;

  // move the y-axis mirror to the next row
  update_y_axis_position(current_row);

  // reset the Timer Gate Latch
  mPORTAClearBits(BIT_3);     // set the active low reset signal
  Nop(); Nop(); Nop(); Nop(); // wait for reset to occur - 4 nops is 100ns, an
                              // upper bound on the time the signal has to be
                              // low for the latch to be happy
  mPORTASetBits(BIT_3);       // unset the active low reset signal

  // prepare the DMA to output the next row
  configure_dma_for_row(current_row);
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL5SOFT) LightArrival(void) {
  if ((PORTA & BIT_4) == BIT_4) {
    // if the bit is high, the light just went off. The light disappearing is a
    // low-to-high edge.
    trigger_row();
  }

  // clear the interrupt flag
  IFS1CLR = BIT_13;
}

//@}

/*******************************/
/* LOCAL Function Definitions  */
/*******************************/
//@{

static void configure_dma_for_row(uint8_t row_number) {

  // Open dma channel, priority 3, in default mode
  // (transfer the block and stop)
  DmaChnOpen(PIXEL_DMA_CHN, 3, DMA_OPEN_DEFAULT);

  // Set up the DMA transfer
  DmaChnSetTxfer(PIXEL_DMA_CHN,                              // channel number
                 (void *) projector_framebuffer[row_number], // Source
                 (void *) &LATB,                             // Destination
                 IMAGE_WIDTH,                                // source size
                 1,                                          // dest size
                 1);                                         // cell size

  // Start DMA channel transfer on IRQ from timer 1
  // i.e. transfer a pixel every time the timer hits its period
  DmaChnSetEventControl(PIXEL_DMA_CHN, DMA_EV_START_IRQ(_TIMER_1_IRQ));

  // Create an interrupt when the transfer completes and
  // Enable the channel interrupt in the MCU INT controller.
  //
  // This interrupt indicates that the entire row of pixels has been displayed
  // and that the next row should be configured.
  DmaChnSetEvEnableFlags(PIXEL_DMA_CHN, DMA_EV_BLOCK_DONE);
  DmaChnIntEnable(PIXEL_DMA_CHN);
}

static void update_y_axis_position(uint8_t row_number) {
  // Convert the row number into the correct DAC output
  // float avoided by rewrite: (row_number / IMAGE_HEIGHT) * (MAX_COMMAND - MIN_COMMAND) + MIN_COMMAND
  uint16_t mirror_position =
    (row_number * (Y_AXIS_MAX_COMMAND - Y_AXIS_MIN_COMMAND)) / IMAGE_HEIGHT
    + Y_AXIS_MIN_COMMAND;

  // construct the DAC message by adding the DAC config bits on to the message
  uint16_t dac_word = Y_MIRROR_SPI_CONFIG | mirror_position;

  // put the message in the DAC FIFO
  WriteSPI1(dac_word);
}

static void write_pixel(struct color const pixel) {
  // pixel needs to be accessed as raw bits which is achieved using this
  // disgusting hack
  union {
    struct color pixel;
    uint8_t pixel_as_int;
  } pixel_to_int_converter;
  pixel_to_int_converter.pixel = pixel;
  uint8_t pixel_int = pixel_to_int_converter.pixel_as_int;

  // set/clear the bits appropriately
  // The masking is unecessary in the first case and could be avoided in the
  // second case but is included in both because it is more explicit and more
  // clear.
  mPORTBSetBits  ( ((unsigned int) pixel_int) & 0XFF);
  mPORTBClearBits(~((unsigned int) pixel_int) & 0XFF);
}

//@}
