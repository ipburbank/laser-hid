#ifndef _RENDERING_H
#define _RENDERING_H

/*******************************/
/* Includes                    */
/*******************************/
//@{

#include "include.h"
#include "color.h"
#include "projector.h"
#include "parameters.h"

//@}

/*******************************/
/* GLOBAL Macro Definitions    */
/*******************************/
//@{

//@}

/*********************************/
/* GLOBAL Type(def) Declarations */
/*********************************/
//@{

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

void rendering_drawLine(short x0, short y0, short x1, short y1,
                        struct color color);

void rendering_drawRect(short x, short y, short w, short h,
                        struct color color);

void rendering_drawCircle(short x0, short y0, short r,
                          struct color color);

void rendering_drawCircleHelper(short x0, short y0, short r, unsigned char cornername,
                                struct color color);

void rendering_fillCircle(short x0, short y0, short r,
                          struct color color);

void rendering_fillCircleHelper(short x0, short y0, short r, unsigned char cornername,
                                short delta, struct color color);

void rendering_drawTriangle(short x0, short y0, short x1, short y1,
                            short x2, short y2, struct color color);

void rendering_fillTriangle(short x0, short y0, short x1, short y1,
                            short x2, short y2, struct color color);

void rendering_drawRoundRect(short x0, short y0, short w, short h,
                             short radius, struct color color);

void rendering_fillRoundRect(short x0, short y0, short w, short h, short radius,
                             struct color color);

void rendering_drawBitmap(short x, short y, const unsigned char *bitmap, short w,
                          short h, struct color color);

void rendering_drawChar(short x, short y, unsigned char c,
                        struct color color,
                        struct color bg, unsigned char size);

void rendering_setCursor(short x, short y);

void rendering_setTextColor(struct color c);

void rendering_setTextColor2(struct color c,
                             struct color bg);

void rendering_setTextSize(unsigned char s);

void rendering_setTextWrap(char w);

void rendering_write(unsigned char c);

void rendering_writeString(char* str);    // This is the function to use to write a string

void rendering_drawPixel(short x, short y, struct color color);

void rendering_drawFastVLine(short x, short y, short h,
                             struct color color);

void rendering_drawFastHLine(short x, short y, short w,
                             struct color color);

void rendering_fillRect(short x, short y, short w, short h,
                        struct color color);

/**
 * @brief Draw a 3x3 pixel cross
 *
 * @param x X coordinate of center of cross
 * @param y Y coordinate of center of cross
 * @param color Color of cross
 *
 * The cross is the center pixel plus one pixel above and below the center and
 * one pixel to the left and right of the center.
 */
void rendering_drawMinimalCross(short const x, short const y,
                                struct color const color);

/**
 * @brief Draw's a Morse Code dot, starting at the designated position
 *
 * @param x X coordinate of the start of the Dot
 * @param y Y coordinate of the start of the Dot
 * @param color Color of the Dot
 *
 * The Dot is a Morse Code dot, it has a fixed length based on the DOT_LEN
 * parameter.
 */
void rendering_drawDot(short const x, short const y, struct color const color);

/**
 * @brief Draw's a Morse Code dash, starting at the designated position
 *
 * @param x X coordinate of the start of the Dash
 * @param y Y coordinate of the start of the Dash
 * @param color Color of the Dot
 *
 * The Dash is a Morse Code dash, it has a fixed length based on the DASH_LEN
 * parameter.
 */
void rendering_drawDash(short const x, short const y,
                        struct color const color);

/**
 * @brief Draw the character in Morse Code
 *
 * @param letter The character that we want to draw, must be a-z
 * @param color Color of the outputted Morse code
 *
 * The letter is encoded into Morse code and is drawn to the display
 */
void rendering_drawMorseChar(short x, short y, char letter, struct color color);

/**
 * @brief Draw a string in Morse Code
 *
 * @param x The starting X coordinate to start drawing at
 * @param y The starting Y coordinate to start drawing at
 * @param str The string that we want to draw, characters must all be a-z
 * @param color Color of the outputted Morse code
 *
 * The string is encoded into Morse code and is drawn to the display
 */
void rendering_drawMorseString(short x, short y, char* str, struct color color);

//@}

#endif
