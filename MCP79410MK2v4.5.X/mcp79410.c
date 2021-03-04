#define FCY 16000000UL

#include "xc.h"
#include <assert.h>
#include <stdbool.h>
#include <libpic30.h>
#include "mcc_generated_files/system.h"
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

#define MCP79410_ADDR               0x6F //0x57 
    
#define RTCC_SECONDS                0x00
#define RTCC_MINUTES                0x01
#define RTCC_HOUR                   0x02
#define RTCC_DAY                    0x03
#define RTCC_DATE                   0x04
#define RTCC_MONTH                  0x05
#define RTCC_YEAR                   0x06
#define CONTROL_REG                 0x07
#define CALIBRATION                 0x08

#define PWR_DOWN_MINUTES            0x18
#define PWR_DOWN_HOUR               0x19
#define PWR_DOWN_DATE               0x1A
#define PWR_DOWN_MONTH              0x1B
#define PWR_UP_MINUTES              0x1C
#define PWR_UP_HOUR                 0x1D
#define PWR_UP_DATE                 0x1E
#define PWR_UP_MONTH                0x1F


I2C1_MESSAGE_STATUS status;                                                     // 

uint8_t Slave_Address;                                                          // definiert "Slave_Address" 

uint8_t  writeBuffer[4] = {0};                                                  // definiert "writeBuffer"
uint8_t  readBuffer[3] = {0};                                                   // definiert "readBuffer" 
uint16_t retryTimeOut1;                                                         // definiert "retryTimeOut1" 
uint16_t retryTimeOut2;                                                         // definiert "retryTimeOut2" 


void MCP79410_set_date_time (                                                   // "MCP79410_set_date_time"
                             uint8_t day,                                       // definiert "day"
                             uint8_t mouth,                                     // definiert "mouth"
                             uint8_t year,                                      // definiert "year"        
                             uint8_t hour,                                      // definiert "hour"
                             uint8_t min){                                      // definiert "min" 
    Slave_Address = MCP79410_ADDR;                                              // schreibt "MCP79410_ADDR" in "Slave_Address"

    writeBuffer[0] = RTCC_DAY;                                                  // schreibt "RTCC_DAY" in "writeBuffer[0]"                               
    writeBuffer[1] = 0x08;      //0b00001000;                                   // schreibt "0x08" in "writeBuffer[1]"                      
    I2C1_MasterWrite(&writeBuffer[0], 2, Slave_Address, &status);               // schreibt 2 Byte (writeBuffer[0]) an I2C1                      
    i2c1_message_pending_100();                                                 // ruft "i2c1_message_pending_100" auf    
    
    writeBuffer[0] = RTCC_SECONDS;                                              // schreibt "RTCC_SECONDS" in "writeBuffer[0]"
    writeBuffer[1] = 0x80;                                                      // schreibt "0x80" in "writeBuffer[1]"
    I2C1_MasterWrite(&writeBuffer[0], 2, Slave_Address, &status);               // schreibt 2 Byte (writeBuffer[0]) an I2C1                  
    i2c1_message_pending_100();                                                 // ruft "i2c1_message_pending_100" auf 
    
    writeBuffer[0] = RTCC_HOUR;                                                 // schreibt "RTCC_HOUR" in "writeBuffer[0]"
    I2C1_MasterWrite(&writeBuffer[0], 1, Slave_Address, &status);               // schreibt 1 Byte (writeBuffer[0]) an I2C1                  
    i2c1_message_pending_100();                                                 // ruft "i2c1_message_pending_100" auf 
    I2C1_MasterRead(&readBuffer[1], 1, Slave_Address, &status);                 // liest von I2C1 in 1 Byte (readBuffer[0]) 
    i2c1_message_pending_100();                                                 // ruft "i2c1_message_pending_100" auf
    
    writeBuffer[1] = 0x01;                                                      // schreibt "0x01" in "writeBuffer[1]"
    I2C1_MasterWrite(&writeBuffer[0], 2, Slave_Address, &status);               // schreibt 2 Byte (writeBuffer[0]) an I2C1                  
    i2c1_message_pending_100();                                                 // ruft "i2c1_message_pending_100" auf 
    
    writeBuffer[0] = CONTROL_REG;                                               // schreibt "CONTROL_REG" in "writeBuffer[0]"
    writeBuffer[1] = 0x40;                                                      // schreibt "0x40" in "writeBuffer[1]"
    I2C1_MasterWrite(&writeBuffer[0], 2, Slave_Address, &status);               // schreibt 2 Byte (writeBuffer[0]) an I2C1                  
    i2c1_message_pending_100();                                                 // ruft "i2c1_message_pending_100" auf 
    
    
    day = day & 0b00111111;                                                     // "day" maskieren mit "0b00111111"
    writeBuffer[0] = RTCC_DATE;                                                 // schreibt "RTCC_DATE" in "writeBuffer[0]"
    writeBuffer[1] = day;                                                       // schreibt "day" in "writeBuffer[1]"
    I2C1_MasterWrite(&writeBuffer[0], 2, Slave_Address, &status);               // schreibt 2 Byte (writeBuffer[0]) an I2C1                  
    i2c1_message_pending_100();                                                 // ruft "i2c1_message_pending_100" auf 
    
    mouth = mouth & 0b00111111;                                                 // "mouth" maskieren mit "0b00111111"
    writeBuffer[0] = RTCC_MONTH;                                                // schreibt "RTCC_MONTH" in "writeBuffer[0]"
    writeBuffer[1] = mouth;                                                     // schreibt "mouth" in "writeBuffer[1]"
    I2C1_MasterWrite(&writeBuffer[0], 2, Slave_Address, &status);               // schreibt 2 Byte (writeBuffer[0]) an I2C1                  
    i2c1_message_pending_100();                                                 // ruft "i2c1_message_pending_100" auf 
    
    year = year & 0b11111111;                                                   // "year" maskieren mit "0b11111111"
    writeBuffer[0] = RTCC_YEAR;                                                 // schreibt "RTCC_YEAR" in "writeBuffer[0]"
    writeBuffer[1] = year;                                                      // schreibt "year" in "writeBuffer[1]"
    I2C1_MasterWrite(&writeBuffer[0], 2, Slave_Address, &status);               // schreibt 2 Byte (writeBuffer[0]) an I2C1                  
    i2c1_message_pending_100();                                                 // ruft "i2c1_message_pending_100" auf 
    
    hour = hour & 0b01111111;                                                   // "hour" maskieren mit "0b01111111"
    writeBuffer[0] = RTCC_HOUR;                                                 // schreibt "RTCC_HOUR" in "writeBuffer[0]"
    writeBuffer[1] = hour;                                                      // schreibt "hour" in "writeBuffer[1]"
    I2C1_MasterWrite(&writeBuffer[0], 2, Slave_Address, &status);               // schreibt 2 Byte (writeBuffer[0]) an I2C1                  
    i2c1_message_pending_100();                                                 // ruft "i2c1_message_pending_100" auf 
    
    min = min & 0b01111111;                                                     // "min" maskieren mit "0b01111111"
    writeBuffer[0] = RTCC_MINUTES;                                              // schreibt "RTCC_MONTH" in "writeBuffer[0]"
    writeBuffer[1] = min;                                                       // schreibt "min" in "writeBuffer[1]"
    I2C1_MasterWrite(&writeBuffer[0], 2, Slave_Address, &status);               // schreibt 2 Byte (writeBuffer[0]) an I2C1                  
    i2c1_message_pending_100();                                                 // ruft "i2c1_message_pending_100" auf 
}                                                                               // 

