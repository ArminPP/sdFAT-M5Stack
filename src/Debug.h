#ifndef _DEBUG_H_
#define _DEBUG_H_
// Debug Library (enhanced by Armin :-)  blocking because of serial functions instead if streams, but simple ;-)
// https://forum.arduino.cc/t/how-to-debug-code-in-arduino-ide/209670/32
// 
//examples:
// 
// #define DPRINT(args...)  Serial.print(args)  OR use the following syntax:
// 
//  This  converts to >>>>----------------------->  This OR a Blank Line.  
// DPRINTLN("Testing123");                          Serial.println("Testing123");  
// DPRINTLN(0xC0FFEEul,DEC);                        Serial.println(0xC0FFEEul,DEC); 
// DPRINTLN(12648430ul,HEX);                        Serial.println(12648430ul,HEX); 
// DPRINTLNF("This message came from your flash");  Serial.println(F("This message came from your flash"));
// DPRINT(myVariable);                              Serial.print(myVariable);
// DELAY(100);                                      delay(100);
// PINMODE(9600);                                   pinMode(9600);
// TOGGLEd13;                                       PINB = 0x20;  // D13 Toggle,for UNO ONLY
//
// Also, this works  #define INFO(...)  { Console->printf("INFO: "); Console->printf(__VA_ARGS__); }   >>>--->   where {} allows multiple lines of code.
// See: http://forum.arduino.cc/index.php?topic=511393.msg3485833#new
// 


#ifdef DEBUG

#define DWRITE(...)          Serial.write(__VA_ARGS__)
#define DWRITELN(...)        Serial.writeln(__VA_ARGS__)

#define DPRINT(...)          Serial.print(__VA_ARGS__)
#define DPRINTLN(...)        Serial.println(__VA_ARGS__)
#define DRINTF(...)          Serial.print(F(__VA_ARGS__))
#define DPRINTLNF(...)       Serial.println(F(__VA_ARGS__)) //printing text using the F macro

#define DEBUG_PRINT(...)     Serial.print(F(#__VA_ARGS__" = ")); Serial.print(__VA_ARGS__); Serial.print(F(" ")) 
#define DEBUG_PRINTLN(...)   DEBUG_PRINT(__VA_ARGS__); Serial.println()

#define HEADER(...)          Serial.print(__FUNCTION__);Serial.print(F("():[")); Serial.print(__LINE__); Serial.print(F("]   "));

#define DPRINT_H(...)        HEADER(__VA_ARGS__); Serial.print(__VA_ARGS__)
#define DPRINTLN_H(...)      HEADER(__VA_ARGS__); Serial.println(__VA_ARGS__)
#define DRINTF_H(...)        HEADER(__VA_ARGS__); Serial.print(F(__VA_ARGS__))
#define DPRINTLNF_H(...)     HEADER(__VA_ARGS__); Serial.println(F(__VA_ARGS__)) //printing text using the F macro


#define DEBUG_PRINT_H(...)   HEADER(__VA_ARGS__); Serial.print(F(#__VA_ARGS__" = ")); Serial.print(__VA_ARGS__); Serial.print(F(" "))
#define DEBUG_PRINTLN_H(...) DEBUG_PRINT_H(__VA_ARGS__); Serial.println()

#else
#define DWRITE(...)
#define DWRITELN(...) 

#define DPRINT(...)       
#define DPRINTLN(...)     
#define DPRINTF(...)      
#define DPRINTLNF(...)    

#define DEBUG_PRINT(...)    
#define DEBUG_PRINTLN(...)  

#define HEADER(...) 

#define DPRINT_H(...)
#define DPRINTLN_H(...)
#define DRINTF_H(...)
#define DPRINTLNF_H(...)

#define DEBUG_PRINT_H(...)
#define DEBUG_PRINTLN_H(...)

#endif


#endif /* _DEBUG_H_ */