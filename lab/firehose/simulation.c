/**
 * @file   simulation.c
 * @author Istvan Burbank, Peter Friend, James Cassell
 * @date   2016-10-04
 * @brief  Implementation drink from the firehose simulation
 * 
 */

#include "simulation.h"

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

static inline void updateCollision(struct ball *ball_i, struct ball *ball_j);

int simulation_collision_substep(int substep_ball_id);

//@}

/*******************************/
/* GLOBAL Variable Definitions */
/*******************************/
//@{

unsigned int simulation_num_spawned_balls = 0;

//@}

/*******************************/
/* GLOBAL Function Definitions */
/*******************************/
//@{

struct xy_vec simulation_spawnBall(void) {
  // find a ball which should can use
  unsigned int i;
  unsigned int spawn_index = 0;
  for (i = 0; i < NUM_BALLS; i++) {
    if (!balls[i].is_on_field) { // we can use this ball
      spawn_index = i;
      // increment the number of balls we have spawned
      simulation_num_spawned_balls += 1;
      break;
    } else if (balls[i].age > balls[spawn_index].age) {
      // choose the oldest ball
      spawn_index = i;
    }
  }

  struct xy_vec sacrificed_ball_position = balls[spawn_index].position;

  // spawn on right hand edge of the screen with vx=-30 to -60 pixels/sec and
  // vy=+5 to -5 pixels/sec. The speed distribution in the y-direction must be
  // uniform. The x speed can be fixed between the values indicated.
  struct xy_vec const init_position = {
    .x = int2fix16(IMAGE_WIDTH - BALL_COLLISION_RADIUS - 2),
    .y = int2fix16(IMAGE_HEIGHT / 2)
  };
  struct xy_vec const init_velocity = {
    .x = int2fix16(-2),
    // I arbitrarily chose these, they worked well enough, so I didn't change
    // them again. Who cares what it means, just know its arbitrary.
    .y = int2fix16((rand() % 88) - 44) >> 3
  };

  // initialize ball we will now spawn
  balls[spawn_index].is_on_field           = true;
  balls[spawn_index].age                   = 0;
  balls[spawn_index].ticks_since_collision = 0;
  balls[spawn_index].position              = init_position;
  balls[spawn_index].velocity              = init_velocity;

  return sacrificed_ball_position;
}

void simulation_step(void) {
  int ball_id;
  for(ball_id = 0; ball_id < NUM_BALLS; ball_id++) {
    simulation_substep(ball_id);
  }
}

int simulation_substep(int substep_ball_id) {
  struct ball *substep_ball = &balls[substep_ball_id];

  substep_ball->age                   += 1;
  substep_ball->ticks_since_collision += 1;

  int collision_return = simulation_collision_substep(substep_ball_id);

  // update the ball position
  substep_ball->position.x += substep_ball->velocity.x;
  substep_ball->position.y += substep_ball->velocity.y;
  substep_ball->velocity.x  = multfix16(substep_ball->velocity.x,
                                        DRAG_COEFFICIENT);
  substep_ball->velocity.y  = multfix16(substep_ball->velocity.y,
                                        DRAG_COEFFICIENT);

  return collision_return;
}