void MCP79410_get_date(                                                         // "MCP79410_get_date"
                        uint8_t *day,                                           // definiert "day"
                        uint8_t *mouth,                                         // definiert "mouth" 
                        uint8_t *year){                                         // definiert "year"
    Slave_Address = MCP79410_ADDR;                                              // schreibt "MCP79410_ADDR" in "Slave_Address"
    
    writeBuffer[0] = RTCC_DATE;                                                 // schreibt "RTCC_DATE" in "writeBuffer[0]"
    I2C1_MasterWrite(&writeBuffer[0], 1, Slave_Address, &status);               // schreibt 1 Byte (writeBuffer[0]) an I2C1                  
    i2c1_message_pending_100();                                                 // ruft "i2c1_message_pending_100" auf 
    I2C1_MasterRead(&readBuffer[0], 1, Slave_Address, &status);                 // liest von I2C1 in 1 Byte (readBuffer[0]) 
    i2c1_message_pending_100();                                                 // ruft "i2c1_message_pending_100" auf
    *day = readBuffer[0];                                                       // schreibt "readBuffer[0]" in "day"
    *day = *day & 0b00111111;                                                   // maskiert "day" mit "0b00111111"
    
    writeBuffer[0] = RTCC_MONTH;                                                // schreibt "RTCC_MONTH" in "writeBuffer[0]"
    I2C1_MasterWrite(&writeBuffer[0], 1, Slave_Address, &status);               // schreibt 1 Byte (writeBuffer[0]) an I2C1                  
    i2c1_message_pending_100();                                                 // ruft "i2c1_message_pending_100" auf 
    I2C1_MasterRead(&readBuffer[0], 1, Slave_Address, &status);                 // liest von I2C1 in 1 Byte (readBuffer[0]) 
    i2c1_message_pending_100();                                                 // ruft "i2c1_message_pending_100" auf
    *mouth = readBuffer[0];                                                     // schreibt "readBuffer[0]" in "mouth"
    *mouth = *mouth & 0b00011111;                                               // maskiert "mouth" mit "0b00011111"
    
    writeBuffer[0] = RTCC_YEAR;                                                 // schreibt "RTCC_YEAR" in "writeBuffer[0]"
    I2C1_MasterWrite(&writeBuffer[0], 1, Slave_Address, &status);               // schreibt 1 Byte (writeBuffer[0]) an I2C1                  
    i2c1_message_pending_100();                                                 // ruft "i2c1_message_pending_100" auf 
    I2C1_MasterRead(&readBuffer[0], 1, Slave_Address, &status);                 // liest von I2C1 in 1 Byte (readBuffer[0]) 
    i2c1_message_pending_100();                                                 // ruft "i2c1_message_pending_100" auf
    *year = readBuffer[0];                                                      // schreibt "readBuffer[0]" in "year"
}                                                                               // 

