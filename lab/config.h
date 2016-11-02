/**
 * @file   config.h
 * @author Syed Tahmid Mahbub
 * @author Bruce Land
 * @date   2015-9-24
 * Created on October 10, 2014
 */

#ifndef CONFIG_H
#define	CONFIG_H

#include "include.h"
//=============================================================
// 40 MHz
#pragma config FNOSC = FRCPLL, POSCMOD = OFF
#pragma config FPLLIDIV = DIV_2, FPLLMUL = MUL_20 //40 MHz
#pragma config FPBDIV = DIV_1, FPLLODIV = DIV_2 // PB 40 MHz
#pragma config FWDTEN = OFF,  FSOSCEN = OFF, JTAGEN = OFF

//==============================================================
// Protothreads configure

// IF use_vref_debug IS defined, pin 25 is Vref output
//#define use_vref_debug

// IF use_uart_serial IS defined, pin 21 and pin 22 are used by the uart
//#define use_uart_serial
#ifdef use_uart_serial
#define BAUDRATE 9600 // must match PC terminal emulator setting
#endif

/////////////////////////////////
// set up clock parameters
// system cpu clock
#define sys_clock 40000000

// sys_clock/FPBDIV
#define pb_clock sys_clock // divide by one in this case

#endif	/* CONFIG_H */

