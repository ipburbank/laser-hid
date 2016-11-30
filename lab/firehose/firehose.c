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

#include "firehose.h"

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
static short paddle_top_position;
// Keep track of previous paddle top position
static short previous_paddle_top_position;

// Players current score
static int player_score;

// Current game time
static int game_time;

// Accumulator for frame rate per second
static int frames_per_second_accum;

/**
 * @brief Whether the game is running/whether game threads are scheduled
 *
 * Starts out true and becomes false when the game ends for any reason. When
 * this becomes false control leaves the main firehose function.
 */
static bool game_ongoing = true;

static struct pt pt_frameThread, pt_spawnThread, pt_perSecondUpdateThread;

static volatile bool frame_trigger_flag;

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

/** 
 * The main function configures peripherals, interrupts, and threads, then
 * infinately runs the thread scheduler.
 */
void firehose_main(void) {

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

  ///////////////////////
  /* Run the scheduler */
  ///////////////////////

  // the ordering might be important in laser-hid (as opposed to lab3 proper)
  // because the game_ongoing flag will be set to false in
  // perSecondUpdateThread, and we don't want the other threads to get a chance
  // to run after that happens
  while (game_ongoing) {
    PT_SCHEDULE(protothread_frameThread(&pt_frameThread));
    PT_SCHEDULE(protothread_spawnThread(&pt_spawnThread));
    PT_SCHEDULE(protothread_perSecondUpdateThread(&pt_perSecondUpdateThread));
  }

  // end timer 3
  CloseTimer3();

  // end pthreads, return to normal operation
  // TODO end pthreads
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
    struct joystick_vect const stick_pos = joystick_get_pos();
    // un-center joystick so it is in [0, 2*JOYSTICK_OUTPUT_RANGE] instead of
    // [-JOYSTICK_OUTPUT_RANGE, JOYSTICK_OUTPUT_RANGE]
    uint16_t const uncentered_stick =
      (uint16_t) stick_pos.x + JOYSTICK_OUTPUT_RANGE;
    /* range of paddle_top_position is [0, IMAGE_HEIGHT - PADDLE_HEIGHT]. This
     * arithmetic scales the joystick output to this range without using a
     * floating point number by doing careful operation ordering. It is
     * mathematically equivalent to: (uncentered_stick /
     * (2*JOYSTICK_OUTOUT_RANGE)) * (IMAGE_HEIGHT - PADDLE_HEIGHT)
     */
    paddle_top_position =
      (IMAGE_HEIGHT - PADDLE_HEIGHT) *
      uncentered_stick /
      2 * JOYSTICK_OUTPUT_RANGE;
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
      // Display "END GAME" on the TFT
      rendering_setCursor(BARRIER_DIST_FROM_LEFT, IMAGE_HEIGHT/2);
      rendering_setTextColor(game_over_color_fg);
      rendering_writeString("GAME OVER");
      // Yield for three seconds
      PT_YIELD_TIME_msec(3000);
      game_ongoing = false; // stop the game, return control to top level
    }
    // Yield for one second
    PT_YIELD_TIME_msec(1000);
    // never exit while
  }
  PT_END(pt);
}

//@}