int simulation_score_substep(int substep_ball_id, short paddle_y_position) {
  struct ball *substep_ball = &balls[substep_ball_id];

  // if the ball isn't on the field we have no work to do
  if(!substep_ball->is_on_field) {
    return 0;
  }

  // check for collision with catcher
  if (substep_ball->position.x <
          int2fix16(PADDLE_DISTANCE_FROM_LEFT + BALL_COLLISION_RADIUS) &&
      substep_ball->position.x >
          int2fix16(PADDLE_DISTANCE_FROM_LEFT - BALL_COLLISION_RADIUS) &&
      substep_ball->position.y > int2fix16(paddle_y_position) &&
      substep_ball->position.y < int2fix16(paddle_y_position + PADDLE_HEIGHT)) {
    substep_ball->is_on_field = false;
    simulation_num_spawned_balls -= 1;

    return +1;
  }

  // check if the ball went off the left side
  if(substep_ball->position.x < int2fix16(0)) {
    substep_ball->is_on_field = false;
    simulation_num_spawned_balls -= 1;

    return -1;
  }

  // no score update, return 0
  return 0;
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

static inline void updateCollision(struct ball *ball_i, struct ball *ball_j) {
  // calculate the change in velocity in both x and y
  struct xy_vec pi = ball_i->position;
  struct xy_vec vi = ball_i->velocity;

  struct xy_vec pj = ball_j->position;
  struct xy_vec vj = ball_j->velocity;


  fix16 rx = pi.x - pj.x;
  fix16 ry = pi.y - pj.y;
  fix16 rabs = int2fix16(4); // sqrtfix16(multfix16(rx, rx) + multfix16(ry, ry));

  fix16 vx = vi.x - vj.x;
  fix16 vy = vi.y - vj.y;

  fix16 term1x = divfix16(0 - rx, rabs);
  fix16 term1y = divfix16(0 - ry, rabs);

  // compute the dot product of r and v
  fix16 term2 = divfix16((multfix16(rx, vx) + multfix16(ry, vy)), rabs);

  // dx and dy are term1x * term2 and term1y * term2 respectively
  fix16 delta_vx = multfix16(term1x, term2);
  fix16 delta_vy = multfix16(term1y, term2);

  ball_i->velocity.x += delta_vx;
  ball_i->velocity.y += delta_vy;

  ball_j->velocity.x -= delta_vx;
  ball_j->velocity.y -= delta_vy;
}

int simulation_collision_substep(int substep_ball_id) {
  struct ball *substep_ball = &balls[substep_ball_id];

  // check for collisions with the vertical walls
  if (substep_ball->ticks_since_collision > MIN_TICKS_BETWEEN_COLLISIONS &&
      (// right wall
       substep_ball->position.x > int2fix16(IMAGE_WIDTH - BALL_COLLISION_RADIUS)
       )
      )
    {
      substep_ball->velocity.x = 0 - substep_ball->velocity.x; // faster than * -1
      substep_ball->ticks_since_collision = 0;

    // it will not collide with another ball since it just experiences a
    // collision
    return -1;
  }

  // check for collisions with the barrier
  if (substep_ball->ticks_since_collision > MIN_TICKS_BETWEEN_COLLISIONS &&
      (// At the right height for the barriers
       ((substep_ball->position.y < int2fix16(BARRIER_HEIGHT) ||
         substep_ball->position.y > int2fix16(IMAGE_HEIGHT - BARRIER_HEIGHT)) &&
        // and at the right x for the barriers
        // +- 4 intuitively makes the barrier wider to prevent balls slipping
        // past
        (substep_ball->position.x > int2fix16(BARRIER_X - BALL_COLLISION_RADIUS - 4) &&
         substep_ball->position.x < int2fix16(BARRIER_X + BALL_COLLISION_RADIUS + 4)))
       )
      )
    {
      substep_ball->position.x = int2fix16(BARRIER_X + 5);
      substep_ball->velocity.x = 0 - substep_ball->velocity.x; // faster than * -1
      substep_ball->ticks_since_collision = 0;

    // it will not collide with another ball since it just experiences a
    // collision
    return -1;
  }

  // check for collision with walls
  // check for collision with the top and bottom
  if(substep_ball->ticks_since_collision > MIN_TICKS_BETWEEN_COLLISIONS &&
     (
      substep_ball->position.y > int2fix16(IMAGE_HEIGHT - BALL_COLLISION_RADIUS)
      || substep_ball->position.y < int2fix16(0 + BALL_COLLISION_RADIUS))
     )
    {
      substep_ball->velocity.y = 0 - substep_ball->velocity.y; // faster than * -1
      substep_ball->ticks_since_collision = 0;

      // it will not collide with a nother ball since it just experiences a
      // collision
      return -1;
  }

  // check all balls of lower ID for a collision
  int collision_ball_id;
  struct ball *collision_ball;
  for (collision_ball_id = 0; collision_ball_id < substep_ball_id;
       collision_ball_id++) {
    collision_ball = &balls[collision_ball_id];

    // if the collision ball is not on the field then we can't collide with it
    if(!collision_ball->is_on_field ||
       substep_ball->ticks_since_collision < MIN_TICKS_BETWEEN_COLLISIONS) {
      continue;
    }

    fix16 dposition_x = substep_ball->position.x - collision_ball->position.x;
    fix16 dposition_y = substep_ball->position.y - collision_ball->position.y;

    if (collision_ball->ticks_since_collision > MIN_TICKS_BETWEEN_COLLISIONS &&
        absfix16(dposition_x) < int2fix16(2 * BALL_COLLISION_RADIUS) &&
        absfix16(dposition_y) < int2fix16(2 * BALL_COLLISION_RADIUS) &&
        (multfix16(dposition_x, dposition_x) +
         multfix16(dposition_y, dposition_y)) <
            int2fix16(2 * BALL_COLLISION_RADIUS * 2 * BALL_COLLISION_RADIUS)) {

      updateCollision(substep_ball, collision_ball);

      // mark that the balls were in a collison
      substep_ball->ticks_since_collision   = 0;
      collision_ball->ticks_since_collision = 0;

      // return the other ball so it can be redrawn
      return collision_ball_id;
    }
  }

  // no collisions, return -1
  return -1;
}

//@}
