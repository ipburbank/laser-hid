#ifndef _RENDERING_H
#define _RENDERING_H

/*******************************/
/* Includes                    */
/*******************************/
//@{

#include "include.h"
#include "color.h"
#include "projector.h"

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

//@}

#endif
