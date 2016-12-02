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
static void frame(void);

/** 
 * Spawn new balls on the screen when appropriate
 */
static void spawn_balls(void);

/**
 * Does slow updates that are neccessary.
 * These updates include game time, frame rate, barrier redraw 
 */
static void per_second_update(void);

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

/**
 * @brief The ticks since when we define the second to have began.
 */
static unsigned int ticks_since_second = 0;

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

void firehose_init(void) {
  // lol we can keep any paddle position from a previous play who cares yolo

  player_score = 0;
  game_time = 0;
  frames_per_second_accum = 0;
  game_ongoing = true;
  ticks_since_second = 0;


  ////////////////////
  /* Configure View */
  ////////////////////

  view_init();
}

void firehose_tick(void) {
  frame();
  spawn_balls();

  if (ticks_since_second == FRAMERATE) {
    per_second_update();
    ticks_since_second = 0;
  } else {
    ticks_since_second += 1;
  }
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

static void frame(void) {
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
}

static void spawn_balls(void) {
  unsigned int i;
  for (i = 0; i < SPAWN_BALLS_PER_TICK; i++) {
    struct xy_vec sacrificed_ball_pos = simulation_spawnBall();
    view_remove_ball(sacrificed_ball_pos);
  }
}

static void per_second_update(void) {
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
    game_ongoing = false; // stop the game, return control to top level
  }
}

//@}
