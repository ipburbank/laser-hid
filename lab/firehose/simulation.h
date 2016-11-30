/**
 * @file   simulation.h
 * @author Istvan Burbank, Peter Friend, James Cassell
 * @date   2016-10-04
 * @brief  Implementation of drink from the firehose game logic
 * 
 * Pinout:
 *
 */

#ifndef SIMULATION_H
#define SIMULATION_H
/*******************************/
/* Includes                    */
/*******************************/
//@{

#include "include.h"

#include "tft_master.h" // for tft size defines

//@}

/*******************************/
/* GLOBAL Macro Definitions    */
/*******************************/
//@{

#define MIN_TICKS_BETWEEN_COLLISIONS 5 /// to avoid capture

#define BALL_COLLISION_RADIUS 2

#define BARRIER_HEIGHT 60
#define BARRIER_X      80

#define DRAG_COEFFICIENT (float2fix16(0.999f))

//@}

/*********************************/
/* GLOBAL Type(def) Declarations */
/*********************************/
//@{

struct ball {
  bool is_on_field; /**< True iff the ball should currently be displayed. */
  unsigned int age; /**< Number of updates since ball was
                       spawned. If the ball is not on the field then
                       the value of this field is undefined. */
  unsigned int ticks_since_collision; /**< Number of updates since ball was
                                         in a collision */
  struct xy_vec position; /**< The x-y position in pixels of the ball on the screen. */
  struct xy_vec velocity; /**< The x and y velocities in pixels/sec on the screen. */
};

//@}

/*******************************/
/* GLOBAL Variable Definitions */
/*******************************/
//@{

struct ball balls[NUM_BALLS]; ///< vector of all the balls in the simulation

extern unsigned int simulation_num_spawned_balls;

//@}

/********************************/
/* GLOBAL Variable Declarations */
/********************************/
//@{

//@}

/********************************/
/* GLOBAL Function Declarations */
/********************************/
//@{


/** 
 * Spawn another ball onto the playing field.
 * 
 * spawn on right hand edge of the screen with vx=-30 to -60 pixels/sec and
 * vy=+5 to -5 pixels/sec. The speed distribution in the y-direction must be
 * uniform. The x speed can be fixed between the values indicated.
 * 
 * @return Position of the sacrificed ball
 */
struct xy_vec simulation_spawnBall(void);

void simulation_step(void);

/** 
 * Update a single ball. Check the ball for collisions with lower-indexed balls
 * (so ball 5 can collide with balls [0-4]). Should be called repeatedly with
 * indexes in order from low to high. If a collision occurs BOTH balls will be
 * updated by this function. The other ball's ID is returned so it can be
 * re-drawn.
 * @param ball_id the ID of the ball to update
 * @return -1 if no collision occurs, ball id of ball colided with otherwise
 */
int simulation_substep(int ball_id);


/** 
 * Return the change in score attributed to this ball.
 *
 * @param substep_ball_id Ball id to check
 * @param paddle_y_positon Coordinates of the top of the paddle
 *
 * @return If substep_ball collides with the catcher then +1 will be returned,
 * if it goes off the left side then -1 will be returned, otherwise 0.
 */
int simulation_score_substep(int substep_ball_id, short paddle_y_position);

//@}

#endif // SIMULATION_H

