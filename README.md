<h1 align="center">sdFAT running on M5Stack</h1>

---

<p align="center"> how to use sdFAT library on M5Stack in short.
    <br> 
</p>

## Table of Contents

- [About <a name = "about"></a>](#about-)
- [Usage <a name="usage"></a>](#usage-)
- [Acknowledgements <a name = "acknowledgement"></a>](#acknowledgements-)

# About <a name = "about"></a>

I wanted to use the sdFAT library on the M5Stack because I need the long file names.

I succeeded in doing this after a few attempts, which I would like to document here.

# Usage <a name="usage"></a>

The original SD library is integrated in the M5Stack library.
The sdFAT library can be used in parallel if a few settings are made correctly.

There are 3 files in the src folder, with Arduino IDE they have to be renamed from *.cpp to *.ino.

- **sdFAT_M5Stack_test.cpp** <br>
  This is the first attempt to use the lib with M5Stack.

- **sdFAT_bench.cpp** <br>
  This is a benchmark with the new library.
 
- **SD_bench.cpp** <br>
  This is a benchmark with the original library.
<br><br>
  <h3>pay attention in using sdFAT library</h3><br>

- the SPI bus on M5Stack is shared with the TFT<br>
  https://docs.m5stack.com/#/en/core/gray
- the TFCARD_CS_PIN is defined in M5Stack Config.h (Pin 4)<br>
  https://github.com/m5stack/M5Stack/blob/6cd8ce15e723c2043a3c01763320413ebff300d5/src/utility/Config.h#L14
- the SPI bus read frequency for the TFT is 16 MHz<br>
  https://github.com/m5stack/M5Stack/blob/6cd8ce15e723c2043a3c01763320413ebff300d5/src/utility/In_eSPI_Setup.h#L266
- max speed of SPI bus for SD card is 40 MHz<br>
  https://github.com/m5stack/M5Stack/blob/6cd8ce15e723c2043a3c01763320413ebff300d5/src/M5Stack.cpp#L27   
- **the max speed of SPI with sdFAT library is 25 MHz, higher speed will cause errors**<br>
  I don't know, why there are these speed issues, but the sdFAT library is at 16 MHz slightly faster then the original one. <br>
The original code of the benchmark is located at:
https://codebender.cc/example/SdFat/benchSD#benchSD.ino
<br><br>
```cpp
// in main file define the max SPI speed
#define SPI_SPEED SD_SCK_MHZ(25)

if (!sd.begin(TFCARD_CS_PIN, SPI_SPEED))
{}
```
 
```txt 
sdFAT benchmark on M5Stack    
##########################
Type any character to start
File size 5MB        
Buffer size 100 bytes
Starting write test.  Please wait up to a minute
Write 77.38 KB/sec
Maximum latency: 146413 usec, Minimum Latency: 1 usec, Avg Latency: 1291 usec

Starting read test.  Please wait up to a minute
Read 296.52 KB/sec
Maximum latency: 10455 usec, Minimum Latency: 1 usec, Avg Latency: 336 usec

Done


SD benchmark on M5Stack    
#######################
Type any character to start
File size 5MB
Buffer size 100 bytes
Starting write test.  Please wait up to a minute
Write 68.35 KB/sec
Maximum latency: 191672 usec, Minimum Latency: 12 usec, Avg Latency: 1454 usec

Starting read test.  Please wait up to a minute
Read 258.91 KB/sec
Maximum latency: 3927 usec, Minimum Latency: 9 usec, Avg Latency: 382 usec

Done
```

# Acknowledgements <a name = "acknowledgement"></a>

- thanks to the makers of M5Stack, I like to have a nice all in one case in my projects :-)
- thanks to Bill Greiman for the sdFAT library https://github.com/greiman/SdFat

