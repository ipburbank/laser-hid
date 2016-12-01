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