void MCP79410_get_time(                                                         // "MCP79410_get_time"
                        uint8_t *hour,                                          // definiert "hour"
                        uint8_t *min,                                           // definiert "min"
                        uint8_t *sec){                                          // definiert "sec"
    Slave_Address = MCP79410_ADDR;                                              // schreibt "MCP79410_ADDR" in "Slave_Address"

    writeBuffer[0] = RTCC_SECONDS;                                              // schreibt "RTCC_SECONDS" in "writeBuffer[0]"
    I2C1_MasterWrite(&writeBuffer[0], 1, Slave_Address, &status);               // schreibt 1 Byte (writeBuffer[0]) an I2C1                  
    i2c1_message_pending_100();                                                 // ruft "i2c1_message_pending_100" auf 
    I2C1_MasterRead(&readBuffer[0], 1, Slave_Address, &status);                 // liest von I2C1 in 1 Byte (readBuffer[0]) 
    i2c1_message_pending_100();                                                 // ruft "i2c1_message_pending_100" auf
    *sec = readBuffer[0];                                                       // schreibt "readBuffer[0]" in "sec"
    *sec = *sec & 0b01111111;                                                   // maskiert "sec" mit "0b01111111"
    
    writeBuffer[0] = RTCC_MINUTES;                                              // schreibt "RTCC_MINUTES" in "writeBuffer[0]"
    I2C1_MasterWrite(&writeBuffer[0], 1, Slave_Address, &status);               // schreibt 1 Byte (writeBuffer[0]) an I2C1                  
    i2c1_message_pending_100();                                                 // ruft "i2c1_message_pending_100" auf 
    I2C1_MasterRead(&readBuffer[0], 1, Slave_Address, &status);                 // liest von I2C1 in 1 Byte (readBuffer[0]) 
    i2c1_message_pending_100();                                                 // ruft "i2c1_message_pending_100" auf
    *min = readBuffer[0];                                                       // schreibt "readBuffer[0]" in "min"
    *min = *min & 0b01111111;                                                   // maskiert "min" mit "0b01111111"
    
    writeBuffer[0] = RTCC_HOUR;                                                 // schreibt "RTCC_HOUR" in "writeBuffer[0]"
    I2C1_MasterWrite(&writeBuffer[0], 1, Slave_Address, &status);               // schreibt 1 Byte (writeBuffer[0]) an I2C1                  
    i2c1_message_pending_100();                                                 // ruft "i2c1_message_pending_100" auf 
    I2C1_MasterRead(&readBuffer[0], 1, Slave_Address, &status);                 // liest von I2C1 in 1 Byte (readBuffer[0]) 
    i2c1_message_pending_100();                                                 // ruft "i2c1_message_pending_100" auf
    *hour = readBuffer[0];                                                      // schreibt "readBuffer[0]" in "hour"
    *hour = *hour & 0b00111111;                                                 // maskiert "hour" mit "0b00111111"
}                                                                               // 

void i2c1_message_pending_100(void){                                            // "i2c1_message_pending_100"
    retryTimeOut1 = 0;                                                          // setzt "retryTimeOut1" auf 0
    retryTimeOut2 = 0;                                                          // setzt "retryTimeOut2" auf 0
    while(status == I2C1_MESSAGE_PENDING){                                      // solange "status" gleich "I2C1_MESSAGE_PENDING" ist
        if (retryTimeOut2 == 500){                                              // wenn "retryTimeOut2" gleich 100 ist
            break;                                                              // Abbruch der Schleife
        }else{                                                                  // sonst
            retryTimeOut2++;                                                    // "retryTimeOut2" +1
        }                                                                       // 
    }                                                                           //
    while(status != I2C1_MESSAGE_FAIL){                                         // solange "status" nicht gleich "I2C1_MESSAGE_FAIL" ist
        if (retryTimeOut1 == 500){                                              // wenn "retryTimeOut1" gleich 100 ist
            break;                                                              // Abbruch der Schleife
        }else{                                                                  // sonst
            retryTimeOut1++;                                                    // "retryTimeOut1" +1
        }                                                                       // 
    }                                                                           // 
}                                                                               // 