/**
 * @file   firehose.c
 * @author Istvan Burbank, Peter Friend, James Cassell
 * @date   2016-09-12
 * @brief  Implementation of ECE4760 Lab2, a DTMF Dialer
 * 
 * Resources:
 *  Clock 1   - ADC
 * 
 * Pinout:
 *
 */

/*******************************/
/* Includes                    */
/*******************************/
//@{

#include "include.h"

////////////////////////////////////
// clock AND protoThreads configure!
// You MUST check this file!
#include "config.h"
// threading library
#include "pt_cornell_1_2_1.h"
////////////////////////////////////

////////////////////////////////////
// graphics libraries
#include "tft_gfx.h"
#include "tft_master.h"
////////////////////////////////////

//#include "view.h"
#include "simulation.h"
#include "sound.h"

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

/** 
 * Update the ball simulation, redraw the balls, and update score, ball count,
 * catcher, etc on the TFT
 */
static PT_THREAD(protothread_frameThread(struct pt *pt));

/** 
 * Spawn new balls on the screen when appropriate
 */
static PT_THREAD(protothread_spawnThread(struct pt *pt));

/**
 * Does slow updates that are neccessary.
 * These updates include game time, frame rate, barrier redraw 
 */
static PT_THREAD(protothread_perSecondUpdateThread(struct pt *pt));

//@}

/*******************************/
/* LOCAL Variable Declarations */
/*******************************/
//@{

// Describes the position of the top of the paddle in pixel position
short paddle_top_position;
// Keep track of previous paddle top position
short previous_paddle_top_position;

// Players current score
int player_score;

// Current game time
int game_time;

// Accumulator for frame rate per second
int frames_per_second_accum;

//@}

/*******************************/
/* GLOBAL Variable Definitions */
/*******************************/
//@{

static struct pt pt_frameThread, pt_spawnThread, pt_perSecondUpdateThread;

volatile bool frame_trigger_flag;

//@}

/*******************************/
/* GLOBAL Function Definitions */
/*******************************/
//@{

/** 
 * The main function configures peripherals, interrupts, and threads, then
 * infinately runs the thread scheduler.
 */
int main(void) {

  ///////////////////////
  /* Set Up PT Threads */
  ///////////////////////

  PT_setup();
  PT_INIT(&pt_frameThread);
  PT_INIT(&pt_spawnThread);
  PT_INIT(&pt_perSecondUpdateThread);

  //////////////////////
  /* Configure Timers */
  //////////////////////

  // set up timer3 to trigger at FRAMERATE
  // sys_clock/64 is the clock to the timer after the configured divison
  OpenTimer3(T3_ON | T3_SOURCE_INT | T3_PS_1_64, (sys_clock/64) / FRAMERATE);
  ConfigIntTimer3(T3_INT_ON | T3_INT_PRIOR_2);

  ////////////////////
  /* Configure View */
  ////////////////////

  view_init();
  view_draw_barriers();

  //////////////////////
  /* Configure Paddle */
  //////////////////////

  paddle_init();

  //////////////////////////////////
  /* Setup system wide interrupts */
  //////////////////////////////////

  INTEnableSystemMultiVectoredInt();
  /////////////////////
  /* Configure Sound */
  /////////////////////

  sound_init();

  ///////////////////////
  /* Run the scheduler */
  ///////////////////////

  while (1) {
    PT_SCHEDULE(protothread_frameThread(&pt_frameThread));
    PT_SCHEDULE(protothread_spawnThread(&pt_spawnThread));
    PT_SCHEDULE(protothread_perSecondUpdateThread(&pt_perSecondUpdateThread));
  }
} // main

//@}

/*******************************/
/* ISR Definitions             */
/*******************************/
//@{

void __ISR(_TIMER_3_VECTOR, ipl2) Timer3Handler(void) {
  // clear the interrupt flag
  mT3ClearIntFlag();

  // indicate that it is time to execute a frame
  frame_trigger_flag = true;
}

//@}

/*******************************/
/* LOCAL Function Definitions  */
/*******************************/
//@{

static PT_THREAD(protothread_frameThread(struct pt *pt)) {
  PT_BEGIN(pt);

  while (1) {
    // wait until it is time to produce a frame
    PT_WAIT_UNTIL(pt, frame_trigger_flag);

    // reset the flag
    frame_trigger_flag = false;

    // Update the paddle position based on controller
    previous_paddle_top_position = paddle_top_position;
    paddle_top_position = (TFT_HEIGHT - PADDLE_HEIGHT) * (paddle_poll()) / 1023.0;
    // draw new paddle position
    view_redraw_paddle(previous_paddle_top_position, paddle_top_position);

    // for each ball update it and draw it
    int ball_index;
    for(ball_index = 0; ball_index < NUM_BALLS; ball_index++) {
      // if this ball is not on the field don't bother doing anything
      if(!balls[ball_index].is_on_field) {
        continue;
      }

      // remove the ball's old position
      view_remove_ball(balls[ball_index].position);

      simulation_substep(ball_index);

      // check for score updates and play related sounds
      int score_change = simulation_score_substep(ball_index, paddle_top_position);
      player_score += score_change;
      // play sounds based on scoring
      if (score_change > 0){
        sound_play(PLUS_ONE);
      }
      else if (score_change < 0){
        sound_play(MINUS_ONE);
      }

      // if the ball wasn't erased draw its new position
      if(balls[ball_index].is_on_field) {
        view_draw_ball(balls[ball_index].position);
      }
    }

    // draw the scores

    // update the number of balls on screen
    // Update that we have completed a frame
    frames_per_second_accum++;
    
    // Update score and number of balls on screen
    view_draw_fast_stats(player_score, simulation_num_spawned_balls);

    // yield so that even if we are running behind the framerate other threads
    // still get a chance to run
    PT_YIELD(pt);

    // never exit while
  } // END WHILE(1)
  PT_END(pt);
} // tnhread

static PT_THREAD(protothread_spawnThread(struct pt *pt)) {
  PT_BEGIN(pt);

  while (1) {
    struct xy_vec sacrificed_ball_pos = simulation_spawnBall();
    view_remove_ball(sacrificed_ball_pos);

    PT_YIELD_TIME_msec(SPAWN_TIME);
    // never exit while
  } // END WHILE(1)
  PT_END(pt);
}

static PT_THREAD(protothread_perSecondUpdateThread(struct pt *pt)) {
  PT_BEGIN(pt);

  while (1) {
    // update game time elapsed and redraw game time and framerate
    game_time++;
    view_draw_slow_stats(frames_per_second_accum, game_time);
    // reset accumulator
    frames_per_second_accum = 0;
    // redraw barriers incase they got into a fight with some balls
    view_draw_barriers();
    // Check if we have reached the end of the game
    if(GAME_LENGTH <= game_time){
      // Play sound indicating game is over
      sound_play(END_GAME);
      // Display "END GAME" on the TFT
      tft_setCursor(BARRIER_DIST_FROM_LEFT, TFT_HEIGHT/2);
      tft_setTextColor(ILI9340_RED);
      tft_writeString("GAME OVER");
      while(1){} // Just stop
    }
    // Yield for one second
    PT_YIELD_TIME_msec(1000);
    // never exit while
  }
  PT_END(pt);
}

//@}
