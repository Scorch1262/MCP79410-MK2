#define FCY 16000000UL
#include <assert.h>
#include <stdbool.h>
#include "mcc_generated_files/system.h"
#include <stdio.h>
#include <stdlib.h>
#include <libpic30.h>
#include <xc.h>
#include <string.h>
#include "mcc_generated_files/i2c1.h"
#include "mcp79410.h"

#if defined(__dsPIC33E__)
	#include <p33exxxx.h>
#elif defined(__dsPIC33F__)
	#include <p33fxxxx.h>
#elif defined(__dsPIC30F__)
	#include <p30fxxxx.h>
#elif defined(__PIC24E__)
	#include <p24exxxx.h>
#elif defined(__PIC24H__)
	#include <p24hxxxx.h>
#elif defined(__PIC24F__) || defined(__PIC24FK__)
	#include <p24fxxxx.h>
#endif

uint8_t day;                                    // definiert "day"
uint8_t month;                                  // definiert "month" 
uint8_t year;                                   // definiert "year"       
uint8_t hour;                                   // definiert "hour"
uint8_t min;                                    // definiert "min"
uint8_t sec;                                    // definiert "sec"

/*
                         Main application
 */
int main(void){                                         // "main"
    SYSTEM_Initialize();                                // ruft "SYSTEM_Initialize" auf
    
    day = 0x03;                                         // schreibt "0x03" in "day"
    month = 0x03;                                       // schreibt "0x03" in "month"
    year = 0x21;                                        // schreibt "0x21" in "year"
    hour = 0x19;                                        // schreibt "0x19" in "hour"
    min = 0x25;                                         // schreibt "0x25" in "min"    
    //MCP79410_set_date_time(day,month,year,hour,min);    // schreibt "day,month,year,hour,min" an EEPROM
    
    while(1){                                           // Endloschleife
        MCP79410_get_date(&day,&month,&year);           // 
        MCP79410_get_time(&hour,&min,&sec);             // 
        printf("RTC: 20%02x-%02x-%02x %02x:%02x:%02x\r\n",year,month,day,hour,min,sec);
	    __delay_ms(1000);                               // warte 1000ms
    }                                                   // 
}                                                       // 

