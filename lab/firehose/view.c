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

#define COLOR_BG (ILI9340_BLACK)
#define BALL_COLOR_FG (ILI9340_MAGENTA)

#define BARRIER_COLOR_FG (ILI9340_WHITE)

#define PADDLE_COLOR_FG (ILI9340_YELLOW)

#define STATS_COLOR_FG (ILI9340_CYAN)
#define STATS_NEG_COLOR_FG (ILI9340_RED)
#define STATS_SLOW_DIST_FROM_TOP (30)
#define STATS_FAST_DIST_FROM_TOP (80)

//@}

/********************************/
/* LOCAL Type(def) Declarations */
/********************************/
//@{

// position as understood by the tft, in short
struct tft_pixel_pos {
  short x;
  short y;
};

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

static void fill_ball_pixels(struct tft_pixel_pos const * const pos,
                             unsigned short const color);

// convert to whole numbers and shorts, because that is what the tft library
// likes
static inline void populate_tft_pixel_pos(struct xy_vec const * fix_pos,
                                          struct tft_pixel_pos * pos);

// draw a stats tuple (part1, part2) stacked on top of eachother, starting at
// distance_from_top
void stats_pair_redraw(char * buffer, size_t buff_len, unsigned short color,
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
  tft_init_hw();
  tft_begin();
  tft_fillScreen(COLOR_BG);
  tft_setRotation(1); // 320 x 240
  tft_setTextSize(1);
}

void view_draw_ball(struct xy_vec const new_pos) {
  struct tft_pixel_pos new;
  populate_tft_pixel_pos(&new_pos, &new);

  // fill with ball-fg the  position
  fill_ball_pixels(&new, BALL_COLOR_FG);
}

void view_redraw_paddle(short const old_top, short const new_top) {
  // invariant: paddles are always drawn or erased top-to-bottom
  short const old_bottom = old_top + PADDLE_HEIGHT;
  short const new_bottom = new_top + PADDLE_HEIGHT;

  // clear old paddle
  if (DISABLE_PADDLE_OPTIMIZATION ||
      old_bottom < new_top || new_bottom < old_top) {
    // non-overlapping...
    tft_drawLine(PADDLE_DISTANCE_FROM_LEFT, old_top,
                 PADDLE_DISTANCE_FROM_LEFT, old_bottom,
                 COLOR_BG);
  } else {
    // overlapping...
    if (old_top < new_top) {
      // bottom part of old paddle overlaps top part of new...
      tft_drawLine(PADDLE_DISTANCE_FROM_LEFT, old_top,
                   PADDLE_DISTANCE_FROM_LEFT, new_top,
                   COLOR_BG);
    } else { // (old_top >= new_top)
      // top part of old paddle overlaps bottom part of new...
      tft_drawLine(PADDLE_DISTANCE_FROM_LEFT, new_bottom,
                   PADDLE_DISTANCE_FROM_LEFT, old_bottom,
                   COLOR_BG);
    }
  }

  // draw new paddle
  tft_drawLine(PADDLE_DISTANCE_FROM_LEFT, new_top,
               PADDLE_DISTANCE_FROM_LEFT, new_bottom,
               PADDLE_COLOR_FG);
}

// should probably only need to be called at setup, but maybe a bit of the
// barrier will get overwritten when a ball collides, I am not sure. This is a
// function just in case we need to call it in that situation.
void view_draw_barriers(void) {
  // top barrier
  tft_drawLine(BARRIER_DIST_FROM_LEFT, 0, // top edge
               BARRIER_DIST_FROM_LEFT, BARRIER_LENGTH,
               BARRIER_COLOR_FG);
  // bottom barrier
  tft_drawLine(BARRIER_DIST_FROM_LEFT, TFT_HEIGHT, // bottom edge
               BARRIER_DIST_FROM_LEFT, TFT_HEIGHT - BARRIER_LENGTH,
               BARRIER_COLOR_FG);
}

void view_remove_ball(struct xy_vec const pos) {
  struct tft_pixel_pos pixel_pos;
  populate_tft_pixel_pos(&pos, &pixel_pos);
  fill_ball_pixels(&pixel_pos, COLOR_BG);
}

void view_draw_slow_stats(int const fps, int const num_secs) {
  stats_pair_redraw(last_slow_stats, ARRAY_LEN(last_slow_stats), STATS_COLOR_FG,
                    fps, num_secs, STATS_SLOW_DIST_FROM_TOP);
}

void view_draw_fast_stats(int score, int const num_balls) {
  // draw in different color if score negative
  unsigned short color;
  if (score < 0) {
    color = STATS_NEG_COLOR_FG;
    score = abs(score);
  } else {
    color = STATS_COLOR_FG;
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

static void fill_ball_pixels(struct tft_pixel_pos const * const pos,
                             unsigned short const color) {
  tft_drawPixel(pos->x - 1, pos->y,     color); // b
  tft_drawPixel(pos->x,     pos->y,     color); // c    X a X
  tft_drawPixel(pos->x + 1, pos->y,     color); // d    b c d
  tft_drawPixel(pos->x,     pos->y + 1, color); // a    X e X
  tft_drawPixel(pos->x,     pos->y - 1, color); // e
}

static inline void populate_tft_pixel_pos(struct xy_vec const * fix_pos,
                                          struct tft_pixel_pos * pos) {
  pos->x = (short) fix2int16(fix_pos->x);
  pos->y = (short) fix2int16(fix_pos->y);
}

void stats_pair_redraw(char * buffer, size_t buff_len,
                       unsigned short const color, int const part1,
                       int const part2, short const distance_from_top) {
  // erase old one
  tft_setCursor(0, distance_from_top);
  tft_setTextColor(COLOR_BG);
  tft_writeString(buffer);
  // put new stats into the string
  snprintf(buffer, buff_len, "%d\n%d", part1, part2);
  // write onto the screen
  tft_setCursor(0, distance_from_top);
  tft_setTextColor(color);
  tft_writeString(buffer);
}

//@}
