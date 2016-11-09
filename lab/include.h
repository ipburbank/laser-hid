#ifndef _INCLUDE_H
#define _INCLUDE_H

#define _SUPPRESS_PLIB_WARNING 1
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING

// for microcontroller
#include <plib.h>
// serial stuff
//#include <stdio.h>
// for standard library
#include <stdlib.h>
#include <math.h>
// for sane types (e.g. uint8_t and bool)
#include <stdint.h>
#include <stdbool.h>


//////////////////////
/* ARRAY LENGTH     */
//////////////////////

// only works on arrays (not on pointers). Don't get confused.
#define ARRAY_LEN(a) (sizeof(a)/sizeof(a[0]))

//////////////////////
/* PULL UP/DOWN     */
//////////////////////

// PORT B
#define EnablePullDownB(bits) CNPUBCLR=bits; CNPDBSET=bits;
#define DisablePullDownB(bits) CNPDBCLR=bits;
#define EnablePullUpB(bits) CNPDBCLR=bits; CNPUBSET=bits;
#define DisablePullUpB(bits) CNPUBCLR=bits;
//PORT A
#define EnablePullDownA(bits) CNPUACLR=bits; CNPDASET=bits;
#define DisablePullDownA(bits) CNPDACLR=bits;
#define EnablePullUpA(bits) CNPDACLR=bits; CNPUASET=bits;
#define DisablePullUpA(bits) CNPUACLR=bits;

//////////////////////
/* FIXED POINT      */
//////////////////////

typedef signed int fix16 ;
#define multfix16(a,b) ((fix16)(((( signed long long)(a))*(( signed long long)(b)))>>16)) //multiply two fixed 16:16
#define float2fix16(a) ((fix16)((a)*65536.0)) // 2^16
#define fix2float16(a) ((float)(a)/65536.0)
#define fix2int16(a)    ((int)((a)>>16))
#define int2fix16(a)    ((fix16)((a)<<16))
#define divfix16(a,b) ((fix16)((((signed long long)(a)<<16)/(b))))
#define sqrtfix16(a) (float2fix16(sqrt(fix2float16(a))))
#define absfix16(a) abs(a)

#endif
