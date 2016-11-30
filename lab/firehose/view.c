/**
 * @file   view.c
 * @author Istvan Burbank, Peter Friend, James Cassell
 * @date   2016-10-07
 * @brief  Implementation of ECE4760 Lab3
 * 
 *  Pinout:
 * 
 * @see https://drive.google.com/file/d/0B4SoPFPRNziHcmlxMmg2Si0zWjQ/view
 */

#include "view.h"

/*******************************/
/* LOCAL Macro Definitions     */
/*******************************/
//@{

#define STATS_SLOW_DIST_FROM_TOP (30)
#define STATS_FAST_DIST_FROM_TOP (80)

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

char last_slow_stats[8];
char last_fast_stats[8];

//@}

/*******************************/
/* LOCAL Function Declarations */
/*******************************/
//@{

// draw a stats tuple (part1, part2) stacked on top of eachother, starting at
// distance_from_top
void stats_pair_redraw(char * buffer, size_t buff_len, struct color const color,
                       int const part1, int const part2,
                       short const distance_from_top);

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

void view_init(void) {
  // fill entire screen to erase whatever was there before
  rendering_fillRect(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, color_blank);

  rendering_setTextSize(1);
}

void view_draw_ball(struct xy_vec const new_pos) {
  // fill with ball-fg the  position
  rendering_drawMinimalCross((uint16_t) fix2int16(new_pos.x),
                             (uint16_t) fix2int16(new_pos.y),
                             ball_color_fg);
}

void view_redraw_paddle(short const old_top, short const new_top) {
  // invariant: paddles are always drawn or erased top-to-bottom
  short const old_bottom = old_top + PADDLE_HEIGHT;
  short const new_bottom = new_top + PADDLE_HEIGHT;

  // clear old paddle
  if (DISABLE_PADDLE_OPTIMIZATION ||
      old_bottom < new_top || new_bottom < old_top) {
    // non-overlapping...
    rendering_drawLine(PADDLE_DISTANCE_FROM_LEFT, old_top,
                       PADDLE_DISTANCE_FROM_LEFT, old_bottom,
                       color_blank);
  } else {
    // overlapping...
    if (old_top < new_top) {
      // bottom part of old paddle overlaps top part of new...
      rendering_drawLine(PADDLE_DISTANCE_FROM_LEFT, old_top,
                         PADDLE_DISTANCE_FROM_LEFT, new_top,
                         color_blank);
    } else { // (old_top >= new_top)
      // top part of old paddle overlaps bottom part of new...
      rendering_drawLine(PADDLE_DISTANCE_FROM_LEFT, new_bottom,
                         PADDLE_DISTANCE_FROM_LEFT, old_bottom,
                         color_blank);
    }
  }

  // draw new paddle
  rendering_drawLine(PADDLE_DISTANCE_FROM_LEFT, new_top,
                     PADDLE_DISTANCE_FROM_LEFT, new_bottom,
                     paddle_color_fg);
}

// should probably only need to be called at setup, but maybe a bit of the
// barrier will get overwritten when a ball collides, I am not sure. This is a
// function just in case we need to call it in that situation.
void view_draw_barriers(void) {
  // top barrier
  rendering_drawLine(BARRIER_DIST_FROM_LEFT, 0, // top edge
                     BARRIER_DIST_FROM_LEFT, BARRIER_LENGTH,
                     barrier_color_fg);
  // bottom barrier
  rendering_drawLine(BARRIER_DIST_FROM_LEFT, IMAGE_HEIGHT, // bottom edge
                     BARRIER_DIST_FROM_LEFT, IMAGE_HEIGHT - BARRIER_LENGTH,
                     barrier_color_fg);
}

void view_remove_ball(struct xy_vec const pos) {
  // fill with ball-fg the  position
  rendering_drawMinimalCross((uint16_t) fix2int16(pos.x),
                             (uint16_t) fix2int16(pos.y),
                             color_blank);
}

void view_draw_slow_stats(int const fps, int const num_secs) {
  stats_pair_redraw(last_slow_stats, ARRAY_LEN(last_slow_stats), stats_color_fg,
                    fps, num_secs, STATS_SLOW_DIST_FROM_TOP);
}

void view_draw_fast_stats(int score, int const num_balls) {
  // draw in different color if score negative
  struct color color;
  if (score < 0) {
    color = stats_neg_color_fg;
    score = abs(score);
  } else {
    color = stats_color_fg;
  }
  stats_pair_redraw(last_fast_stats, ARRAY_LEN(last_fast_stats), color,
                    score, num_balls, STATS_FAST_DIST_FROM_TOP);
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


void stats_pair_redraw(char * buffer, size_t buff_len,
                       struct color const color, int const part1,
                       int const part2, short const distance_from_top) {
  // erase old one
  rendering_setCursor(0, distance_from_top);
  rendering_setTextColor(color_blank);
  rendering_writeString(buffer);
  // put new stats into the string
  snprintf(buffer, buff_len, "%d\n%d", part1, part2);
  // write onto the screen
  rendering_setCursor(0, distance_from_top);
  rendering_setTextColor(color);
  rendering_writeString(buffer);
}

//@}
